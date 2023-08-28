#ifndef CMAKE_PROGRESS_PROGRESSBAR_H
#define CMAKE_PROGRESS_PROGRESSBAR_H

#include <stddef.h>
#include <bits/types/FILE.h>

#define CMAKE_PG_PASS_THROUGH 0b0001

struct progressbar_t
{
    unsigned short cur;
    unsigned short max;
    short bar_width;
    FILE* out;
    int flags;
};

int start_progressbar(char *line, size_t len, FILE* out);

int update_progressbar(struct progressbar_t* bar, const char* line);

int read_progressbar(const char* line, struct progressbar_t* bar, FILE* out);

#endif /* !CMAKE_PROGRESS_PROGRESSBAR_H */