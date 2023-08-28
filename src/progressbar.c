#include "progressbar.h"
#include "re.h"

#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <limits.h>

#define CMAKE_DF_BAR_WIDTH 30

void start_progress_bar(char *line, size_t len)
{
    ssize_t nb_read;
    struct progressbar_t bar;
    if (read_progressbar(line, &bar))
        return;

    while ((nb_read = getline(&line, &len, stdin)) != -1)
    {
#ifdef CMAKE_DEBUG
        if (line[nb_read - 1] == '\n')
            line[nb_read - 1] = 0;
        printf("Progress bar with '%s'\n", line);
#endif /* CMAKE_DEBUG */
        if (regexec(&full_regex, line, 0, NULL, 0) == REG_NOMATCH)
            // NOTE: don't consume this line if we want to match with other patterns (can fseek back no ?)
            return;
    }
}

int read_progressbar(const char* line, struct progressbar_t* bar)
{
    char *endptr = NULL;
    long cur, max;
    cur = strtol(line + 1, &endptr, 10);
    if (cur > USHRT_MAX)
        return 1;
    bar->cur = (unsigned short)cur;
    if (*endptr != '/')
       return 2;
    max = strtol(endptr + 1, &endptr, 10);
    if (max > USHRT_MAX)
        return 3;
    bar->max = (unsigned short)max;
    if (*endptr != ']')
        return 4;

    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
    {
        perror("ioctl(winsize)");
        bar->bar_width = CMAKE_DF_BAR_WIDTH;
    }
    else
        bar->bar_width = w.ws_col;

    return 0;
}
