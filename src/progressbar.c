#include "progressbar.h"
#include "re.h"

#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <limits.h>

#define CMAKE_PB_DEFAULT_WIDTH 30
#define CMAKE_PB_START " ["
#define CMAKE_PB_TILE_FULL '#'
#define CMAKE_PB_TILE_EMPTY ' '
#define CMAKE_PB_END "] (%.2f%%)"

static int print_progressbar(struct progressbar_t *bar);

int start_progressbar(char *line, size_t len, FILE *out) {
    ssize_t nb_read;
    struct progressbar_t bar;
    if (read_progressbar(line, &bar, out))
        return 1;
    bar.out = out;

    while ((nb_read = getline(&line, &len, stdin)) != -1) {
#ifdef CMAKE_DEBUG
        if (line[nb_read - 1] == '\n')
            line[nb_read - 1] = 0;
        printf("Progress bar with '%s'\n", line);
#endif /* CMAKE_DEBUG */
        if (!update_progressbar(&bar, line))
            return 1;
        (void) print_progressbar(&bar);
        if (regexec(&full_regex, line, 0, NULL, 0) == REG_NOMATCH)
            // NOTE: don't consume this line if we want to match with other patterns (can fseek back no ?)
            return 0;
    }
    return 0;
}

int update_progressbar(struct progressbar_t *bar, const char *line) {
    char *endptr = NULL;
    long cur = strtol(line, &endptr, 10);
    if (cur > USHRT_MAX || cur < 0)
        return 1;
    bar->cur = (unsigned short) cur;
    return 0;
}

int print_progressbar(struct progressbar_t *bar) {
#ifdef CMAKE_DEBUG
    fputs(CMAKE_PB_START, bar->out);
#else
    fputs("\r" CMAKE_PB_START, bar->out);
#endif /* CMAKE_DEBUG */
    float percentage = (float) 100.0 * (float) (bar->max > bar->cur ? bar->max : bar->cur) / (float) bar->max;
    short filled_tiles = (short) (percentage * (float) bar->bar_width);
    short empty_tiles = bar->bar_width - filled_tiles;
    for (short i = 0; i < filled_tiles; ++i)
        fputc(CMAKE_PB_TILE_FULL, bar->out);
    for (short i = 0; i < empty_tiles; ++i)
        fputc(CMAKE_PB_TILE_EMPTY, bar->out);
    fprintf(bar->out, CMAKE_PB_END, percentage);
#ifdef CMAKE_DEBUG
    fputc('\n', bar->out);
#endif /* CMAKE_DEBUG */
    fflush(bar->out);
    return 0;
}

int close_progressbar(struct progressbar_t *bar) {
    if (bar->out != stdout && bar->out != stderr)
        fclose(bar->out);
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
        bar->bar_width = (short) w.ws_col;

    return 0;
}
