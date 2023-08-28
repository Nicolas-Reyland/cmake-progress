#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

#include "re.h"
#include "progressbar.h"

#define STRINGIFY_MACRO(x) STRINGIFY(x)
#define STRINGIFY(x) #x

#define CMAKE_FILE_INDEX_MAX_CHARS 5
#define CMAKE_CUR_FILE_INDEX_REGEX_PATTERN "[0-9]\\{1," STRINGIFY_MACRO(CMAKE_FILE_INDEX_MAX_CHARS) "\\}"
#define CMAKE_FULL_REGEX_PATTERN "^\\[" CMAKE_CUR_FILE_INDEX_REGEX_PATTERN "/" CMAKE_CUR_FILE_INDEX_REGEX_PATTERN "\\] "

//_Static_assert(sizeof(unsigned short) >= 2, "unsigned short is not at least 2 bytes");

static FILE* out_stream;

int main(void)
{
    ssize_t nb_read;
    size_t len = 0;
    char *line = NULL;

    // TODO: parse option for stderr output
    out_stream = stdout;

    full_regex = create_regex(CMAKE_FULL_REGEX_PATTERN);

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
            start_progressbar(line, len, out_stream);
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
