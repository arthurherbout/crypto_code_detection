#define _GNU_SOURCE
#include <stdio.h>
#include <wchar.h>
#include <curses.h>
#include <dialog.h>
#include <assert.h>

#include "render.h"
#include "utils.h"
#include "ncurses_convenience.h"
#include "procedure.h"
#include "vis.h"
#include "copy.h"

#define LEGEND_WIDTH 20

typedef struct blk_report {
    uint64_t seqno;
    DC_BlockReport report;
} blk_report_t;

typedef struct {
    WINDOW *legend; // not for updating, just to free afterwards
    WINDOW *w_stats;
    int vis_height;
    int vis_width;
    WINDOW *vis; // window to print vis-char for each block
    WINDOW *avg_speed;
    //WINDOW *cur_speed;
    WINDOW *eta;
    //WINDOW *progress;
    WINDOW *summary;
    WINDOW *w_end_lba;
    WINDOW *w_cur_lba;

    struct timespec start_time;
    uint64_t access_time_stats_accum[6];
    uint64_t error_stats_accum[6]; // 0th is unused, the rest are as in DC_BlockStatus enum
    uint64_t bytes_processed;
    uint64_t avg_processing_speed;
    uint64_t eta_time; // estimated time
    uint64_t reports_handled;
    uint64_t cur_lba;
    uint64_t errors_count;  // This one is used, error_stats_accum is currently not
    uint64_t unread_count;
    uint64_t read_ok_count;

    pthread_t render_thread;
    int order_hangup; // if interrupted or completed, render remainings and end render thread

    // lockless ringbuffer
    blk_report_t reports[100*1000];
    uint64_t next_report_seqno_write;
    uint64_t next_report_seqno_read;

    int64_t nb_blocks;
    int64_t blocks_per_vis;
    int sectors_per_block;
    uint8_t *blocks_map;
} WholeSpace;



static void *render_thread_proc(void *arg);
static void blk_rep_write_finalize(WholeSpace *priv, blk_report_t *rep);
static blk_report_t *blk_rep_get_next_for_write(WholeSpace *priv);
static void update_blocks_info(WholeSpace *priv, blk_report_t *rep);
static void render_update_stats(WholeSpace *priv);

static blk_report_t *blk_rep_get_next_for_write(WholeSpace *priv) {
    blk_report_t *rep = &priv->reports[
        (priv->next_report_seqno_write) % (sizeof(priv->reports) / sizeof(priv->reports[0]))
        ];
    //fprintf(stderr, "giving %p for write\n", rep);
    return rep;
}

static void blk_rep_write_finalize(WholeSpace *priv, blk_report_t *rep) {
    rep->seqno = priv->next_report_seqno_write;
    priv->next_report_seqno_write++;
    //fprintf(stderr, "mark %p with seqno %"PRIu64", go to next\n", rep, rep->seqno);
}

static blk_report_t *blk_rep_get_unread(WholeSpace *priv) {
    blk_report_t *rep = &priv->reports[
        priv->next_report_seqno_read % (sizeof(priv->reports) / sizeof(priv->reports[0]))
        ];
    return rep;
}

static blk_report_t *blk_rep_read(WholeSpace *priv) {
    blk_report_t *rep = blk_rep_get_unread(priv);
    priv->next_report_seqno_read++;
    return rep;
}

static int get_queue_length(WholeSpace *priv) {
    return priv->next_report_seqno_write - priv->next_report_seqno_read;
}

static void render_map(WholeSpace *priv) {
    int i;
    // Clear vis window
    werase(priv->vis);
    for (i = 0; i < priv->nb_blocks; i += priv->blocks_per_vis) {
        int64_t end_blk_index = (i + priv->blocks_per_vis < priv->nb_blocks) ? i + priv->blocks_per_vis : priv->nb_blocks;
        int j;
        int this_cell_fully_processed = 1;
        int errors_in_this_cell = 0;
        for (j = i; j < end_blk_index; j++) {
            if (priv->blocks_map[j] == 2) {
                errors_in_this_cell = 1;
                break;
            }
            if (!priv->blocks_map[j])
                this_cell_fully_processed = 0;
        }
        if (errors_in_this_cell)
            print_vis(priv->vis, error_vis[3]);
        else if (!this_cell_fully_processed)
            print_vis(priv->vis, bs_vis[0]);  // gray light shade
        else
            print_vis(priv->vis, bs_vis[3]);  // green shade
    }
    wnoutrefresh(priv->vis);
}

static void render_queued(WholeSpace *priv) {
    int queue_length = get_queue_length(priv);
    while (queue_length) {
        blk_report_t *cur_rep = blk_rep_read(priv);
        update_blocks_info(priv, cur_rep);
        queue_length--;
    }
    render_update_stats(priv);
    render_map(priv);
    doupdate();
}

static void *render_thread_proc(void *arg) {
    WholeSpace *priv = arg;
    // TODO block signals in priv thread
    while (!priv->order_hangup) {
        render_queued(priv);
        usleep(40000);  // 25 Hz
    }
    render_queued(priv);
    return NULL;
}

static void update_blocks_info(WholeSpace *priv, blk_report_t *rep) {
    uint8_t *map_pointer = &priv->blocks_map[rep->report.lba / priv->sectors_per_block];
    if (rep->report.blk_status)
    {
        priv->error_stats_accum[rep->report.blk_status]++;
        *map_pointer = 2;  // block processed with failure result
        priv->errors_count += rep->report.sectors_processed;
    }
    else
    {
        *map_pointer = 1;  //block processed successfully
        unsigned int i;
        for (i = 0; i < 5; i++)
            if (rep->report.blk_access_time < bs_vis[i].access_time) {
                priv->access_time_stats_accum[i]++;
                break;
            }
        if (i == 5)
            priv->access_time_stats_accum[5]++; // of exceed
        priv->read_ok_count += rep->report.sectors_processed;
    }
    priv->unread_count -= rep->report.sectors_processed;
    wnoutrefresh(priv->vis);
}

static void render_update_stats(WholeSpace *priv) {
    if (priv->avg_processing_speed != 0) {
        werase(priv->avg_speed);
        wprintw(priv->avg_speed, "SPEED %7"PRIu64" kb/s", priv->avg_processing_speed / 1024);
        wnoutrefresh(priv->avg_speed);
    }

    if (priv->eta_time != 0) {
        unsigned int minute, second;
        second = priv->eta_time % 60;
        minute = priv->eta_time / 60;
        werase(priv->eta);
        wprintw(priv->eta, "ETA %11u:%02u", minute, second);
        wnoutrefresh(priv->eta);
    }

    werase(priv->w_cur_lba);
    char comma_lba_buf[30], *comma_lba_p;
    comma_lba_p = commaprint(priv->cur_lba, comma_lba_buf, sizeof(comma_lba_buf));
    wprintw(priv->w_cur_lba, "LBA: %14s", comma_lba_p);
    wnoutrefresh(priv->w_cur_lba);

    werase(priv->w_stats);
    print_vis(priv->w_stats, bs_vis[0]);
    wattrset(priv->w_stats, A_NORMAL);
    wprintw(priv->w_stats, " %"PRIu64"\n", priv->unread_count);

    print_vis(priv->w_stats, bs_vis[3]);
    wattrset(priv->w_stats, A_NORMAL);
    wprintw(priv->w_stats, " %"PRIu64"\n", priv->read_ok_count);

    print_vis(priv->w_stats, error_vis[3]);
    wattrset(priv->w_stats, A_NORMAL);
    wprintw(priv->w_stats, " %"PRIu64"\n", priv->errors_count);

    wnoutrefresh(priv->w_stats);
}

void whole_space_show_legend(WholeSpace *priv) {
    WINDOW *win = priv->legend;
    print_vis(win, bs_vis[0]);
    wattrset(win, A_NORMAL);
    wprintw(win, " unread space\n\n");

    print_vis(win, bs_vis[3]);
    wattrset(win, A_NORMAL);
    wprintw(win, " copied space,\n  no read errors\n");

    print_vis(win, error_vis[3]);
    wattrset(win, A_NORMAL);
    wprintw(win, " read errors\n  occured\n");

    wprintw(win, "Display block is %"PRId64" blocks by %d sectors\n",
            priv->blocks_per_vis, priv->sectors_per_block);
    wrefresh(win);
}

/*
25x80

                                                             <--LEGEND_WIDTH=20->
+--------------------------------------------------------------------------------+
|                   LBA:       xxx,xxx / xxx,xxx,xxx         ETA           xx:xx |
|ZZZZZZZZZZZZZZZxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx SPEED    xxxxx kb/s |
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                     |
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx x unread space      |^
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                     ||
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx x copied space,     ||
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx   no read errors    ||
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx x read errors       ||
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx   occured           || LEGEND_HEIGHT=9
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Display block is XXX||
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx XXX blocks by 256 se||
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx ctors               |v
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx x NNNNNNNNNN        |^
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx x NNNNNNNNNN        || W_STATS_HEIGHT=3
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx x NNNNNNNNNN        |v
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                     |
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Device copying /dev/|^
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx XXX                 ||
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Ctrl+C to abort     ||
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                     || SUMMARY
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                     ||
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                     ||
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                     ||
|xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                               |v
| WHDD rev. X.X-X-gXXXXXXX                                                       |
+--------------------------------------------------------------------------------+
*/
static int Open(DC_RendererCtx *ctx) {
    WholeSpace *priv = ctx->priv;
    DC_ProcedureCtx *actctx = ctx->procedure_ctx;

    // TODO Raise error message
    if (LINES < 25 || COLS < 80)
        return -1;

    priv->nb_blocks = actctx->dev->capacity / actctx->blk_size;
    if (actctx->dev->capacity % actctx->blk_size)
        priv->nb_blocks++;
    priv->unread_count = actctx->progress.den;
    priv->sectors_per_block = actctx->blk_size / 512;
    priv->blocks_map = calloc(priv->nb_blocks, sizeof(uint8_t));
    assert(priv->blocks_map);
    int journal_fd = ((CopyPriv*)actctx->priv)->journal_fd;
    lseek(journal_fd, 0, SEEK_SET);
    if (((CopyPriv*)actctx->priv)->use_journal) {
        priv->unread_count = 0;
        uint8_t journal_chunk[1*1024*1024];
        int64_t chunklen;
        int64_t end_lba = ((CopyPriv*)actctx->priv)->end_lba;
        for (int64_t i = 0; i < priv->nb_blocks; i++) {
            int64_t lba = i * priv->sectors_per_block;
            if (lba % sizeof(journal_chunk) == 0) {
                chunklen = (end_lba - lba) < (int64_t)sizeof(journal_chunk) ? (end_lba - lba) : (int64_t)sizeof(journal_chunk);
                int ret = read(journal_fd, journal_chunk, chunklen);
                if (ret != chunklen)
                    return 1;
            }
            char sector_status = journal_chunk[lba % sizeof(journal_chunk)];
            priv->blocks_map[i] = sector_status;
            int sectors_in_block = priv->sectors_per_block;
            if (i == priv->nb_blocks - 1)  // Last block may be smaller
                sectors_in_block = (actctx->dev->capacity % actctx->blk_size) / 512;
            switch ((enum SectorStatus)sector_status) {
                case SectorStatus_eUnread:
                    priv->unread_count += sectors_in_block;
                    break;
                case SectorStatus_eReadOk:
                    priv->read_ok_count += sectors_in_block;
                    break;
                case SectorStatus_eBlockReadError:
                case SectorStatus_eSectorReadError:
                    priv->errors_count += sectors_in_block;
                    break;
            }
        }
    }

#define LBA_WIDTH 20
#define LEGEND_WIDTH 20
#define LEGEND_HEIGHT 9
#define LEGEND_VERT_OFFSET 3 /* ETA & SPEED are above, 1 for spacing */

    priv->w_cur_lba = derwin(stdscr, 1, LBA_WIDTH, 0 /* at the top */, COLS - LEGEND_WIDTH - 1 - (LBA_WIDTH * 2) );
    assert(priv->w_cur_lba);
    wbkgd(priv->w_cur_lba, COLOR_PAIR(MY_COLOR_GRAY));

    priv->w_end_lba = derwin(stdscr, 1, LBA_WIDTH, 0 /* at the top */, COLS - LEGEND_WIDTH - 1 - LBA_WIDTH);
    assert(priv->w_end_lba);
    wbkgd(priv->w_end_lba, COLOR_PAIR(MY_COLOR_GRAY));

    priv->eta = derwin(stdscr, 1, LEGEND_WIDTH, 0 /* at the top */, COLS-LEGEND_WIDTH);
    assert(priv->eta);
    wbkgd(priv->eta, COLOR_PAIR(MY_COLOR_GRAY));

    priv->avg_speed = derwin(stdscr, 1, LEGEND_WIDTH, 1 /* ETA is above */, COLS-LEGEND_WIDTH);
    assert(priv->avg_speed);
    wbkgd(priv->avg_speed, COLOR_PAIR(MY_COLOR_GRAY));

    priv->legend = derwin(stdscr, LEGEND_HEIGHT, LEGEND_WIDTH, LEGEND_VERT_OFFSET, COLS-LEGEND_WIDTH);
    assert(priv->legend);
    wbkgd(priv->legend, COLOR_PAIR(MY_COLOR_GRAY));

#define W_STATS_HEIGHT 3
#define W_STATS_VERT_OFFSET ( LEGEND_VERT_OFFSET + LEGEND_HEIGHT + 1 /* spacing */ )
    priv->w_stats = derwin(stdscr, W_STATS_HEIGHT, LEGEND_WIDTH, W_STATS_VERT_OFFSET, COLS-LEGEND_WIDTH);
    assert(priv->w_stats);

#define SUMMARY_VERT_OFFSET ( W_STATS_VERT_OFFSET + W_STATS_HEIGHT + 1 /* spacing */ )
#define SUMMARY_HEIGHT ( LINES - SUMMARY_VERT_OFFSET - 1 /* don't touch bottom line */ )
    priv->summary = derwin(stdscr, SUMMARY_HEIGHT, LEGEND_WIDTH, SUMMARY_VERT_OFFSET, COLS-LEGEND_WIDTH);
    assert(priv->summary);
    wbkgd(priv->summary, COLOR_PAIR(MY_COLOR_GRAY));

    priv->vis_height = LINES - 2; /* LBA is above, version is below */
    priv->vis_width = COLS - LEGEND_WIDTH - 1;
    int vis_cells_avail = priv->vis_height * priv->vis_width;
    priv->blocks_per_vis = priv->nb_blocks / vis_cells_avail;
    if (priv->nb_blocks % vis_cells_avail)
        priv->blocks_per_vis++;
    priv->vis = derwin(stdscr, priv->vis_height, priv->vis_width, 1 /* LBA is above */, 0);
    assert(priv->vis);
    wrefresh(priv->vis);

    whole_space_show_legend(priv);

    priv->reports[0].seqno = 1; // anything but zero

    char comma_lba_buf[30], *comma_lba_p;
    comma_lba_p = commaprint(actctx->dev->capacity / 512, comma_lba_buf, sizeof(comma_lba_buf));
    wprintw(priv->w_end_lba, "/ %s", comma_lba_p);
    wnoutrefresh(priv->w_end_lba);
    wprintw(priv->summary,
            "%s %s\n"
            "Ctrl+C to abort\n",
            actctx->procedure->display_name, actctx->dev->dev_path);
    wrefresh(priv->summary);
    int r = pthread_create(&priv->render_thread, NULL, render_thread_proc, priv);
    if (r)
        return r; // FIXME leak
    return 0;
}

static int HandleReport(DC_RendererCtx *ctx) {
    int r;
    WholeSpace *priv = ctx->priv;
    DC_ProcedureCtx *actctx = ctx->procedure_ctx;

    priv->bytes_processed += actctx->report.sectors_processed * 512;
    priv->cur_lba = actctx->report.lba + actctx->report.sectors_processed;

    priv->reports_handled++;
    if (priv->reports_handled == 1) {  // TODO fix priv hack
        r = clock_gettime(DC_BEST_CLOCK, &priv->start_time);
        assert(!r);
    } else {
        if ((priv->reports_handled % 10) == 0) {
            struct timespec now;
            r = clock_gettime(DC_BEST_CLOCK, &now);
            assert(!r);
            uint64_t time_elapsed_ms = now.tv_sec * 1000 + now.tv_nsec / (1000*1000)
                - priv->start_time.tv_sec * 1000 - priv->start_time.tv_nsec / (1000*1000);
            if (time_elapsed_ms > 0) {
                priv->avg_processing_speed = priv->bytes_processed * 1000 / time_elapsed_ms; // Byte/s
                // capacity / speed = total_time
                // total_time = elapsed + eta
                // eta = total_time - elapsed
                // eta = capacity / speed  -  elapsed
                priv->eta_time = actctx->dev->capacity / priv->avg_processing_speed - time_elapsed_ms / 1000;

            }
        }
    }

    // enqueue block report
    blk_report_t *rep = blk_rep_get_next_for_write(priv);
    assert(rep);
    rep->report = actctx->report;
    blk_rep_write_finalize(priv, rep);
    //fprintf(stderr, "finalized %"PRIu64"\n", priv->next_report_seqno_write-1);
    return 0;
}

static void Close(DC_RendererCtx *ctx) {
    WholeSpace *priv = ctx->priv;
    DC_ProcedureCtx *actctx = ctx->procedure_ctx;

    priv->order_hangup = 1;
    pthread_join(priv->render_thread, NULL);
    if (actctx->interrupt)
        wprintw(priv->summary, "Aborted.\n");
    else
        wprintw(priv->summary, "Completed.\n");
    wprintw(priv->summary, "Press 'm' for menu");
    wrefresh(priv->summary);
    beep();
    while (getchar() != 'm')
        ;
    delwin(priv->legend);
    delwin(priv->vis);
    delwin(priv->avg_speed);
    delwin(priv->eta);
    delwin(priv->summary);
    delwin(priv->w_end_lba);
    delwin(priv->w_cur_lba);
    clear_body();
    free(priv->blocks_map);
}

DC_Renderer whole_space = {
    .name = "whole_space",
    .open = Open,
    .handle_report = HandleReport,
    .close = Close,
    .priv_data_size = sizeof(WholeSpace),
};
