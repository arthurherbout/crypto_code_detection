/*
png2pos is a utility to convert PNG images to ESC/POS format
(printer control codes and escape sequences) used by POS thermal
printers.
*/

#ifndef __linux__
#define NOSECCOMP
#endif

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "lodepng.h"

#ifndef NOSECCOMP
#include "seccomp.h"
#endif

#ifdef LODEPNG_NO_COMPILE_ALLOCATORS
/* modified lodepng allocators */

void *lodepng_malloc(size_t size) {
    /* for security reason I use calloc instead of malloc;
       here we redefine lodepng allocator */
    return calloc(size, 1);
}

void *lodepng_realloc(void *ptr, size_t new_size) {
    return realloc(ptr, new_size);
}

void lodepng_free(void *ptr) {
    free(ptr);
}
#endif

/* number of dots/lines in vertical direction in one F112 command
   set GS8L_MAX_Y env. var. to <= 128u for Epson TM-J2000/J2100
   default value is 1662, TM-T70, TM-T88 etc. */
#ifndef GS8L_MAX_Y
#define GS8L_MAX_Y 1662
#endif

/* max image width printer is able to process;
   printer_max_width must be divisible by 8!! */
#ifndef PRINTER_MAX_WIDTH
#define PRINTER_MAX_WIDTH 512u
#endif

struct dithering_matrix {
    int dx; /* x-offset */
    int dy; /* y-offset */
    int v; /* error = v * 1/1,024th of value */
};

struct app_config {
    unsigned int cut;
    unsigned int photo;
    char align;
    unsigned int rotate;
    const char *output;
    unsigned int gs8l_max_y;
    unsigned int printer_max_width;
    unsigned int speed;
};

/* app configuration */
struct app_config config = {
    .cut = 0,
    .photo = 0,
    .align = '?',
    .rotate = 0,
    .output = NULL,
    .gs8l_max_y = GS8L_MAX_Y,
    .printer_max_width = PRINTER_MAX_WIDTH,
    .speed = 0
};

FILE *fout = NULL;

#ifdef DEBUG
void pbm_write(const char *filename, unsigned int w, unsigned int h,
    const unsigned char *buffer, size_t buffer_size) {

    if (access(filename, F_OK) != -1) {
        fprintf(stderr, "Debug file '%s' exists. Please remove it.\n", filename);
    } else {
        FILE *f = fopen(filename, "wb");
        if (f) {
            fprintf(f, "P4\n%u %u\n", w, h);
            fwrite(buffer, 1, buffer_size, f);
            fflush(f);
            fclose(f);
            f = NULL;
        }
    }
}

void png_write(const char *filename, unsigned int w, unsigned int h,
    const unsigned char *buffer, size_t img_bw_size) {

    if (access(filename, F_OK) != -1) {
        fprintf(stderr, "Debug file '%s' exists. Please remove it.\n", filename);
    } else {
        unsigned char *inv = calloc(img_bw_size, 1);
        if (inv) {
            for (unsigned int i = 0; i != img_bw_size; ++i) {
                inv[i] = ~buffer[i];
            }
            lodepng_encode_file(filename, inv, w, h, LCT_GREY, 1);
            free(inv);
            inv = NULL;
        }
    }
}
#endif

unsigned char s_add_to_byte(unsigned char v, int d) {
    int a = v + d;
    if (a > 0xff) {
        a = 0xff;
    } else if (a < 0) {
        a = 0;
    }
    return a;
}

int main(int argc, char *argv[]) {
    int ret = EXIT_FAILURE;

    unsigned char *img_rgba = NULL;
    unsigned char *img_grey = NULL;
    unsigned char *img_bw = NULL;

#ifndef NOSECCOMP
    /* none of our children will ever be granted more privs,
       escape via ptrace is impossible,
       init, set, compile and load custom BPF */
    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0
        || prctl(PR_SET_DUMPABLE, 0, 0, 0, 0) < 0
        || prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &seccomp_filter_prog) < 0) {

        fprintf(stderr, "Unable to initialize seccomp subsystem\n");
        goto fail;
    }
#endif

    /* http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap12.html
       Utility Conventions: 12.1 Utility Argument Syntax */
    extern int optind;
    extern int opterr;
    extern int optopt;
    extern char *optarg;
    int optc = -1;

    while ((optc = getopt(argc, argv, ":Vhca:rps:o:")) != -1) {
        switch (optc) {
            case 'o':
                config.output = optarg;
                break;

            case 'c':
                config.cut = 1;
                break;

            case 'a':
                config.align = toupper(optarg[0]);
                if (!strchr("LCR", config.align)) {
                    fprintf(stderr, "Unknown horizontal alignment '%c'\n",
                        config.align);
                    goto fail;
                }
                break;

            case 'r':
                config.rotate = 1;
                break;

            case 'p':
                config.photo = 1;
                break;

            case 's':
                config.speed = strtoul(optarg, NULL, 0);
                if (config.speed < 1 || config.speed > 9) {
                    config.speed = 0;
                    fprintf(stderr, "Speed must be in the interval <1; 9>."
                        " Falling back to the default speed\n");
                }
                break;

            case 'V':
                fprintf(stderr, "%s %s\n", "png2pos", GIT_VERSION);
                fprintf(stderr, "%s %s\n", "LodePNG", LODEPNG_VERSION_STRING);
                ret = EXIT_SUCCESS;
                goto fail;

            case 'h':
                fprintf(stderr,
                    "png2pos is a utility to convert PNG to ESC/POS\n"
                    "Usage: png2pos [-V] [-h] [-c] [-a L|C|R] [-r] [-p]  [-s SPEED] [-o FILE] [INPUT_FILES...]\n"
                    "\n"
                    "  -V           display the version number and exit\n"
                    "  -h           display this short help and exit\n"
                    "  -c           cut the paper at the end of job\n"
                    "  -a L|C|R     horizontal image alignment (Left, Center, Right)\n"
                    "  -r           rotate image upside down before it is printed\n"
                    "  -p           switch to photo mode (post-process input files)\n"
                    "  -s SPEED     set the print speed (1 = slow .. 9 = fast)\n"
                    "  -o FILE      output file\n"
                    "\n"
                    "With no FILE, or when FILE is -, write to standard output\n"
                    "\n"
                    "The following environment variables are recognized by png2pos:\n"
                    "  PNG2POS_PRINTER_MAX_WIDTH\n"
                    "  PNG2POS_GS8L_MAX_Y\n"
                    "\n"
                    "Please read the manual page (man png2pos)\n"
                    "Report bugs at https://github.com/petrkutalek/png2pos/issues\n"
                    "(c) Petr Kutalek <petr@kutalek.cz>, 2012-2019, Licensed under the MIT license\n"
                    );
                ret = EXIT_SUCCESS;
                goto fail;

            case ':':
                fprintf(stderr, "Option '%c' requires an argument\n", optopt);
                fprintf(stderr, "For usage options run 'png2pos -h'\n");
                goto fail;

            default:
            case '?':
                fprintf(stderr, "'%c' is an unknown option\n", optopt);
                fprintf(stderr, "For usage options run 'png2pos -h'\n");
                goto fail;
        }
    }

    argc -= optind;
    argv += optind;
    optind = 0;

    {
        char *printer_max_width_env = getenv("PNG2POS_PRINTER_MAX_WIDTH");

        if (printer_max_width_env) {
            config.printer_max_width = strtoul(printer_max_width_env, NULL, 0);
        }
        /* printer_max_width must be divisible by 8!! */
        config.printer_max_width &= ~0x7u;
    }

    {
        char *gs8l_max_y_env = getenv("PNG2POS_GS8L_MAX_Y");

        if (gs8l_max_y_env) {
            config.gs8l_max_y = strtoul(gs8l_max_y_env, NULL, 0);
        }
    }

    /* open output file and disable line buffering */
    if (!config.output || !strcmp(config.output, "-")) {
        fout = stdout;
    } else {
        fout = fopen(config.output, "wb");
        if (!fout) {
            fprintf(stderr, "Could not open output file '%s'\n", config.output);
            goto fail;
        }
    }

    if (isatty(fileno(fout))) {
        fprintf(stderr, "This utility produces binary sequence printer"
            " commands. Output have to be redirected\n");
        goto fail;
    }

    if (setvbuf(fout, NULL, _IOFBF, 8192)) {
        fprintf(stderr, "Could not set new buffer policy on output stream\n");
    }
    /* init printer */
    const unsigned char ESC_INIT[2] = {
        /* ESC @, Initialize printer, p. 412 */
        0x1b, 0x40
    };
    fwrite(ESC_INIT, 1, sizeof ESC_INIT, fout);
    fflush(fout);

    /* print speed */
    if (config.speed > 0) {
        const unsigned char ESC_SPEED[7] = {
            /* GS ( K <Function 50>, Select the print speed, p. 451 */
            0x1d, 0x28, 0x4b, 0x02, 0x00, 0x32,
            /* m (01-09) */
            config.speed
        };
        fwrite(ESC_SPEED, 1, sizeof ESC_SPEED, fout);
        fflush(fout);
    }

    /* for each input file */
    while (optind != argc) {
        char *input = argv[optind++];

        /* load RGBA PNG */
        unsigned int img_w = 0;
        unsigned int img_h = 0;
        unsigned int lodepng_error = lodepng_decode32_file(&img_rgba,
            &img_w, &img_h, input);

        if (lodepng_error) {
            fprintf(stderr, "Could not load and process input PNG file, %s\n",
                lodepng_error_text(lodepng_error));
            goto fail;
        }

        if (img_w > config.printer_max_width) {
            fprintf(stderr, "Image width %u px exceeds the printer's"
                " capability (%u px)\n", img_w, config.printer_max_width);
            goto fail;
        }

        unsigned int histogram[256] = { 0 };

        /* convert RGBA to greyscale */
        unsigned int img_grey_size = img_h * img_w;

        img_grey = calloc(img_grey_size, 1);
        if (!img_grey) {
            fprintf(stderr, "Could not allocate enough memory\n");
            goto fail;
        }

        for (unsigned int i = 0; i != img_grey_size; ++i) {
            /* A */
            unsigned int a = img_rgba[(i << 2) | 3];

            /* RGBA → RGB → L* */
            unsigned int r = (255 - a) + a / 255 * img_rgba[i << 2];
            unsigned int g = (255 - a) + a / 255 * img_rgba[(i << 2) | 1];
            unsigned int b = (255 - a) + a / 255 * img_rgba[(i << 2) | 2];
            unsigned int L = (55 * r + 182 * g + 18 * b) / 255;

            img_grey[i] = L;

            /* prepare a histogram for HEA */
            ++histogram[img_grey[i]];
        }

        free(img_rgba);
        img_rgba = NULL;

        {
            /* -p hints */
            unsigned int colors = 0;

            for (unsigned int i = 0; i != 256; ++i) {
                if (histogram[i]) {
                    ++colors;
                }
            }
            if (colors < 16 && config.photo) {
                fprintf(stderr, "Image seems to be B/W. -p is probably"
                    " not good option this time\n");
            }
            if (colors >= 16 && !config.photo) {
                fprintf(stderr, "Image seems to be greyscale or colored."
                    " Maybe you should use option -p for better results\n");
            }
        }

        /* post-processing convert to B/W bitmap */
        if (config.photo) {
            /* Histogram Equalization Algorithm */
            for (unsigned int i = 1; i != 256; ++i) {
                histogram[i] += histogram[i - 1];
            }
            for (unsigned int i = 0; i != img_grey_size; ++i) {
                img_grey[i] = 255 * histogram[img_grey[i]] / img_grey_size;
            }

            /* Jarvis, Judice, and Ninke Dithering
               http://www.tannerhelland.com/4660/
                dithering-eleven-algorithms-source-code/

               In the same year that Floyd and Steinberg published their
               famous dithering algorithm, a lesser-known – but much more
               powerful – algorithm was also published. With this
               algorithm, the error is distributed to three times as many
               pixels as in Floyd-Steinberg, leading to much smoother –
               and more subtle – output. */
            const struct dithering_matrix dithering_matrix[12] = {
                /* for simplicity of computation, all standard dithering
                   formulas push the error forward, never backward */
                {.dx =  1, .dy = 0, .v = 149 /* 1024 * 7 / 48 */},
                {.dx =  2, .dy = 0, .v = 107 /* 1024 * 5 / 48 */},
                {.dx = -2, .dy = 1, .v =  64 /* 1024 * 3 / 48 */},
                {.dx = -1, .dy = 1, .v = 107 /* 1024 * 5 / 48 */},
                {.dx =  0, .dy = 1, .v = 149 /* 1024 * 7 / 48 */},
                {.dx =  1, .dy = 1, .v = 107 /* 1024 * 5 / 48 */},
                {.dx =  2, .dy = 1, .v =  64 /* 1024 * 3 / 48 */},
                {.dx = -2, .dy = 2, .v =  21 /* 1024 * 1 / 48 */},
                {.dx = -1, .dy = 2, .v =  64 /* 1024 * 3 / 48 */},
                {.dx =  0, .dy = 2, .v = 107 /* 1024 * 5 / 48 */},
                {.dx =  1, .dy = 2, .v =  64 /* 1024 * 3 / 48 */},
                {.dx =  2, .dy = 2, .v =  21 /* 1024 * 1 / 48 */}
            };

            for (unsigned int i = 0; i != img_grey_size; ++i) {
                unsigned int o = img_grey[i];
                unsigned int n = o <= 0x80 ? 0x00 : 0xff;

                int x = i % img_w;
                int y = i / img_w;

                img_grey[i] = n;
                for (unsigned int j = 0; j != 12; ++j) {
                    int x0 = x + dithering_matrix[j].dx;
                    int y0 = y + dithering_matrix[j].dy;

                    if (x0 > (int) img_w - 1 || x0 < 0
                        || y0 > (int) img_h - 1 || y0 < 0) {

                        continue;
                    }
                    /* the residual quantization error, warning: !have to
                       overcast to signed int before calculation! */
                    int d = (int) (o - n) * dithering_matrix[j].v / 1024;

                    /* keep a value in the <min; max> interval */
                    img_grey[x0 + img_w * y0] =
                        s_add_to_byte(img_grey[x0 + img_w * y0], d);
                }
            }
        }
        /* canvas size is width of printable area */
        unsigned int canvas_w = config.printer_max_width;

        unsigned int img_bw_size = img_h * (canvas_w >> 3);

        img_bw = calloc(img_bw_size, 1);
        if (!img_bw) {
            fprintf(stderr, "Could not allocate enough memory\n");
            goto fail;
        }

        /* align rotated image to the right border */
        if (config.rotate && config.align == '?') {
            config.align = 'R';
        }

        /* left offset */
        unsigned int offset = 0;

        switch (config.align) {
            case 'C':
                offset = (canvas_w - img_w) / 2;
                break;

            case 'R':
                offset = canvas_w - img_w;
                break;

            case 'L':
            case '?':
            default:
                offset = 0;
        }

        /* compress bytes into bitmap */
        for (unsigned int i = 0; i != img_grey_size; ++i) {
            unsigned int idx = config.rotate ? img_grey_size - 1 - i : i;

            if (img_grey[idx] <= 0x80) {
                unsigned int x = i % img_w + offset;
                unsigned int y = i / img_w;

                img_bw[(y * canvas_w + x) >> 3] |= 0x80 >> (x & 0x07);
            }
        }

        free(img_grey);
        img_grey = NULL;

#ifdef DEBUG
        pbm_write("debug.pbm", canvas_w, img_h, img_bw, img_bw_size);
        png_write("debug.png", canvas_w, img_h, img_bw, img_bw_size);
#endif

        /* chunking, l = lines already printed, currently processing a
           chunk of height k */
        for (unsigned int l = 0, k = config.gs8l_max_y; l < img_h; l += k) {
            if (k > img_h - l) {
                k = img_h - l;
            }

            unsigned int f112_p = 10 + k * (canvas_w >> 3);

            const unsigned char ESC_STORE[17] = {
                /* GS 8 L, Store the graphics data in the print buffer
                   (raster format), p. 252 */
                0x1d, 0x38, 0x4c,
                /* p1 p2 p3 p4 */
                f112_p & 0xff, f112_p >> 8 & 0xff, f112_p >> 16 & 0xff,
                f112_p >> 24 & 0xff,
                /* Function 112 */
                0x30, 0x70, 0x30,
                /* bx by, zoom */
                0x01, 0x01,
                /* c, single-color printing model */
                0x31,
                /* xl, xh, number of dots in the horizontal direction */
                canvas_w & 0xff, canvas_w >> 8 & 0xff,
                /* yl, yh, number of dots in the vertical direction */
                k & 0xff, k >> 8 & 0xff
            };
            fwrite(ESC_STORE, 1, sizeof ESC_STORE, fout);
            fwrite(&img_bw[l * (canvas_w >> 3)], 1, k * (canvas_w >> 3), fout);

            const unsigned char ESC_FLUSH[7] = {
                /* GS ( L, Print the graphics data in the print buffer,
                   p. 241 Moves print position to the left side of the
                   print area after printing of graphics data is
                   completed */
                0x1d, 0x28, 0x4c, 0x02, 0x00, 0x30,
                /* Fn 50 */
                0x32
            };
            fwrite(ESC_FLUSH, 1, sizeof ESC_FLUSH, fout);
            fflush(fout);
        }

        free(img_bw);
        img_bw = NULL;
    }

    if (config.cut) {
        /* cut the paper */
        const unsigned char ESC_CUT[4] = {
            /* GS V, Sub-Function B, p. 373 */
            0x1d, 0x56, 0x41,
            /* Feeds paper to (cutting position + n × vertical motion
               unit) and executes a full cut (cuts the paper completely)
               The vertical motion unit is specified by GS P. */
            0x40
        };
        fwrite(ESC_CUT, 1, sizeof ESC_CUT, fout);
        fflush(fout);
    }

    ret = EXIT_SUCCESS;

fail:
    free(img_rgba);
    img_rgba = NULL;

    free(img_grey);
    img_grey = NULL;

    free(img_bw);
    img_bw = NULL;

    if (fout && fout != stdout) {
        fclose(fout);
        fout = NULL;
    }

    return ret;
}
