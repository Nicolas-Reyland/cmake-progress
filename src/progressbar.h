#ifndef CMAKE_PROGRESS_PROGRESSBAR_H
#define CMAKE_PROGRESS_PROGRESSBAR_H

#include <stddef.h>

struct progressbar_t
{
    unsigned short cur;
    unsigned short max;
    unsigned short bar_width;
};

int read_progressbar(const char* line, struct progressbar_t* bar);

void start_progress_bar(char *line, size_t len);

#endif /* !CMAKE_PROGRESS_PROGRESSBAR_H */