#ifndef CMAKE_PROGRESS_PROGRESSBAR_H
#define CMAKE_PROGRESS_PROGRESSBAR_H

#include <stddef.h>
#include <bits/types/FILE.h>

#define CMAKE_PB_PASS_THROUGH 0b0001
#define CMAKE_PB_SET_WIDTH    0b0010

struct progressbar_t {
    unsigned short cur;
    unsigned short max;
    short bar_width;
    FILE *out;
    unsigned int flags;
};

int start_progressbar(char *line, size_t len, struct progressbar_t *bar);

int update_progressbar(struct progressbar_t *bar, const char *line);

int read_progressbar(const char *line, struct progressbar_t *bar);

#endif /* !CMAKE_PROGRESS_PROGRESSBAR_H */