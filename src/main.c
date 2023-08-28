#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

#define CMAKE_CUR_FILE_INDEX_REGEX_PATTERN "[0-9]\\{1,5\\}"
#define CMAKE_FULL_REGEX_PATTERN "^\\[" CMAKE_CUR_FILE_INDEX_REGEX_PATTERN "/" CMAKE_CUR_FILE_INDEX_REGEX_PATTERN "\\] "

//_Static_assert(sizeof(unsigned short) >= 2, "unsigned short is not at least 2 bytes");

struct progressbar_t
{
     unsigned short cur;
     unsigned short max;
     unsigned short bar_width;
};

static FILE* out_stream;

static regex_t full_regex;
static regex_t cur_file_index_regex;

static void start_progress_bar(char *line, size_t len);
static regex_t create_regex(const char* pattern);

int main(void)
{
    ssize_t nb_read;
    size_t len = 0;
    char *line = NULL;

    // TODO: parse option for stderr output
    out_stream = stdout;

    full_regex = create_regex(CMAKE_FULL_REGEX_PATTERN);
    cur_file_index_regex = create_regex(CMAKE_CUR_FILE_INDEX_REGEX_PATTERN);

    while ((nb_read = getline(&line, &len, stdin)) != -1)
    {
#ifdef CMAKE_DEBUG
        if (line[nb_read - 1] == '\n')
            line[nb_read - 1] = 0;
#endif /* CMAKE_DEBUG */
        if (regexec(&full_regex, line, 0, NULL, 0) == 0)
        {
#ifdef CMAKE_DEBUG
            printf("Found a match for line '%s'\n", line);
#endif /* CMAKE_DEBUG */
            start_progress_bar(line, len);
        }
        else
#ifdef CMAKE_DEBUG
            puts(line);
#else
            fputs(line, out_stream);
#endif /* CMAKE_DEBUG */
    }

    regfree(&full_regex);
    free(line);

    return 0;
}

static struct progressbar_t read_progressbar(const char* line);

void start_progress_bar(char *line, size_t len)
{
    ssize_t nb_read;
    progressbar_t bar = read_progressbar(line);

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

struct progressbar_t read_progressbar(const char* line)
{
    struct progressbar_t bar;

    return bar;
}

regex_t create_regex(const char* pattern)
{
    regex_t regex;
    if (regcomp(&regex, pattern, 0))
    {
        fprintf(stderr, "Failed to compile %s\n", pattern);
        exit(EXIT_FAILURE);
    }
    return regex;
}
