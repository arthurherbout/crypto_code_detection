/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Copyright 2016 Joyent, Inc.
 */

/*
 * Polled I/O safe ANSI terminal emulator module;
 * Supporting TERM types 'sun' and 'sun-color, parsing
 * ANSI x3.64 escape sequences, and the like.  (See wscons(7d)
 * for more information).
 *
 * IMPORTANT:
 *
 *   The functions in this file *must* be able to function in
 *   standalone mode, ie. on a quiesced system.   In that state,
 *   access is single threaded, only one CPU is running.
 *   System services are NOT available.
 *
 * The following restrictions pertain to every function
 * in this file:
 *
 *     - CANNOT use the DDI or LDI interfaces
 *     - CANNOT call system services
 *     - CANNOT use mutexes
 *     - CANNOT wait for interrupts
 *     - CANNOT allocate memory
 *
 * All non-static functions in this file which:
 *     - Operates on tems and tem_vt_state
 *     - Not only called from standalone mode, i.e. has
 *       a "calledfrom" argument
 * should assert this at the beginning:
 *
 *    ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
 *        called_from == CALLED_FROM_STANDALONE);
 */

#include <sys/types.h>
#include <sys/ascii.h>
#include <sys/visual_io.h>
#include <sys/font.h>
#include <sys/tem.h>
#include <sys/tem_impl.h>
#include <sys/ksynch.h>
#include <sys/sysmacros.h>
#include <sys/mutex.h>
#include <sys/note.h>
#include <sys/t_lock.h>

tem_safe_callbacks_t tem_safe_text_callbacks = {
	&tem_safe_text_display,
	&tem_safe_text_copy,
	&tem_safe_text_cursor,
	NULL,
	&tem_safe_text_cls
};
tem_safe_callbacks_t tem_safe_pix_callbacks = {
	&tem_safe_pix_display,
	&tem_safe_pix_copy,
	&tem_safe_pix_cursor,
	&tem_safe_pix_bit2pix,
	&tem_safe_pix_cls
};

static void	tem_safe_control(struct tem_vt_state *, tem_char_t,
			cred_t *, enum called_from);
static void	tem_safe_setparam(struct tem_vt_state *, int, int);
static void	tem_safe_selgraph(struct tem_vt_state *);
static void	tem_safe_chkparam(struct tem_vt_state *, tem_char_t,
			cred_t *, enum called_from);
static void	tem_safe_getparams(struct tem_vt_state *, tem_char_t,
			cred_t *, enum called_from);
static void	tem_safe_outch(struct tem_vt_state *, tem_char_t,
			cred_t *, enum called_from);
static void	tem_safe_parse(struct tem_vt_state *, tem_char_t,
			cred_t *, enum called_from);

static void	tem_safe_new_line(struct tem_vt_state *,
			cred_t *, enum called_from);
static void	tem_safe_cr(struct tem_vt_state *);
static void	tem_safe_lf(struct tem_vt_state *,
			cred_t *, enum called_from);
static void	tem_safe_send_data(struct tem_vt_state *, cred_t *,
			enum called_from);
static void	tem_safe_cls(struct tem_vt_state *,
			cred_t *, enum called_from);
static void	tem_safe_tab(struct tem_vt_state *,
			cred_t *, enum called_from);
static void	tem_safe_back_tab(struct tem_vt_state *,
			cred_t *, enum called_from);
static void	tem_safe_clear_tabs(struct tem_vt_state *, int);
static void	tem_safe_set_tab(struct tem_vt_state *);
static void	tem_safe_mv_cursor(struct tem_vt_state *, int, int,
			cred_t *, enum called_from);
static void	tem_safe_shift(struct tem_vt_state *, int, int,
			cred_t *, enum called_from);
static void	tem_safe_scroll(struct tem_vt_state *, int, int,
			int, int, cred_t *, enum called_from);
static void	tem_safe_clear_chars(struct tem_vt_state *tem,
			int count, screen_pos_t row, screen_pos_t col,
			cred_t *credp, enum called_from called_from);
static void	tem_safe_copy_area(struct tem_vt_state *tem,
			screen_pos_t s_col, screen_pos_t s_row,
			screen_pos_t e_col, screen_pos_t e_row,
			screen_pos_t t_col, screen_pos_t t_row,
			cred_t *credp, enum called_from called_from);
static void	tem_safe_image_display(struct tem_vt_state *, uchar_t *,
			int, int, screen_pos_t, screen_pos_t,
			cred_t *, enum called_from);
static void	tem_safe_bell(struct tem_vt_state *tem,
			enum called_from called_from);
static void	tem_safe_pix_clear_prom_output(struct tem_vt_state *tem,
			cred_t *credp, enum called_from called_from);
static void	tem_safe_get_color(text_color_t *, text_color_t *, term_char_t);

static void	tem_safe_virtual_cls(struct tem_vt_state *, int, screen_pos_t,
		    screen_pos_t);
static void	tem_safe_virtual_display(struct tem_vt_state *,
		    term_char_t *, int, screen_pos_t, screen_pos_t);
static void	tem_safe_virtual_copy(struct tem_vt_state *, screen_pos_t,
		    screen_pos_t, screen_pos_t, screen_pos_t,
		    screen_pos_t, screen_pos_t);
static void	tem_safe_align_cursor(struct tem_vt_state *tem);
static void	bit_to_pix4(struct tem_vt_state *tem, tem_char_t c,
		    text_color_t fg_color, text_color_t bg_color);
static void	bit_to_pix8(struct tem_vt_state *tem, tem_char_t c,
		    text_color_t fg_color, text_color_t bg_color);
static void	bit_to_pix16(struct tem_vt_state *tem, tem_char_t c,
		    text_color_t fg_color, text_color_t bg_color);
static void	bit_to_pix24(struct tem_vt_state *tem, tem_char_t c,
		    text_color_t fg_color, text_color_t bg_color);
static void	bit_to_pix32(struct tem_vt_state *tem, tem_char_t c,
		    text_color_t fg_color, text_color_t bg_color);

#define	PIX4TO32(pix4) (uint32_t)(  \
    cmap4_to_24.red[pix4] << 16 |  \
    cmap4_to_24.green[pix4] << 8 | \
    cmap4_to_24.blue[pix4])

#define	INVERSE(ch) (ch ^ 0xff)

#define	tem_safe_callback_display	(*tems.ts_callbacks->tsc_display)
#define	tem_safe_callback_copy		(*tems.ts_callbacks->tsc_copy)
#define	tem_safe_callback_cursor	(*tems.ts_callbacks->tsc_cursor)
#define	tem_safe_callback_cls		(*tems.ts_callbacks->tsc_cls)
#define	tem_safe_callback_bit2pix(tem, c)	{		\
	ASSERT(tems.ts_callbacks->tsc_bit2pix != NULL);			\
	(void) (*tems.ts_callbacks->tsc_bit2pix)((tem), (c));\
}

void
tem_safe_check_first_time(
    struct tem_vt_state *tem,
    cred_t *credp,
    enum called_from called_from)
{
	static int first_time = 1;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	/*
	 * Realign the console cursor. We did this in tem_init().
	 * However, drivers in the console stream may emit additional
	 * messages before we are ready. This causes text overwrite
	 * on the screen. This is a workaround.
	 */
	if (!first_time)
		return;

	first_time = 0;
	if (tems.ts_display_mode == VIS_TEXT)
		tem_safe_text_cursor(tem, VIS_GET_CURSOR, credp, called_from);
	else
		tem_safe_pix_cursor(tem, VIS_GET_CURSOR, credp, called_from);
	tem_safe_align_cursor(tem);
}

/*
 * This entry point handles output requests from restricted contexts like
 * kmdb, where services like mutexes are not available. This function
 * is entered when OBP or when a kernel debugger (such as kmdb)
 * are generating console output.  In those cases, power management
 * concerns are handled by the abort sequence initiation (ie. when
 * the user hits L1+A or the equivalent to enter OBP or the debugger.).
 * It is also entered when the kernel is panicing.
 */
void
tem_safe_polled_write(
    tem_vt_state_t tem_arg,
    uchar_t *buf,
    int len)
{
	struct tem_vt_state *tem = (struct tem_vt_state *)tem_arg;

#ifdef	__lock_lint
	_NOTE(NO_COMPETING_THREADS_NOW)
	_NOTE(NO_COMPETING_THREADS_AS_SIDE_EFFECT)
#endif

	if (!tem->tvs_initialized) {
		return;
	}

	tem_safe_check_first_time(tem, kcred, CALLED_FROM_STANDALONE);
	tem_safe_terminal_emulate(tem, buf, len, NULL, CALLED_FROM_STANDALONE);
}

/* Process partial UTF-8 sequence. */
static void
tem_safe_input_partial(struct tem_vt_state *tem, cred_t *credp,
    enum called_from called_from)
{
	unsigned i;
	uint8_t c;

	if (tem->tvs_utf8_left == 0)
		return;

	for (i = 0; i < sizeof (tem->tvs_utf8_partial); i++) {
		c = (tem->tvs_utf8_partial >> (24 - (i << 3))) & 0xff;
		if (c != 0) {
			tem_safe_parse(tem, c, credp, called_from);
		}
	}
	tem->tvs_utf8_left = 0;
	tem->tvs_utf8_partial = 0;
}

/*
 * Handle UTF-8 sequences.
 */
static void
tem_safe_input_byte(struct tem_vt_state *tem, uchar_t c, cred_t *credp,
    enum called_from called_from)
{
	/*
	 * Check for UTF-8 code points. In case of error fall back to
	 * 8-bit code. As we only have 8859-1 fonts for console, this will set
	 * the limits on what chars we actually can display, therefore we
	 * have to return to this code once we have solved the font issue.
	 */
	if ((c & 0x80) == 0x00) {
		/* One-byte sequence. */
		tem_safe_input_partial(tem, credp, called_from);
		tem_safe_parse(tem, c, credp, called_from);
		return;
	}
	if ((c & 0xe0) == 0xc0) {
		/* Two-byte sequence. */
		tem_safe_input_partial(tem, credp, called_from);
		tem->tvs_utf8_left = 1;
		tem->tvs_utf8_partial = c;
		return;
	}
	if ((c & 0xf0) == 0xe0) {
		/* Three-byte sequence. */
		tem_safe_input_partial(tem, credp, called_from);
		tem->tvs_utf8_left = 2;
		tem->tvs_utf8_partial = c;
		return;
	}
	if ((c & 0xf8) == 0xf0) {
		/* Four-byte sequence. */
		tem_safe_input_partial(tem, credp, called_from);
		tem->tvs_utf8_left = 3;
		tem->tvs_utf8_partial = c;
		return;
	}
	if ((c & 0xc0) == 0x80) {
		/* Invalid state? */
		if (tem->tvs_utf8_left == 0) {
			tem_safe_parse(tem, c, credp, called_from);
			return;
		}
		tem->tvs_utf8_left--;
		tem->tvs_utf8_partial = (tem->tvs_utf8_partial << 8) | c;
		if (tem->tvs_utf8_left == 0) {
			tem_char_t v, u;
			uint8_t b;

			/*
			 * Transform the sequence of 2 to 4 bytes to
			 * unicode number.
			 */
			v = 0;
			u = tem->tvs_utf8_partial;
			b = (u >> 24) & 0xff;
			if (b != 0) {		/* Four-byte sequence */
				v = b & 0x07;
				b = (u >> 16) & 0xff;
				v = (v << 6) | (b & 0x3f);
				b = (u >> 8) & 0xff;
				v = (v << 6) | (b & 0x3f);
				b = u & 0xff;
				v = (v << 6) | (b & 0x3f);
			} else if ((b = (u >> 16) & 0xff) != 0) {
				v = b & 0x0f;	/* Three-byte sequence */
				b = (u >> 8) & 0xff;
				v = (v << 6) | (b & 0x3f);
				b = u & 0xff;
				v = (v << 6) | (b & 0x3f);
			} else if ((b = (u >> 8) & 0xff) != 0) {
				v = b & 0x1f;	/* Two-byte sequence */
				b = u & 0xff;
				v = (v << 6) | (b & 0x3f);
			}

			tem_safe_parse(tem, v, credp, called_from);
			tem->tvs_utf8_partial = 0;
		}
		return;
	}
	/* Anything left is illegal in UTF-8 sequence. */
	tem_safe_input_partial(tem, credp, called_from);
	tem_safe_parse(tem, c, credp, called_from);
}

/*
 * This is the main entry point into the terminal emulator.
 *
 * For each data message coming downstream, ANSI assumes that it is composed
 * of ASCII characters, which are treated as a byte-stream input to the
 * parsing state machine. All data is parsed immediately -- there is
 * no enqueing.
 */
void
tem_safe_terminal_emulate(
    struct tem_vt_state *tem,
    uchar_t *buf,
    int len,
    cred_t *credp,
    enum called_from called_from)
{

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	if (tem->tvs_isactive)
		tem_safe_callback_cursor(tem,
		    VIS_HIDE_CURSOR, credp, called_from);

	for (; len > 0; len--, buf++)
		tem_safe_input_byte(tem, *buf, credp, called_from);

	/*
	 * Send the data we just got to the framebuffer.
	 */
	tem_safe_send_data(tem, credp, called_from);

	if (tem->tvs_isactive)
		tem_safe_callback_cursor(tem,
		    VIS_DISPLAY_CURSOR, credp, called_from);
}

/*
 * Display an rectangular image on the frame buffer using the
 * mechanism appropriate for the system state being called
 * from quiesced or normal (ie. use polled I/O vs. layered ioctls)
 */
static void
tems_safe_display(struct vis_consdisplay *pda, cred_t *credp,
    enum called_from called_from)
{
	if (called_from == CALLED_FROM_STANDALONE)
		tems.ts_fb_polledio->display(tems.ts_fb_polledio->arg, pda);
	else
		tems_display_layered(pda, credp);
}

/*
 * Copy a rectangle from one location to another on the frame buffer
 * using the mechanism appropriate for the system state being called
 * from, quiesced or normal (ie. use polled I/O vs. layered ioctls)
 */
void
tems_safe_copy(struct vis_conscopy *pca, cred_t *credp,
    enum called_from called_from)
{
	if (called_from == CALLED_FROM_STANDALONE)
		tems.ts_fb_polledio->copy(tems.ts_fb_polledio->arg, pca);
	else
		tems_copy_layered(pca, credp);
}

/*
 * Display or hide a rectangular block text cursor of a specificsize
 * at a specific location on frame buffer* using the mechanism
 * appropriate for the system state being called from, quisced or
 * normal (ie. use polled I/O vs. layered ioctls).
 */
static void
tems_safe_cursor(struct vis_conscursor *pca, cred_t *credp,
    enum called_from called_from)
{
	if (called_from == CALLED_FROM_STANDALONE)
		tems.ts_fb_polledio->cursor(tems.ts_fb_polledio->arg, pca);
	else
		tems_cursor_layered(pca, credp);
}

/*
 * send the appropriate control message or set state based on the
 * value of the control character ch
 */

static void
tem_safe_control(struct tem_vt_state *tem, tem_char_t ch, cred_t *credp,
    enum called_from called_from)
{
	tem->tvs_state = A_STATE_START;
	switch (ch) {
	case A_BEL:
		tem_safe_bell(tem, called_from);
		break;

	case A_BS:
		tem_safe_mv_cursor(tem,
		    tem->tvs_c_cursor.row,
		    tem->tvs_c_cursor.col - 1,
		    credp, called_from);
		break;

	case A_HT:
		tem_safe_tab(tem, credp, called_from);
		break;

	case A_NL:
		/*
		 * tem_safe_send_data(tem, credp, called_from);
		 * tem_safe_new_line(tem, credp, called_from);
		 * break;
		 */

	case A_VT:
		tem_safe_send_data(tem, credp, called_from);
		tem_safe_lf(tem, credp, called_from);
		break;

	case A_FF:
		tem_safe_send_data(tem, credp, called_from);
		tem_safe_cls(tem, credp, called_from);
		break;

	case A_CR:
		tem_safe_send_data(tem, credp, called_from);
		tem_safe_cr(tem);
		break;

	case A_ESC:
		tem->tvs_state = A_STATE_ESC;
		break;

	case A_CSI:
		{
			int i;
			tem->tvs_curparam = 0;
			tem->tvs_paramval = 0;
			tem->tvs_gotparam = B_FALSE;
			/* clear the parameters */
			for (i = 0; i < TEM_MAXPARAMS; i++)
				tem->tvs_params[i] = -1;
			tem->tvs_state = A_STATE_CSI;
		}
		break;

	case A_GS:
		tem_safe_back_tab(tem, credp, called_from);
		break;

	default:
		break;
	}
}


/*
 * if parameters [0..count - 1] are not set, set them to the value
 * of newparam.
 */

static void
tem_safe_setparam(struct tem_vt_state *tem, int count, int newparam)
{
	int i;

	for (i = 0; i < count; i++) {
		if (tem->tvs_params[i] == -1)
			tem->tvs_params[i] = newparam;
	}
}


static void
tem_select_color(struct tem_vt_state *tem, text_color_t color, boolean_t fg)
{
	if (tems.ts_pdepth >= 24 ||
	    (color < 8 && tems.ts_pdepth < 24)) {
		if (fg == B_TRUE) {
			tem->tvs_fg_color = color;
			tem->tvs_flags &= ~TEM_ATTR_BRIGHT_FG;
		} else {
			tem->tvs_bg_color = color;
			tem->tvs_flags &= ~TEM_ATTR_BRIGHT_BG;
		}
		return;
	}

	if (color > 15)
		return;

	/* Bright color and depth < 24 */
	color -= 8;
	if (fg == B_TRUE) {
		tem->tvs_fg_color = color;
		tem->tvs_flags |= TEM_ATTR_BRIGHT_FG;
	} else {
		tem->tvs_bg_color = color;
		tem->tvs_flags |= TEM_ATTR_BRIGHT_BG;
	}
}

/*
 * select graphics mode based on the param vals stored in a_params
 */
static void
tem_safe_selgraph(struct tem_vt_state *tem)
{
	int curparam;
	int count = 0;
	int param;

	tem->tvs_state = A_STATE_START;

	curparam = tem->tvs_curparam;
	do {
		param = tem->tvs_params[count];

		switch (param) {
		case -1:
		case 0:
			/* reset to initial normal settings */
			tem->tvs_fg_color = tems.ts_init_color.fg_color;
			tem->tvs_bg_color = tems.ts_init_color.bg_color;
			tem->tvs_flags = tems.ts_init_color.a_flags;
			break;

		case 1: /* Bold Intense */
			tem->tvs_flags |= TEM_ATTR_BOLD;
			break;

		case 2: /* Faint Intense */
			tem->tvs_flags &= ~TEM_ATTR_BOLD;
			break;

		case 4: /* Underline */
			tem->tvs_flags |= TEM_ATTR_UNDERLINE;
			break;
		case 5: /* Blink */
			tem->tvs_flags |= TEM_ATTR_BLINK;
			break;

		case 7: /* Reverse video */
			if (tem->tvs_flags & TEM_ATTR_SCREEN_REVERSE) {
				tem->tvs_flags &= ~TEM_ATTR_REVERSE;
			} else {
				tem->tvs_flags |= TEM_ATTR_REVERSE;
			}
			break;

		case 22: /* Remove Bold */
			tem->tvs_flags &= ~TEM_ATTR_BOLD;
			break;

		case 24: /* Remove Underline */
			tem->tvs_flags &= ~TEM_ATTR_UNDERLINE;
			break;

		case 25: /* Remove Blink */
			tem->tvs_flags &= ~TEM_ATTR_BLINK;
			break;

		case 27: /* Remove Reverse */
			if (tem->tvs_flags & TEM_ATTR_SCREEN_REVERSE) {
				tem->tvs_flags |= TEM_ATTR_REVERSE;
			} else {
				tem->tvs_flags &= ~TEM_ATTR_REVERSE;
			}
			break;

		case 30: /* black	(grey)		foreground */
		case 31: /* red		(light red)	foreground */
		case 32: /* green	(light green)	foreground */
		case 33: /* brown	(yellow)	foreground */
		case 34: /* blue	(light blue)	foreground */
		case 35: /* magenta	(light magenta)	foreground */
		case 36: /* cyan	(light cyan)	foreground */
		case 37: /* white	(bright white)	foreground */
			tem->tvs_fg_color = param - 30;
			tem->tvs_flags &= ~TEM_ATTR_BRIGHT_FG;
			break;

		case 38:
			/* We should have at least 3 parameters */
			if (curparam < 3)
				break;

			/*
			 * 256 and truecolor needs depth at least 24, but
			 * we still need to process the sequence.
			 */
			count++;
			curparam--;
			param = tem->tvs_params[count];
			switch (param) {
			case 5: /* 256 colors */
				count++;
				curparam--;
				tem_select_color(tem, tem->tvs_params[count],
				    B_TRUE);
				break;
			default:
				break;
			}
			break;

		case 39:
			/*
			 * Reset the foreground colour and brightness.
			 */
			tem->tvs_fg_color = tems.ts_init_color.fg_color;
			if (tems.ts_init_color.a_flags & TEM_ATTR_BRIGHT_FG)
				tem->tvs_flags |= TEM_ATTR_BRIGHT_FG;
			else
				tem->tvs_flags &= ~TEM_ATTR_BRIGHT_FG;
			break;

		case 40: /* black	(grey)		background */
		case 41: /* red		(light red)	background */
		case 42: /* green	(light green)	background */
		case 43: /* brown	(yellow)	background */
		case 44: /* blue	(light blue)	background */
		case 45: /* magenta	(light magenta)	background */
		case 46: /* cyan	(light cyan)	background */
		case 47: /* white	(bright white)	background */
			tem->tvs_bg_color = param - 40;
			tem->tvs_flags &= ~TEM_ATTR_BRIGHT_BG;
			break;

		case 48:
			/* We should have at least 3 parameters */
			if (curparam < 3)
				break;

			/*
			 * 256 and truecolor needs depth at least 24, but
			 * we still need to process the sequence.
			 */
			count++;
			curparam--;
			param = tem->tvs_params[count];
			switch (param) {
			case 5: /* 256 colors */
				count++;
				curparam--;
				tem_select_color(tem, tem->tvs_params[count],
				    B_FALSE);
				break;
			default:
				break;
			}
			break;

		case 49:
			/*
			 * Reset the background colour and brightness.
			 */
			tem->tvs_bg_color = tems.ts_init_color.bg_color;
			if (tems.ts_init_color.a_flags & TEM_ATTR_BRIGHT_BG)
				tem->tvs_flags |= TEM_ATTR_BRIGHT_BG;
			else
				tem->tvs_flags &= ~TEM_ATTR_BRIGHT_BG;
			break;

		case 90: /* black	(grey)		foreground */
		case 91: /* red		(light red)	foreground */
		case 92: /* green	(light green)	foreground */
		case 93: /* brown	(yellow)	foreground */
		case 94: /* blue	(light blue)	foreground */
		case 95: /* magenta	(light magenta)	foreground */
		case 96: /* cyan	(light cyan)	foreground */
		case 97: /* white	(bright white)	foreground */
			tem->tvs_fg_color = param - 90;
			tem->tvs_flags |= TEM_ATTR_BRIGHT_FG;
			break;

		case 100: /* black	(grey)		background */
		case 101: /* red	(light red)	background */
		case 102: /* green	(light green)	background */
		case 103: /* brown	(yellow)	background */
		case 104: /* blue	(light blue)	background */
		case 105: /* magenta	(light magenta)	background */
		case 106: /* cyan	(light cyan)	background */
		case 107: /* white	(bright white)	background */
			tem->tvs_bg_color = param - 100;
			tem->tvs_flags |= TEM_ATTR_BRIGHT_BG;
			break;

		default:
			break;
		}
		count++;
		curparam--;

	} while (curparam > 0);
}

/*
 * perform the appropriate action for the escape sequence
 *
 * General rule:  This code does not validate the arguments passed.
 *                It assumes that the next lower level will do so.
 */
static void
tem_safe_chkparam(struct tem_vt_state *tem, tem_char_t ch, cred_t *credp,
    enum called_from called_from)
{
	int	i;
	int	row;
	int	col;

	ASSERT((called_from == CALLED_FROM_STANDALONE) ||
	    MUTEX_HELD(&tem->tvs_lock));

	row = tem->tvs_c_cursor.row;
	col = tem->tvs_c_cursor.col;

	switch (ch) {

	case 'm': /* select terminal graphics mode */
		tem_safe_send_data(tem, credp, called_from);
		tem_safe_selgraph(tem);
		break;

	case '@':		/* insert char */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_shift(tem, tem->tvs_params[0], TEM_SHIFT_RIGHT,
		    credp, called_from);
		break;

	case 'A':		/* cursor up */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_mv_cursor(tem, row - tem->tvs_params[0], col,
		    credp, called_from);
		break;

	case 'd':		/* VPA - vertical position absolute */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_mv_cursor(tem, tem->tvs_params[0] - 1, col,
		    credp, called_from);
		break;

	case 'e':		/* VPR - vertical position relative */
	case 'B':		/* cursor down */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_mv_cursor(tem, row + tem->tvs_params[0], col,
		    credp, called_from);
		break;

	case 'a':		/* HPR - horizontal position relative */
	case 'C':		/* cursor right */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_mv_cursor(tem, row, col + tem->tvs_params[0],
		    credp, called_from);
		break;

	case '`':		/* HPA - horizontal position absolute */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_mv_cursor(tem, row, tem->tvs_params[0] - 1,
		    credp, called_from);
		break;

	case 'D':		/* cursor left */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_mv_cursor(tem, row, col - tem->tvs_params[0],
		    credp, called_from);
		break;

	case 'E':		/* CNL cursor next line */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_mv_cursor(tem, row + tem->tvs_params[0], 0,
		    credp, called_from);
		break;

	case 'F':		/* CPL cursor previous line */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_mv_cursor(tem, row - tem->tvs_params[0], 0,
		    credp, called_from);
		break;

	case 'G':		/* cursor horizontal position */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_mv_cursor(tem, row, tem->tvs_params[0] - 1,
		    credp, called_from);
		break;

	case 'g':		/* clear tabs */
		tem_safe_setparam(tem, 1, 0);
		tem_safe_clear_tabs(tem, tem->tvs_params[0]);
		break;

	case 'f':		/* HVP Horizontal and Vertical Position */
	case 'H':		/* CUP position cursor */
		tem_safe_setparam(tem, 2, 1);
		tem_safe_mv_cursor(tem,
		    tem->tvs_params[0] - 1,
		    tem->tvs_params[1] - 1,
		    credp, called_from);
		break;

	case 'I':		/* CHT - Cursor Horizontal Tab */
		/* Not implemented */
		break;

	case 'J':		/* ED - Erase in Display */
		tem_safe_send_data(tem, credp, called_from);
		tem_safe_setparam(tem, 1, 0);
		switch (tem->tvs_params[0]) {
		case 0:
			/* erase cursor to end of screen */
			/* FIRST erase cursor to end of line */
			tem_safe_clear_chars(tem,
			    tems.ts_c_dimension.width -
			    tem->tvs_c_cursor.col,
			    tem->tvs_c_cursor.row,
			    tem->tvs_c_cursor.col, credp, called_from);

			/* THEN erase lines below the cursor */
			for (row = tem->tvs_c_cursor.row + 1;
			    row < tems.ts_c_dimension.height;
			    row++) {
				tem_safe_clear_chars(tem,
				    tems.ts_c_dimension.width,
				    row, 0, credp, called_from);
			}
			break;

		case 1:
			/* erase beginning of screen to cursor */
			/* FIRST erase lines above the cursor */
			for (row = 0;
			    row < tem->tvs_c_cursor.row;
			    row++) {
				tem_safe_clear_chars(tem,
				    tems.ts_c_dimension.width,
				    row, 0, credp, called_from);
			}
			/* THEN erase beginning of line to cursor */
			tem_safe_clear_chars(tem,
			    tem->tvs_c_cursor.col + 1,
			    tem->tvs_c_cursor.row,
			    0, credp, called_from);
			break;

		case 2:
			/* erase whole screen */
			for (row = 0;
			    row < tems.ts_c_dimension.height;
			    row++) {
				tem_safe_clear_chars(tem,
				    tems.ts_c_dimension.width,
				    row, 0, credp, called_from);
			}
			break;
		}
		break;

	case 'K':		/* EL - Erase in Line */
		tem_safe_send_data(tem, credp, called_from);
		tem_safe_setparam(tem, 1, 0);
		switch (tem->tvs_params[0]) {
		case 0:
			/* erase cursor to end of line */
			tem_safe_clear_chars(tem,
			    (tems.ts_c_dimension.width -
			    tem->tvs_c_cursor.col),
			    tem->tvs_c_cursor.row,
			    tem->tvs_c_cursor.col,
			    credp, called_from);
			break;

		case 1:
			/* erase beginning of line to cursor */
			tem_safe_clear_chars(tem,
			    tem->tvs_c_cursor.col + 1,
			    tem->tvs_c_cursor.row,
			    0, credp, called_from);
			break;

		case 2:
			/* erase whole line */
			tem_safe_clear_chars(tem,
			    tems.ts_c_dimension.width,
			    tem->tvs_c_cursor.row,
			    0, credp, called_from);
			break;
		}
		break;

	case 'L':		/* insert line */
		tem_safe_send_data(tem, credp, called_from);
		tem_safe_setparam(tem, 1, 1);
		tem_safe_scroll(tem,
		    tem->tvs_c_cursor.row,
		    tems.ts_c_dimension.height - 1,
		    tem->tvs_params[0], TEM_SCROLL_DOWN,
		    credp, called_from);
		break;

	case 'M':		/* delete line */
		tem_safe_send_data(tem, credp, called_from);
		tem_safe_setparam(tem, 1, 1);
		tem_safe_scroll(tem,
		    tem->tvs_c_cursor.row,
		    tems.ts_c_dimension.height - 1,
		    tem->tvs_params[0], TEM_SCROLL_UP,
		    credp, called_from);
		break;

	case 'P':		/* DCH - delete char */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_shift(tem, tem->tvs_params[0], TEM_SHIFT_LEFT,
		    credp, called_from);
		break;

	case 'S':		/* scroll up */
		tem_safe_send_data(tem, credp, called_from);
		tem_safe_setparam(tem, 1, 1);
		tem_safe_scroll(tem, 0,
		    tems.ts_c_dimension.height - 1,
		    tem->tvs_params[0], TEM_SCROLL_UP,
		    credp, called_from);
		break;

	case 'T':		/* scroll down */
		tem_safe_send_data(tem, credp, called_from);
		tem_safe_setparam(tem, 1, 1);
		tem_safe_scroll(tem, 0,
		    tems.ts_c_dimension.height - 1,
		    tem->tvs_params[0], TEM_SCROLL_DOWN,
		    credp, called_from);
		break;

	case 'X':		/* erase char */
		tem_safe_setparam(tem, 1, 1);
		tem_safe_clear_chars(tem,
		    tem->tvs_params[0],
		    tem->tvs_c_cursor.row,
		    tem->tvs_c_cursor.col,
		    credp, called_from);
		break;

	case 'Z':		/* cursor backward tabulation */
		tem_safe_setparam(tem, 1, 1);

		/*
		 * Rule exception - We do sanity checking here.
		 *
		 * Restrict the count to a sane value to keep from
		 * looping for a long time.  There can't be more than one
		 * tab stop per column, so use that as a limit.
		 */
		if (tem->tvs_params[0] > tems.ts_c_dimension.width)
			tem->tvs_params[0] = tems.ts_c_dimension.width;

		for (i = 0; i < tem->tvs_params[0]; i++)
			tem_safe_back_tab(tem, credp, called_from);
		break;
	}
	tem->tvs_state = A_STATE_START;
}


/*
 * Gather the parameters of an ANSI escape sequence
 */
static void
tem_safe_getparams(struct tem_vt_state *tem, tem_char_t ch,
    cred_t *credp, enum called_from called_from)
{
	ASSERT((called_from == CALLED_FROM_STANDALONE) ||
	    MUTEX_HELD(&tem->tvs_lock));

	if (ch >= '0' && ch <= '9') {
		tem->tvs_paramval = ((tem->tvs_paramval * 10) + (ch - '0'));
		tem->tvs_gotparam = B_TRUE;  /* Remember got parameter */
		return; /* Return immediately */
	} else if (tem->tvs_state == A_STATE_CSI_EQUAL ||
	    tem->tvs_state == A_STATE_CSI_QMARK) {
		tem->tvs_state = A_STATE_START;
	} else {
		if (tem->tvs_curparam < TEM_MAXPARAMS) {
			if (tem->tvs_gotparam) {
				/* get the parameter value */
				tem->tvs_params[tem->tvs_curparam] =
				    tem->tvs_paramval;
			}
			tem->tvs_curparam++;
		}

		if (ch == ';') {
			/* Restart parameter search */
			tem->tvs_gotparam = B_FALSE;
			tem->tvs_paramval = 0; /* No parame value yet */
		} else {
			/* Handle escape sequence */
			tem_safe_chkparam(tem, ch, credp, called_from);
		}
	}
}

/*
 * Add character to internal buffer.
 * When its full, send it to the next layer.
 */

static void
tem_safe_outch(struct tem_vt_state *tem, tem_char_t ch,
    cred_t *credp, enum called_from called_from)
{
	text_color_t fg;
	text_color_t bg;
	text_attr_t attr;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	/* buffer up the character until later */
	tem_safe_get_attr(tem, &fg, &bg, &attr, TEM_ATTR_REVERSE);
	tem->tvs_outbuf[tem->tvs_outindex].tc_char = ch | TEM_ATTR(attr);
	tem->tvs_outbuf[tem->tvs_outindex].tc_fg_color = fg;
	tem->tvs_outbuf[tem->tvs_outindex].tc_bg_color = bg;
	tem->tvs_outindex++;
	tem->tvs_c_cursor.col++;
	if (tem->tvs_c_cursor.col >= tems.ts_c_dimension.width) {
		tem_safe_send_data(tem, credp, called_from);
		tem_safe_new_line(tem, credp, called_from);
	}
}

static void
tem_safe_new_line(struct tem_vt_state *tem,
    cred_t *credp, enum called_from called_from)
{
	tem_safe_cr(tem);
	tem_safe_lf(tem, credp, called_from);
}

static void
tem_safe_cr(struct tem_vt_state *tem)
{
	tem->tvs_c_cursor.col = 0;
	tem_safe_align_cursor(tem);
}

static void
tem_safe_lf(struct tem_vt_state *tem,
    cred_t *credp, enum called_from called_from)
{
	int row;

	ASSERT((called_from == CALLED_FROM_STANDALONE) ||
	    MUTEX_HELD(&tem->tvs_lock));

	/*
	 * Sanity checking notes:
	 * . a_nscroll was validated when it was set.
	 * . Regardless of that, tem_safe_scroll and tem_safe_mv_cursor
	 *   will prevent anything bad from happening.
	 */
	row = tem->tvs_c_cursor.row + 1;

	if (row >= tems.ts_c_dimension.height) {
		if (tem->tvs_nscroll != 0) {
			tem_safe_scroll(tem, 0,
			    tems.ts_c_dimension.height - 1,
			    tem->tvs_nscroll, TEM_SCROLL_UP,
			    credp, called_from);
			row = tems.ts_c_dimension.height -
			    tem->tvs_nscroll;
		} else {	/* no scroll */
			/*
			 * implement Esc[#r when # is zero.  This means no
			 * scroll but just return cursor to top of screen,
			 * do not clear screen.
			 */
			row = 0;
		}
	}

	tem_safe_mv_cursor(tem, row, tem->tvs_c_cursor.col,
	    credp, called_from);

	if (tem->tvs_nscroll == 0) {
		/* erase rest of cursor line */
		tem_safe_clear_chars(tem,
		    tems.ts_c_dimension.width -
		    tem->tvs_c_cursor.col,
		    tem->tvs_c_cursor.row,
		    tem->tvs_c_cursor.col,
		    credp, called_from);

	}

	tem_safe_align_cursor(tem);
}

static void
tem_safe_send_data(struct tem_vt_state *tem, cred_t *credp,
    enum called_from called_from)
{
	ASSERT((called_from == CALLED_FROM_STANDALONE) ||
	    MUTEX_HELD(&tem->tvs_lock));

	if (tem->tvs_outindex == 0) {
		tem_safe_align_cursor(tem);
		return;
	}

	tem_safe_virtual_display(tem,
	    tem->tvs_outbuf, tem->tvs_outindex,
	    tem->tvs_s_cursor.row, tem->tvs_s_cursor.col);

	if (tem->tvs_isactive) {
		/*
		 * Call the primitive to render this data.
		 */
		tem_safe_callback_display(tem,
		    tem->tvs_outbuf, tem->tvs_outindex,
		    tem->tvs_s_cursor.row, tem->tvs_s_cursor.col,
		    credp, called_from);
	}

	tem->tvs_outindex = 0;

	tem_safe_align_cursor(tem);
}


/*
 * We have just done something to the current output point.  Reset the start
 * point for the buffered data in a_outbuf.  There shouldn't be any data
 * buffered yet.
 */
static void
tem_safe_align_cursor(struct tem_vt_state *tem)
{
	tem->tvs_s_cursor.row = tem->tvs_c_cursor.row;
	tem->tvs_s_cursor.col = tem->tvs_c_cursor.col;
}

/*
 * State machine parser based on the current state and character input
 * major terminations are to control character or normal character
 */

static void
tem_safe_parse(struct tem_vt_state *tem, tem_char_t ch,
    cred_t *credp, enum called_from called_from)
{
	int	i;

	ASSERT((called_from == CALLED_FROM_STANDALONE) ||
	    MUTEX_HELD(&tem->tvs_lock));

	if (tem->tvs_state == A_STATE_START) {	/* Normal state? */
		if (ch == A_CSI || ch == A_ESC || ch < ' ') {
			/* Control */
			tem_safe_control(tem, ch, credp, called_from);
		} else {
			/* Display */
			tem_safe_outch(tem, ch, credp, called_from);
		}
		return;
	}

	/* In <ESC> sequence */
	if (tem->tvs_state != A_STATE_ESC) {	/* Need to get parameters? */
		if (tem->tvs_state != A_STATE_CSI) {
			tem_safe_getparams(tem, ch, credp, called_from);
			return;
		}

		switch (ch) {
		case '?':
			tem->tvs_state = A_STATE_CSI_QMARK;
			return;
		case '=':
			tem->tvs_state = A_STATE_CSI_EQUAL;
			return;
		case 's':
			/*
			 * As defined below, this sequence
			 * saves the cursor.  However, Sun
			 * defines ESC[s as reset.  We resolved
			 * the conflict by selecting reset as it
			 * is exported in the termcap file for
			 * sun-mon, while the "save cursor"
			 * definition does not exist anywhere in
			 * /etc/termcap.
			 * However, having no coherent
			 * definition of reset, we have not
			 * implemented it.
			 */

			/*
			 * Original code
			 * tem->tvs_r_cursor.row = tem->tvs_c_cursor.row;
			 * tem->tvs_r_cursor.col = tem->tvs_c_cursor.col;
			 * tem->tvs_state = A_STATE_START;
			 */

			tem->tvs_state = A_STATE_START;
			return;
		case 'u':
			tem_safe_mv_cursor(tem, tem->tvs_r_cursor.row,
			    tem->tvs_r_cursor.col, credp, called_from);
			tem->tvs_state = A_STATE_START;
			return;
		case 'p':	/* sunbow */
			tem_safe_send_data(tem, credp, called_from);
			/*
			 * Don't set anything if we are
			 * already as we want to be.
			 */
			if (tem->tvs_flags & TEM_ATTR_SCREEN_REVERSE) {
				tem->tvs_flags &= ~TEM_ATTR_SCREEN_REVERSE;
				/*
				 * If we have switched the characters to be the
				 * inverse from the screen, then switch them as
				 * well to keep them the inverse of the screen.
				 */
				if (tem->tvs_flags & TEM_ATTR_REVERSE)
					tem->tvs_flags &= ~TEM_ATTR_REVERSE;
				else
					tem->tvs_flags |= TEM_ATTR_REVERSE;
			}
			tem_safe_cls(tem, credp, called_from);
			tem->tvs_state = A_STATE_START;
			return;
		case 'q':	/* sunwob */
			tem_safe_send_data(tem, credp, called_from);
			/*
			 * Don't set anything if we are
			 * already where as we want to be.
			 */
			if (!(tem->tvs_flags & TEM_ATTR_SCREEN_REVERSE)) {
				tem->tvs_flags |= TEM_ATTR_SCREEN_REVERSE;
				/*
				 * If we have switched the characters to be the
				 * inverse from the screen, then switch them as
				 * well to keep them the inverse of the screen.
				 */
				if (!(tem->tvs_flags & TEM_ATTR_REVERSE))
					tem->tvs_flags |= TEM_ATTR_REVERSE;
				else
					tem->tvs_flags &= ~TEM_ATTR_REVERSE;
			}

			tem_safe_cls(tem, credp, called_from);
			tem->tvs_state = A_STATE_START;
			return;
		case 'r':	/* sunscrl */
			/*
			 * Rule exception:  check for validity here.
			 */
			tem->tvs_nscroll = tem->tvs_paramval;
			if (tem->tvs_nscroll > tems.ts_c_dimension.height)
				tem->tvs_nscroll = tems.ts_c_dimension.height;
			if (tem->tvs_nscroll < 0)
				tem->tvs_nscroll = 1;
			tem->tvs_state = A_STATE_START;
			return;
		default:
			tem_safe_getparams(tem, ch, credp, called_from);
			return;
		}
	}

	/* Previous char was <ESC> */
	if (ch == '[') {
		tem->tvs_curparam = 0;
		tem->tvs_paramval = 0;
		tem->tvs_gotparam = B_FALSE;
		/* clear the parameters */
		for (i = 0; i < TEM_MAXPARAMS; i++)
			tem->tvs_params[i] = -1;
		tem->tvs_state = A_STATE_CSI;
	} else if (ch == 'Q') {	/* <ESC>Q ? */
		tem->tvs_state = A_STATE_START;
	} else if (ch == 'C') {	/* <ESC>C ? */
		tem->tvs_state = A_STATE_START;
	} else {
		tem->tvs_state = A_STATE_START;
		if (ch == 'c') {
			/* ESC c resets display */
			tem_safe_reset_display(tem, credp, called_from,
			    B_TRUE, B_TRUE);
		} else if (ch == 'H') {
			/* ESC H sets a tab */
			tem_safe_set_tab(tem);
		} else if (ch == '7') {
			/* ESC 7 Save Cursor position */
			tem->tvs_r_cursor.row = tem->tvs_c_cursor.row;
			tem->tvs_r_cursor.col = tem->tvs_c_cursor.col;
		} else if (ch == '8') {
			/* ESC 8 Restore Cursor position */
			tem_safe_mv_cursor(tem, tem->tvs_r_cursor.row,
			    tem->tvs_r_cursor.col, credp, called_from);
		/* check for control chars */
		} else if (ch < ' ') {
			tem_safe_control(tem, ch, credp, called_from);
		} else {
			tem_safe_outch(tem, ch, credp, called_from);
		}
	}
}

/* ARGSUSED */
static void
tem_safe_bell(struct tem_vt_state *tem, enum called_from called_from)
{
	if (called_from == CALLED_FROM_STANDALONE)
		(void) beep_polled(BEEP_CONSOLE);
	else
		(void) beep(BEEP_CONSOLE);
}


static void
tem_safe_scroll(struct tem_vt_state *tem, int start, int end, int count,
    int direction, cred_t *credp, enum called_from called_from)
{
	int	row;
	int	lines_affected;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	lines_affected = end - start + 1;
	if (count > lines_affected)
		count = lines_affected;
	if (count <= 0)
		return;

	switch (direction) {
	case TEM_SCROLL_UP:
		if (count < lines_affected) {
			tem_safe_copy_area(tem, 0, start + count,
			    tems.ts_c_dimension.width - 1, end,
			    0, start, credp, called_from);
		}
		for (row = (end - count) + 1; row <= end; row++) {
			tem_safe_clear_chars(tem, tems.ts_c_dimension.width,
			    row, 0, credp, called_from);
		}
		break;

	case TEM_SCROLL_DOWN:
		if (count < lines_affected) {
			tem_safe_copy_area(tem, 0, start,
			    tems.ts_c_dimension.width - 1,
			    end - count, 0, start + count,
			    credp, called_from);
		}
		for (row = start; row < start + count; row++) {
			tem_safe_clear_chars(tem, tems.ts_c_dimension.width,
			    row, 0, credp, called_from);
		}
		break;
	}
}

static int
tem_copy_width(term_char_t *src, term_char_t *dst, int cols)
{
	int width = cols - 1;

	while (width >= 0) {
		/* We can't compare images. */
		if (TEM_CHAR_ATTR(src[width].tc_char) == TEM_ATTR_IMAGE ||
		    TEM_CHAR_ATTR(dst[width].tc_char) == TEM_ATTR_IMAGE)
			break;

		/*
		 * Find difference on line, compare char with its attributes
		 * and colors.
		 */
		if (src[width].tc_char != dst[width].tc_char ||
		    src[width].tc_fg_color != dst[width].tc_fg_color ||
		    src[width].tc_bg_color != dst[width].tc_bg_color) {
			break;
		}
		width--;
	}
	return (width + 1);
}

static void
tem_safe_copy_area(struct tem_vt_state *tem,
    screen_pos_t s_col, screen_pos_t s_row,
    screen_pos_t e_col, screen_pos_t e_row,
    screen_pos_t t_col, screen_pos_t t_row,
    cred_t *credp, enum called_from called_from)
{
	size_t soffset, toffset;
	term_char_t *src, *dst;
	int rows;
	int cols;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	if (s_col < 0 || s_row < 0 ||
	    e_col < 0 || e_row < 0 ||
	    t_col < 0 || t_row < 0 ||
	    s_col >= tems.ts_c_dimension.width ||
	    e_col >= tems.ts_c_dimension.width ||
	    t_col >= tems.ts_c_dimension.width ||
	    s_row >= tems.ts_c_dimension.height ||
	    e_row >= tems.ts_c_dimension.height ||
	    t_row >= tems.ts_c_dimension.height)
		return;

	if (s_row > e_row || s_col > e_col)
		return;

	rows = e_row - s_row + 1;
	cols = e_col - s_col + 1;
	if (t_row + rows > tems.ts_c_dimension.height ||
	    t_col + cols > tems.ts_c_dimension.width)
		return;

	soffset = s_col + s_row * tems.ts_c_dimension.width;
	toffset = t_col + t_row * tems.ts_c_dimension.width;
	src = tem->tvs_screen_buf + soffset;
	dst = tem->tvs_screen_buf + toffset;

	/*
	 * Copy line by line. We determine the length by comparing the
	 * screen content from cached text in tvs_screen_buf.
	 */
	if (toffset <= soffset) {
		for (int i = 0; i < rows; i++) {
			int increment = i * tems.ts_c_dimension.width;
			int width;

			width = tem_copy_width(src + increment,
			    dst + increment, cols);

			tem_safe_virtual_copy(tem, s_col, s_row + i,
			    e_col  - cols + width, s_row + i,
			    t_col, t_row + i);

			if (tem->tvs_isactive) {
				tem_safe_callback_copy(tem, s_col, s_row + i,
				    e_col - cols + width, s_row + i,
				    t_col, t_row + i, credp, called_from);
			}
		}
	} else {
		for (int i = rows - 1; i >= 0; i--) {
			int increment = i * tems.ts_c_dimension.width;
			int width;

			width = tem_copy_width(src + increment,
			    dst + increment, cols);

			tem_safe_virtual_copy(tem, s_col, s_row + i,
			    e_col  - cols + width, s_row + i,
			    t_col, t_row + i);

			if (tem->tvs_isactive) {
				tem_safe_callback_copy(tem, s_col, s_row + i,
				    e_col - cols + width, s_row + i,
				    t_col, t_row + i, credp, called_from);
			}
		}
	}
}

static void
tem_safe_clear_chars(struct tem_vt_state *tem, int count, screen_pos_t row,
    screen_pos_t col, cred_t *credp, enum called_from called_from)
{
	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	if (row < 0 || row >= tems.ts_c_dimension.height ||
	    col < 0 || col >= tems.ts_c_dimension.width ||
	    count < 0)
		return;

	/*
	 * Note that very large values of "count" could cause col+count
	 * to overflow, so we check "count" independently.
	 */
	if (count > tems.ts_c_dimension.width ||
	    col + count > tems.ts_c_dimension.width)
		count = tems.ts_c_dimension.width - col;

	tem_safe_virtual_cls(tem, count, row, col);

	if (!tem->tvs_isactive)
		return;

	tem_safe_callback_cls(tem, count, row, col, credp, called_from);
}

/*ARGSUSED*/
void
tem_safe_text_display(struct tem_vt_state *tem, term_char_t *string,
    int count, screen_pos_t row, screen_pos_t col,
    cred_t *credp, enum called_from called_from)
{
	struct vis_consdisplay da;
	int i;
	tem_char_t c;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	da.data = (uint8_t *)&c;
	da.width = 1;
	da.row = row;
	da.col = col;

	for (i = 0; i < count; i++) {
		tem_safe_get_color(&da.fg_color, &da.bg_color, string[i]);
		c = TEM_CHAR(string[i].tc_char);
		tems_safe_display(&da, credp, called_from);
		da.col++;
	}
}

/*
 * This function is used to blit a rectangular color image,
 * unperturbed on the underlying framebuffer, to render
 * icons and pictures.  The data is a pixel pattern that
 * fills a rectangle bounded to the width and height parameters.
 * The color pixel data must to be pre-adjusted by the caller
 * for the current video depth.
 *
 * This function is unused now.
 */
/*ARGSUSED*/
static void
tem_safe_image_display(struct tem_vt_state *tem, uchar_t *image,
    int height, int width, screen_pos_t row, screen_pos_t col,
    cred_t *credp, enum called_from called_from)
{
	struct vis_consdisplay da;

	mutex_enter(&tems.ts_lock);
	mutex_enter(&tem->tvs_lock);

	da.data = image;
	da.width = (screen_size_t)width;
	da.height = (screen_size_t)height;
	da.row = row;
	da.col = col;

	tems_safe_display(&da, credp, called_from);

	mutex_exit(&tem->tvs_lock);
	mutex_exit(&tems.ts_lock);
}

/*ARGSUSED*/
void
tem_safe_text_copy(struct tem_vt_state *tem,
    screen_pos_t s_col, screen_pos_t s_row,
    screen_pos_t e_col, screen_pos_t e_row,
    screen_pos_t t_col, screen_pos_t t_row,
    cred_t *credp, enum called_from called_from)
{
	struct vis_conscopy da;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	da.s_row = s_row;
	da.s_col = s_col;
	da.e_row = e_row;
	da.e_col = e_col;
	da.t_row = t_row;
	da.t_col = t_col;

	tems_safe_copy(&da, credp, called_from);
}

void
tem_safe_text_cls(struct tem_vt_state *tem,
    int count, screen_pos_t row, screen_pos_t col, cred_t *credp,
    enum called_from called_from)
{
	text_attr_t attr;
	term_char_t c;
	int i;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	tem_safe_get_attr(tem, &c.tc_fg_color, &c.tc_bg_color, &attr,
	    TEM_ATTR_SCREEN_REVERSE);
	c.tc_char = TEM_ATTR(attr & ~TEM_ATTR_UNDERLINE) | ' ';

	if (count > tems.ts_c_dimension.width ||
	    col + count > tems.ts_c_dimension.width)
		count = tems.ts_c_dimension.width - col;

	for (i = 0; i < count; i++)
		tems.ts_blank_line[i] = c;

	tem_safe_text_display(tem, tems.ts_blank_line, count, row, col,
	    credp, called_from);
}

void
tem_safe_pix_display(struct tem_vt_state *tem,
    term_char_t *string, int count,
    screen_pos_t row, screen_pos_t col,
    cred_t *credp, enum called_from called_from)
{
	struct vis_consdisplay da;
	int	i;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	da.data = (uchar_t *)tem->tvs_pix_data;
	da.width = (screen_size_t)tems.ts_font.vf_width;
	da.height = (screen_size_t)tems.ts_font.vf_height;
	da.row = (row * da.height) + tems.ts_p_offset.y;
	da.col = (col * da.width) + tems.ts_p_offset.x;

	for (i = 0; i < count; i++) {
		/* Do not display image area */
		if (!TEM_ATTR_ISSET(string[i].tc_char, TEM_ATTR_IMAGE)) {
			tem_safe_callback_bit2pix(tem, string[i]);
			tems_safe_display(&da, credp, called_from);
		}
		da.col += da.width;
	}
}

void
tem_safe_pix_copy(struct tem_vt_state *tem,
    screen_pos_t s_col, screen_pos_t s_row,
    screen_pos_t e_col, screen_pos_t e_row,
    screen_pos_t t_col, screen_pos_t t_row,
    cred_t *credp,
    enum called_from called_from)
{
	struct vis_conscopy ma;
	static boolean_t need_clear = B_TRUE;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	if (need_clear && tem->tvs_first_line > 0) {
		/*
		 * Clear OBP output above our kernel console term
		 * when our kernel console term begins to scroll up,
		 * we hope it is user friendly.
		 * (Also see comments on tem_safe_pix_clear_prom_output)
		 *
		 * This is only one time call.
		 */
		tem_safe_pix_clear_prom_output(tem, credp, called_from);
	}
	need_clear = B_FALSE;

	ma.s_row = s_row * tems.ts_font.vf_height + tems.ts_p_offset.y;
	ma.e_row = (e_row + 1) * tems.ts_font.vf_height +
	    tems.ts_p_offset.y - 1;
	ma.t_row = t_row * tems.ts_font.vf_height + tems.ts_p_offset.y;

	/*
	 * Check if we're in process of clearing OBP's columns area,
	 * which only happens when term scrolls up a whole line.
	 */
	if (tem->tvs_first_line > 0 && t_row < s_row && t_col == 0 &&
	    e_col == tems.ts_c_dimension.width - 1) {
		/*
		 * We need to clear OBP's columns area outside our kernel
		 * console term. So that we set ma.e_col to entire row here.
		 */
		ma.s_col = s_col * tems.ts_font.vf_width;
		ma.e_col = tems.ts_p_dimension.width - 1;

		ma.t_col = t_col * tems.ts_font.vf_width;
	} else {
		ma.s_col = s_col * tems.ts_font.vf_width + tems.ts_p_offset.x;
		ma.e_col = (e_col + 1) * tems.ts_font.vf_width +
		    tems.ts_p_offset.x - 1;
		ma.t_col = t_col * tems.ts_font.vf_width + tems.ts_p_offset.x;
	}

	tems_safe_copy(&ma, credp, called_from);

	if (tem->tvs_first_line > 0 && t_row < s_row) {
		/* We have scrolled up (s_row - t_row) rows. */
		tem->tvs_first_line -= (s_row - t_row);
		if (tem->tvs_first_line <= 0) {
			/* All OBP rows have been cleared. */
			tem->tvs_first_line = 0;
		}
	}

}

void
tem_safe_pix_bit2pix(struct tem_vt_state *tem, term_char_t c)
{
	text_color_t fg, bg;
	void (*fp)(struct tem_vt_state *, tem_char_t,
	    unsigned char, unsigned char);

	tem_safe_get_color(&fg, &bg, c);
	switch (tems.ts_pdepth) {
	case 4:
		fp = bit_to_pix4;
		break;
	case 8:
		fp = bit_to_pix8;
		break;
	case 15:
	case 16:
		fp = bit_to_pix16;
		break;
	case 24:
		fp = bit_to_pix24;
		break;
	case 32:
		fp = bit_to_pix32;
		break;
	default:
		return;
	}

	fp(tem, c.tc_char, fg, bg);
}


/*
 * This function only clears count of columns in one row
 */
void
tem_safe_pix_cls(struct tem_vt_state *tem, int count,
    screen_pos_t row, screen_pos_t col, cred_t *credp,
    enum called_from called_from)
{
	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	tem_safe_pix_cls_range(tem, row, 1, tems.ts_p_offset.y,
	    col, count, tems.ts_p_offset.x, B_FALSE, credp, called_from);
}

/*
 * This function clears OBP output above our kernel console term area
 * because OBP's term may have a bigger terminal window than that of
 * our kernel console term. So we need to clear OBP output garbage outside
 * of our kernel console term at a proper time, which is when the first
 * row output of our kernel console term scrolls at the first screen line.
 *
 *	_________________________________
 *	|   _____________________	|  ---> OBP's bigger term window
 *	|   |			|	|
 *	|___|			|	|
 *	| | |			|	|
 *	| | |			|	|
 *	|_|_|___________________|_______|
 *	  | |			|	   ---> first line
 *	  | |___________________|---> our kernel console term window
 *	  |
 *	  |---> columns area to be cleared
 *
 * This function only takes care of the output above our kernel console term,
 * and tem_prom_scroll_up takes care of columns area outside of our kernel
 * console term.
 */
static void
tem_safe_pix_clear_prom_output(struct tem_vt_state *tem, cred_t *credp,
    enum called_from called_from)
{
	int	nrows, ncols, width, height, offset;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	width = tems.ts_font.vf_width;
	height = tems.ts_font.vf_height;
	offset = tems.ts_p_offset.y % height;

	nrows = tems.ts_p_offset.y / height;
	ncols = (tems.ts_p_dimension.width + (width - 1))/ width;

	if (nrows > 0)
		tem_safe_pix_cls_range(tem, 0, nrows, offset, 0, ncols, 0,
		    B_FALSE, credp, called_from);
}

/*
 * clear the whole screen for pixel mode, just clear the
 * physical screen.
 */
void
tem_safe_pix_clear_entire_screen(struct tem_vt_state *tem, cred_t *credp,
    enum called_from called_from)
{
	struct vis_consclear cl;
	text_color_t fg_color;
	text_color_t bg_color;
	text_attr_t attr;
	term_char_t c;
	int nrows, ncols, width, height;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	/* call driver first, if error, clear terminal area */
	tem_safe_get_attr(tem, &c.tc_fg_color, &c.tc_bg_color, &attr,
	    TEM_ATTR_SCREEN_REVERSE);
	c.tc_char = TEM_ATTR(attr);

	tem_safe_get_color(&fg_color, &bg_color, c);
	cl.bg_color = bg_color;
	if (tems_cls_layered(&cl, credp) == 0)
		return;

	width = tems.ts_font.vf_width;
	height = tems.ts_font.vf_height;

	nrows = (tems.ts_p_dimension.height + (height - 1))/ height;
	ncols = (tems.ts_p_dimension.width + (width - 1))/ width;

	tem_safe_pix_cls_range(tem, 0, nrows, tems.ts_p_offset.y, 0, ncols,
	    tems.ts_p_offset.x, B_FALSE, credp, called_from);

	/*
	 * Since the whole screen is cleared, we don't need
	 * to clear OBP output later.
	 */
	if (tem->tvs_first_line > 0)
		tem->tvs_first_line = 0;
}

/*
 * clear the whole screen, including the virtual screen buffer,
 * and reset the cursor to start point.
 */
static void
tem_safe_cls(struct tem_vt_state *tem,
    cred_t *credp, enum called_from called_from)
{
	int	row;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	if (tems.ts_display_mode == VIS_TEXT) {
		for (row = 0; row < tems.ts_c_dimension.height; row++) {
			tem_safe_clear_chars(tem, tems.ts_c_dimension.width,
			    row, 0, credp, called_from);
		}
		tem->tvs_c_cursor.row = 0;
		tem->tvs_c_cursor.col = 0;
		tem_safe_align_cursor(tem);
		return;
	}

	ASSERT(tems.ts_display_mode == VIS_PIXEL);

	for (row = 0; row < tems.ts_c_dimension.height; row++) {
		tem_safe_virtual_cls(tem, tems.ts_c_dimension.width, row, 0);
	}
	tem->tvs_c_cursor.row = 0;
	tem->tvs_c_cursor.col = 0;
	tem_safe_align_cursor(tem);

	if (!tem->tvs_isactive)
		return;

	tem_safe_pix_clear_entire_screen(tem, credp, called_from);
}

static void
tem_safe_back_tab(struct tem_vt_state *tem,
    cred_t *credp, enum called_from called_from)
{
	int	i;
	screen_pos_t	tabstop;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	tabstop = 0;

	for (i = tem->tvs_ntabs - 1; i >= 0; i--) {
		if (tem->tvs_tabs[i] < tem->tvs_c_cursor.col) {
			tabstop = tem->tvs_tabs[i];
			break;
		}
	}

	tem_safe_mv_cursor(tem, tem->tvs_c_cursor.row,
	    tabstop, credp, called_from);
}

static void
tem_safe_tab(struct tem_vt_state *tem,
    cred_t *credp, enum called_from called_from)
{
	int	i;
	screen_pos_t	tabstop;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	tabstop = tems.ts_c_dimension.width - 1;

	for (i = 0; i < tem->tvs_ntabs; i++) {
		if (tem->tvs_tabs[i] > tem->tvs_c_cursor.col) {
			tabstop = tem->tvs_tabs[i];
			break;
		}
	}

	tem_safe_mv_cursor(tem, tem->tvs_c_cursor.row,
	    tabstop, credp, called_from);
}

static void
tem_safe_set_tab(struct tem_vt_state *tem)
{
	int	i;
	int	j;

	if (tem->tvs_ntabs == TEM_MAXTAB)
		return;
	if (tem->tvs_ntabs == 0 ||
	    tem->tvs_tabs[tem->tvs_ntabs] < tem->tvs_c_cursor.col) {
			tem->tvs_tabs[tem->tvs_ntabs++] = tem->tvs_c_cursor.col;
			return;
	}
	for (i = 0; i < tem->tvs_ntabs; i++) {
		if (tem->tvs_tabs[i] == tem->tvs_c_cursor.col)
			return;
		if (tem->tvs_tabs[i] > tem->tvs_c_cursor.col) {
			for (j = tem->tvs_ntabs - 1; j >= i; j--)
				tem->tvs_tabs[j+ 1] = tem->tvs_tabs[j];
			tem->tvs_tabs[i] = tem->tvs_c_cursor.col;
			tem->tvs_ntabs++;
			return;
		}
	}
}

static void
tem_safe_clear_tabs(struct tem_vt_state *tem, int action)
{
	int	i;
	int	j;

	switch (action) {
	case 3: /* clear all tabs */
		tem->tvs_ntabs = 0;
		break;
	case 0: /* clr tab at cursor */

		for (i = 0; i < tem->tvs_ntabs; i++) {
			if (tem->tvs_tabs[i] == tem->tvs_c_cursor.col) {
				tem->tvs_ntabs--;
				for (j = i; j < tem->tvs_ntabs; j++)
					tem->tvs_tabs[j] = tem->tvs_tabs[j + 1];
				return;
			}
		}
		break;
	}
}

static void
tem_safe_mv_cursor(struct tem_vt_state *tem, int row, int col,
    cred_t *credp, enum called_from called_from)
{
	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	/*
	 * Sanity check and bounds enforcement.  Out of bounds requests are
	 * clipped to the screen boundaries.  This seems to be what SPARC
	 * does.
	 */
	if (row < 0)
		row = 0;
	if (row >= tems.ts_c_dimension.height)
		row = tems.ts_c_dimension.height - 1;
	if (col < 0)
		col = 0;
	if (col >= tems.ts_c_dimension.width)
		col = tems.ts_c_dimension.width - 1;

	tem_safe_send_data(tem, credp, called_from);
	tem->tvs_c_cursor.row = (screen_pos_t)row;
	tem->tvs_c_cursor.col = (screen_pos_t)col;
	tem_safe_align_cursor(tem);
}

/* ARGSUSED */
void
tem_safe_reset_emulator(struct tem_vt_state *tem,
    cred_t *credp, enum called_from called_from,
    boolean_t init_color)
{
	int j;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	tem->tvs_c_cursor.row = 0;
	tem->tvs_c_cursor.col = 0;
	tem->tvs_r_cursor.row = 0;
	tem->tvs_r_cursor.col = 0;
	tem->tvs_s_cursor.row = 0;
	tem->tvs_s_cursor.col = 0;
	tem->tvs_outindex = 0;
	tem->tvs_state = A_STATE_START;
	tem->tvs_gotparam = B_FALSE;
	tem->tvs_curparam = 0;
	tem->tvs_paramval = 0;
	tem->tvs_nscroll = 1;

	if (init_color) {
		/* use initial settings */
		tem->tvs_fg_color = tems.ts_init_color.fg_color;
		tem->tvs_bg_color = tems.ts_init_color.bg_color;
		tem->tvs_flags = tems.ts_init_color.a_flags;
	}

	/*
	 * set up the initial tab stops
	 */
	tem->tvs_ntabs = 0;
	for (j = 8; j < tems.ts_c_dimension.width; j += 8)
		tem->tvs_tabs[tem->tvs_ntabs++] = (screen_pos_t)j;

	for (j = 0; j < TEM_MAXPARAMS; j++)
		tem->tvs_params[j] = 0;
}

void
tem_safe_reset_display(struct tem_vt_state *tem,
    cred_t *credp, enum called_from called_from,
    boolean_t clear_txt, boolean_t init_color)
{
	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	tem_safe_reset_emulator(tem, credp, called_from, init_color);

	if (clear_txt) {
		if (tem->tvs_isactive)
			tem_safe_callback_cursor(tem,
			    VIS_HIDE_CURSOR, credp, called_from);

		tem_safe_cls(tem, credp, called_from);

		if (tem->tvs_isactive)
			tem_safe_callback_cursor(tem,
			    VIS_DISPLAY_CURSOR, credp, called_from);
	}
}

static void
tem_safe_shift(
	struct tem_vt_state *tem,
	int count,
	int direction,
	cred_t *credp,
	enum called_from called_from)
{
	int rest_of_line;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	rest_of_line = tems.ts_c_dimension.width - tem->tvs_c_cursor.col;
	if (count > rest_of_line)
		count = rest_of_line;

	if (count <= 0)
		return;

	switch (direction) {
	case TEM_SHIFT_LEFT:
		if (count < rest_of_line) {
			tem_safe_copy_area(tem,
			    tem->tvs_c_cursor.col + count,
			    tem->tvs_c_cursor.row,
			    tems.ts_c_dimension.width - 1,
			    tem->tvs_c_cursor.row,
			    tem->tvs_c_cursor.col,
			    tem->tvs_c_cursor.row,
			    credp, called_from);
		}

		tem_safe_clear_chars(tem, count, tem->tvs_c_cursor.row,
		    (tems.ts_c_dimension.width - count), credp,
		    called_from);
		break;
	case TEM_SHIFT_RIGHT:
		if (count < rest_of_line) {
			tem_safe_copy_area(tem,
			    tem->tvs_c_cursor.col,
			    tem->tvs_c_cursor.row,
			    tems.ts_c_dimension.width - count - 1,
			    tem->tvs_c_cursor.row,
			    tem->tvs_c_cursor.col + count,
			    tem->tvs_c_cursor.row,
			    credp, called_from);
		}

		tem_safe_clear_chars(tem, count, tem->tvs_c_cursor.row,
		    tem->tvs_c_cursor.col, credp, called_from);
		break;
	}
}

void
tem_safe_text_cursor(struct tem_vt_state *tem, short action,
    cred_t *credp, enum called_from called_from)
{
	struct vis_conscursor	ca;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	ca.row = tem->tvs_c_cursor.row;
	ca.col = tem->tvs_c_cursor.col;
	ca.action = action;

	tems_safe_cursor(&ca, credp, called_from);

	if (action == VIS_GET_CURSOR) {
		tem->tvs_c_cursor.row = ca.row;
		tem->tvs_c_cursor.col = ca.col;
	}
}

void
tem_safe_pix_cursor(struct tem_vt_state *tem, short action,
    cred_t *credp, enum called_from called_from)
{
	struct vis_conscursor	ca;
	uint32_t color;
	text_color_t fg, bg;
	term_char_t c;
	text_attr_t attr;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	ca.row = tem->tvs_c_cursor.row * tems.ts_font.vf_height +
	    tems.ts_p_offset.y;
	ca.col = tem->tvs_c_cursor.col * tems.ts_font.vf_width +
	    tems.ts_p_offset.x;
	ca.width = (screen_size_t)tems.ts_font.vf_width;
	ca.height = (screen_size_t)tems.ts_font.vf_height;

	tem_safe_get_attr(tem, &c.tc_fg_color, &c.tc_bg_color, &attr,
	    TEM_ATTR_REVERSE);
	c.tc_char = TEM_ATTR(attr);

	tem_safe_get_color(&fg, &bg, c);

	switch (tems.ts_pdepth) {
	case 4:
		ca.fg_color.mono = fg;
		ca.bg_color.mono = bg;
		break;
	case 8:
#ifdef _HAVE_TEM_FIRMWARE
		ca.fg_color.mono = fg;
		ca.bg_color.mono = bg;
#else
		ca.fg_color.mono = tems.ts_color_map(fg);
		ca.bg_color.mono = tems.ts_color_map(bg);
#endif
		break;
	case 15:
	case 16:
		color = tems.ts_color_map(fg);
		ca.fg_color.sixteen[0] = (color >> 8) & 0xFF;
		ca.fg_color.sixteen[1] = color & 0xFF;
		color = tems.ts_color_map(bg);
		ca.bg_color.sixteen[0] = (color >> 8) & 0xFF;
		ca.bg_color.sixteen[1] = color & 0xFF;
		break;
	case 24:
	case 32:
#ifdef _HAVE_TEM_FIRMWARE
		/* Keeping this block to support old binary only drivers */
		if (tem->tvs_flags & TEM_ATTR_REVERSE) {
			ca.fg_color.twentyfour[0] = TEM_TEXT_WHITE24_RED;
			ca.fg_color.twentyfour[1] = TEM_TEXT_WHITE24_GREEN;
			ca.fg_color.twentyfour[2] = TEM_TEXT_WHITE24_BLUE;

			ca.bg_color.twentyfour[0] = TEM_TEXT_BLACK24_RED;
			ca.bg_color.twentyfour[1] = TEM_TEXT_BLACK24_GREEN;
			ca.bg_color.twentyfour[2] = TEM_TEXT_BLACK24_BLUE;
		} else {
			ca.fg_color.twentyfour[0] = TEM_TEXT_BLACK24_RED;
			ca.fg_color.twentyfour[1] = TEM_TEXT_BLACK24_GREEN;
			ca.fg_color.twentyfour[2] = TEM_TEXT_BLACK24_BLUE;

			ca.bg_color.twentyfour[0] = TEM_TEXT_WHITE24_RED;
			ca.bg_color.twentyfour[1] = TEM_TEXT_WHITE24_GREEN;
			ca.bg_color.twentyfour[2] = TEM_TEXT_WHITE24_BLUE;
		}
#else
		color = tems.ts_color_map(fg);
		ca.fg_color.twentyfour[0] = (color >> 16) & 0xFF;
		ca.fg_color.twentyfour[1] = (color >> 8) & 0xFF;
		ca.fg_color.twentyfour[2] = color & 0xFF;
		color = tems.ts_color_map(bg);
		ca.bg_color.twentyfour[0] = (color >> 16) & 0xFF;
		ca.bg_color.twentyfour[1] = (color >> 8) & 0xFF;
		ca.bg_color.twentyfour[2] = color & 0xFF;
#endif
		break;
	}

	ca.action = action;

	tems_safe_cursor(&ca, credp, called_from);

	if (action == VIS_GET_CURSOR) {
		tem->tvs_c_cursor.row = 0;
		tem->tvs_c_cursor.col = 0;

		if (ca.row != 0) {
			tem->tvs_c_cursor.row = (ca.row - tems.ts_p_offset.y) /
			    tems.ts_font.vf_height;
		}
		if (ca.col != 0) {
			tem->tvs_c_cursor.col = (ca.col - tems.ts_p_offset.x) /
			    tems.ts_font.vf_width;
		}
	}
}

static void
bit_to_pix4(struct tem_vt_state *tem, tem_char_t c, text_color_t fg_color,
    text_color_t bg_color)
{
	uint8_t *dest = (uint8_t *)tem->tvs_pix_data;
	font_bit_to_pix4(&tems.ts_font, dest, c, fg_color, bg_color);
}

static void
bit_to_pix8(struct tem_vt_state *tem, tem_char_t c, text_color_t fg_color,
    text_color_t bg_color)
{
	uint8_t *dest = (uint8_t *)tem->tvs_pix_data;

#ifndef _HAVE_TEM_FIRMWARE
	fg_color = (text_color_t)tems.ts_color_map(fg_color);
	bg_color = (text_color_t)tems.ts_color_map(bg_color);
#endif
	font_bit_to_pix8(&tems.ts_font, dest, c, fg_color, bg_color);
}

static void
bit_to_pix16(struct tem_vt_state *tem, tem_char_t c, text_color_t fg_color4,
    text_color_t bg_color4)
{
	uint16_t fg_color16, bg_color16;
	uint16_t *dest;

	ASSERT(fg_color4 < 16 && bg_color4 < 16);

	fg_color16 = (uint16_t)tems.ts_color_map(fg_color4);
	bg_color16 = (uint16_t)tems.ts_color_map(bg_color4);

	dest = (uint16_t *)tem->tvs_pix_data;
	font_bit_to_pix16(&tems.ts_font, dest, c, fg_color16, bg_color16);
}

static void
bit_to_pix24(struct tem_vt_state *tem, tem_char_t c, text_color_t fg_color4,
    text_color_t bg_color4)
{
	uint32_t fg_color32, bg_color32;
	uint8_t *dest;

#ifdef _HAVE_TEM_FIRMWARE
	fg_color32 = PIX4TO32(fg_color4);
	bg_color32 = PIX4TO32(bg_color4);
#else
	fg_color32 = tems.ts_color_map(fg_color4);
	bg_color32 = tems.ts_color_map(bg_color4);
#endif

	dest = (uint8_t *)tem->tvs_pix_data;
	font_bit_to_pix24(&tems.ts_font, dest, c, fg_color32, bg_color32);
}

static void
bit_to_pix32(struct tem_vt_state *tem, tem_char_t c, text_color_t fg_color4,
    text_color_t bg_color4)
{
	uint32_t fg_color32, bg_color32, *dest;

#ifdef _HAVE_TEM_FIRMWARE
	fg_color32 = PIX4TO32(fg_color4);
	bg_color32 = PIX4TO32(bg_color4);
#else
	fg_color32 = ((uint32_t)0xFF << 24) | tems.ts_color_map(fg_color4);
	bg_color32 = ((uint32_t)0xFF << 24) | tems.ts_color_map(bg_color4);
#endif

	dest = (uint32_t *)tem->tvs_pix_data;
	font_bit_to_pix32(&tems.ts_font, dest, c, fg_color32, bg_color32);
}

/*
 * flag: TEM_ATTR_SCREEN_REVERSE or TEM_ATTR_REVERSE
 */
void
tem_safe_get_attr(struct tem_vt_state *tem, text_color_t *fg,
    text_color_t *bg, text_attr_t *attr, uint8_t flag)
{
	if (tem->tvs_flags & flag) {
		*fg = tem->tvs_bg_color;
		*bg = tem->tvs_fg_color;
	} else {
		*fg = tem->tvs_fg_color;
		*bg = tem->tvs_bg_color;
	}

	if (attr == NULL)
		return;

	*attr = tem->tvs_flags;
}

static void
tem_safe_get_color(text_color_t *fg, text_color_t *bg, term_char_t c)
{
	*fg = c.tc_fg_color;
	*bg = c.tc_bg_color;

	if (c.tc_fg_color < XLATE_NCOLORS) {
		if (TEM_ATTR_ISSET(c.tc_char,
		    TEM_ATTR_BRIGHT_FG | TEM_ATTR_BOLD))
			*fg = brt_xlate[c.tc_fg_color];
		else
			*fg = dim_xlate[c.tc_fg_color];
	}

	if (c.tc_bg_color < XLATE_NCOLORS) {
		if (TEM_ATTR_ISSET(c.tc_char, TEM_ATTR_BRIGHT_BG))
			*bg = brt_xlate[c.tc_bg_color];
		else
			*bg = dim_xlate[c.tc_bg_color];
	}
}

/*
 * Clear a rectangle of screen for pixel mode.
 *
 * arguments:
 *    row:	start row#
 *    nrows:	the number of rows to clear
 *    offset_y:	the offset of height in pixels to begin clear
 *    col:	start col#
 *    ncols:	the number of cols to clear
 *    offset_x:	the offset of width in pixels to begin clear
 *    scroll_up: whether this function is called during sroll up,
 *		 which is called only once.
 */
void
tem_safe_pix_cls_range(struct tem_vt_state *tem,
    screen_pos_t row, int nrows, int offset_y,
    screen_pos_t col, int ncols, int offset_x,
    boolean_t sroll_up, cred_t *credp,
    enum called_from called_from)
{
	struct vis_consdisplay da;
	int	i, j;
	int	row_add = 0;
	term_char_t c;
	text_attr_t attr;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	if (sroll_up)
		row_add = tems.ts_c_dimension.height - 1;

	da.width = (screen_size_t)tems.ts_font.vf_width;
	da.height = (screen_size_t)tems.ts_font.vf_height;

	tem_safe_get_attr(tem, &c.tc_fg_color, &c.tc_bg_color, &attr,
	    TEM_ATTR_SCREEN_REVERSE);
	/* Make sure we will not draw underlines */
	c.tc_char = TEM_ATTR(attr & ~TEM_ATTR_UNDERLINE) | ' ';

	tem_safe_callback_bit2pix(tem, c);
	da.data = (uchar_t *)tem->tvs_pix_data;

	for (i = 0; i < nrows; i++, row++) {
		da.row = (row + row_add) * da.height + offset_y;
		da.col = col * da.width + offset_x;
		for (j = 0; j < ncols; j++) {
			tems_safe_display(&da, credp, called_from);
			da.col += da.width;
		}
	}
}

/*
 * virtual screen operations
 */
static void
tem_safe_virtual_display(struct tem_vt_state *tem, term_char_t *string,
    int count, screen_pos_t row, screen_pos_t col)
{
	int i, width;
	term_char_t *addr;

	if (row < 0 || row >= tems.ts_c_dimension.height ||
	    col < 0 || col >= tems.ts_c_dimension.width ||
	    col + count > tems.ts_c_dimension.width)
		return;

	width = tems.ts_c_dimension.width;
	addr = tem->tvs_screen_buf + (row * width + col);
	for (i = 0; i < count; i++) {
		*addr++ = string[i];
	}
}

static void
i_virtual_copy_tem_chars(term_char_t *base,
    screen_pos_t s_col, screen_pos_t s_row,
    screen_pos_t e_col, screen_pos_t e_row,
    screen_pos_t t_col, screen_pos_t t_row)
{
	term_char_t	*from;
	term_char_t	*to;
	int		cnt;
	screen_size_t chars_per_row;
	term_char_t	*to_row_start;
	term_char_t	*from_row_start;
	screen_size_t   rows_to_move;
	int		cols = tems.ts_c_dimension.width;

	chars_per_row = e_col - s_col + 1;
	rows_to_move = e_row - s_row + 1;

	to_row_start = base + ((t_row * cols) + t_col);
	from_row_start = base + ((s_row * cols) + s_col);

	if (to_row_start < from_row_start) {
		while (rows_to_move-- > 0) {
			to = to_row_start;
			from = from_row_start;
			to_row_start += cols;
			from_row_start += cols;
			for (cnt = chars_per_row; cnt-- > 0; )
				*to++ = *from++;
		}
	} else {
		/*
		 * Offset to the end of the region and copy backwards.
		 */
		cnt = rows_to_move * cols + chars_per_row;
		to_row_start += cnt;
		from_row_start += cnt;

		while (rows_to_move-- > 0) {
			to_row_start -= cols;
			from_row_start -= cols;
			to = to_row_start;
			from = from_row_start;
			for (cnt = chars_per_row; cnt-- > 0; )
				*--to = *--from;
		}
	}
}

static void
tem_safe_virtual_copy(struct tem_vt_state *tem,
    screen_pos_t s_col, screen_pos_t s_row,
    screen_pos_t e_col, screen_pos_t e_row,
    screen_pos_t t_col, screen_pos_t t_row)
{
	screen_size_t chars_per_row;
	screen_size_t   rows_to_move;
	int		rows = tems.ts_c_dimension.height;
	int		cols = tems.ts_c_dimension.width;

	if (s_col < 0 || s_col >= cols ||
	    s_row < 0 || s_row >= rows ||
	    e_col < 0 || e_col >= cols ||
	    e_row < 0 || e_row >= rows ||
	    t_col < 0 || t_col >= cols ||
	    t_row < 0 || t_row >= rows ||
	    s_col > e_col ||
	    s_row > e_row)
		return;

	chars_per_row = e_col - s_col + 1;
	rows_to_move = e_row - s_row + 1;

	/* More sanity checks. */
	if (t_row + rows_to_move > rows ||
	    t_col + chars_per_row > cols)
		return;

	i_virtual_copy_tem_chars(tem->tvs_screen_buf, s_col, s_row,
	    e_col, e_row, t_col, t_row);
}

static void
tem_safe_virtual_cls(struct tem_vt_state *tem,
    int count, screen_pos_t row, screen_pos_t col)
{
	int i;
	text_attr_t attr;
	term_char_t c;

	tem_safe_get_attr(tem, &c.tc_fg_color, &c.tc_bg_color, &attr,
	    TEM_ATTR_SCREEN_REVERSE);
	c.tc_char = TEM_ATTR(attr & ~TEM_ATTR_UNDERLINE) | ' ';

	for (i = 0; i < tems.ts_c_dimension.width; i++)
		tems.ts_blank_line[i] = c;

	tem_safe_virtual_display(tem, tems.ts_blank_line, count, row, col);
}

/*
 * only blank screen, not clear our screen buffer
 */
void
tem_safe_blank_screen(struct tem_vt_state *tem, cred_t *credp,
    enum called_from called_from)
{
	int	row;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	if (tems.ts_display_mode == VIS_PIXEL) {
		tem_safe_pix_clear_entire_screen(tem, credp, called_from);
		return;
	}

	for (row = 0; row < tems.ts_c_dimension.height; row++) {
		tem_safe_callback_cls(tem,
		    tems.ts_c_dimension.width,
		    row, 0, credp, called_from);
	}
}

/*
 * unblank screen with associated tem from its screen buffer
 */
void
tem_safe_unblank_screen(struct tem_vt_state *tem, cred_t *credp,
    enum called_from called_from)
{
	int	row;

	ASSERT((MUTEX_HELD(&tems.ts_lock) && MUTEX_HELD(&tem->tvs_lock)) ||
	    called_from == CALLED_FROM_STANDALONE);

	if (tems.ts_display_mode == VIS_PIXEL)
		tem_safe_pix_clear_entire_screen(tem, credp, called_from);

	tem_safe_callback_cursor(tem, VIS_HIDE_CURSOR, credp, called_from);

	/*
	 * Display data in tvs_screen_buf to the actual framebuffer in a
	 * row by row way.
	 * When dealing with one row, output data with the same foreground
	 * and background color all together.
	 */
	for (row = 0; row < tems.ts_c_dimension.height; row++) {
		tem_safe_callback_display(tem, tem->tvs_screen_rows[row],
		    tems.ts_c_dimension.width, row, 0, credp, called_from);
	}

	tem_safe_callback_cursor(tem, VIS_DISPLAY_CURSOR, credp, called_from);
}
