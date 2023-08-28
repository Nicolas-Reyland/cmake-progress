#include "progressbar.h"
#include "re.h"

#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define CMAKE_PB_DEFAULT_WIDTH 30
#define CMAKE_PB_START " ["
#define CMAKE_PB_TILE_FULL '#'
#define CMAKE_PB_TILE_EMPTY ' '
#define CMAKE_PB_END "] (%.1f%%)"

static int draw_progressbar(struct progressbar_t *bar);

int start_progressbar(char *line, size_t len, FILE *out) {
    ssize_t nb_read;
    struct progressbar_t bar;
    int err = 0;
    if ((err = read_progressbar(line, &bar, out))) {
#ifdef CMAKE_DEBUG
        fprintf(stderr, "Progressbar reading failed with code %d\n", err);
#endif /* CMAKE_DEBUG */
        return 1;
    }
    bar.out = out;

    // first draw
    (void) draw_progressbar(&bar);
#if defined(CMAKE_DEBUG) || defined(CMAKE_SLEEP_AFTER_DRAW)
    sleep(1);
#endif /* CMAKE_DEBUG */

    while ((nb_read = getline(&line, &len, stdin)) != -1) {
#ifdef CMAKE_DEBUG
        char *line_no_newline = alloca(nb_read);
        strncpy(line_no_newline, line, nb_read);
        int debug_line_differs;
        if ((debug_line_differs = line_no_newline[nb_read - 1] == '\n'))
            line_no_newline[nb_read - 1] = 0;
        printf("Progress bar with '%s'\n", line_no_newline);
#endif /* CMAKE_DEBUG */

        if (regexec(&full_regex, line, 0, NULL, 0) == REG_NOMATCH) {
            // NOTE: don't consume this line if we want to match with other patterns (can fseek back no ?)
            // newline after progress bar
            fputc('\n', bar.out);
            // pass through next line
            fputs(line, stdout);
#ifdef CMAKE_DEBUG
            if (debug_line_differs)
                fputc('\n', stdout);
#endif /* CMAKE_DEBUG */
            return 0;
        }

        if ((err = update_progressbar(&bar, line))) {
            // newline after progress bar
            fputc('\n', bar.out);
            if (bar.flags & CMAKE_PG_PASS_THROUGH)
            {
                fputs(line, stdout);
#ifdef CMAKE_DEBUG
                if (debug_line_differs)
                    fputc('\n', stdout);
#endif /* CMAKE_DEBUG */
            } else {
#ifdef CMAKE_DEBUG
                fprintf(stderr, "Progressbar updating failed with code %d\n", err);
#endif /* CMAKE_DEBUG */
                return 1;
            }
        }
        (void) draw_progressbar(&bar);
#if defined(CMAKE_DEBUG) || defined(CMAKE_SLEEP_AFTER_DRAW)
        sleep(1);
#endif /* CMAKE_DEBUG */
    }
    // newline after progress bar
    fputc('\n', bar.out);
    return 0;
}

int update_progressbar(struct progressbar_t *bar, const char *line) {
    char *endptr = NULL;
    long cur = strtol(line + 1, &endptr, 10);
    if (*endptr != '/')
        return 1;
    if (cur > USHRT_MAX || cur < 0)
        return 2;
    bar->cur = (unsigned short) cur;
    return 0;
}

int draw_progressbar(struct progressbar_t *bar) {
    float ratio = bar->cur > bar->max ? (float) 1.0 : (float) bar->cur / (float) bar->max;
    short filled_tiles = (short) (ratio * (float) bar->bar_width);
    short empty_tiles = bar->bar_width - filled_tiles;
#ifdef CMAKE_DEBUG
    printf("Cur: %d, Max: %d, Width: %d, Ratio: %.2f, Filled: %d, Empty: %d\n", bar->cur, bar->max, bar->bar_width,
           ratio, filled_tiles,
           empty_tiles);
    fputs(CMAKE_PB_START, bar->out);
#else
    fputs("\r" CMAKE_PB_START, bar->out);
#endif /* CMAKE_DEBUG */
    for (short i = 0; i < filled_tiles; ++i)
        fputc(CMAKE_PB_TILE_FULL, bar->out);
    for (short i = 0; i < empty_tiles; ++i)
        fputc(CMAKE_PB_TILE_EMPTY, bar->out);
    fprintf(bar->out, CMAKE_PB_END, (float) 100.0 * ratio);
#ifdef CMAKE_DEBUG
    fputc('\n', bar->out);
#endif /* CMAKE_DEBUG */
    fflush(bar->out);
    return 0;
}

int read_progressbar(const char *line, struct progressbar_t *bar, FILE *out) {
    bar->out = out;

    char *endptr = NULL;
    long cur, max;
    cur = strtol(line + 1, &endptr, 10);
    if (cur > USHRT_MAX || cur < 0)
        return 1;
    bar->cur = (unsigned short) cur;
    if (*endptr != '/')
        return 2;
    max = strtol(endptr + 1, &endptr, 10);
    if (max > USHRT_MAX || max < 0)
        return 3;
    bar->max = (unsigned short) max;
    if (*endptr != ']')
        return 4;

    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl(winsize)");
        bar->bar_width = CMAKE_PB_DEFAULT_WIDTH;
    } else
        bar->bar_width = (short) w.ws_col - (short) (strlen(CMAKE_PB_START) + strlen(CMAKE_PB_END) + 1);

    return 0;
}
