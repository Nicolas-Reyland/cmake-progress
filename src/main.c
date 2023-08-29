#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <argp.h>

#include "re.h"
#include "progressbar.h"

#define CMAKE_PROGRESS_VERSION "0.1.0"

#define STRINGIFY_MACRO(x) STRINGIFY(x)
#define STRINGIFY(x) #x

#define CMAKE_FILE_INDEX_MAX_CHARS 5
#define CMAKE_CUR_FILE_INDEX_REGEX_PATTERN "[0-9]\\{1," STRINGIFY_MACRO(CMAKE_FILE_INDEX_MAX_CHARS) "\\}"
#define CMAKE_FULL_REGEX_PATTERN "^\\[" CMAKE_CUR_FILE_INDEX_REGEX_PATTERN "/" CMAKE_CUR_FILE_INDEX_REGEX_PATTERN "\\] "

//_Static_assert(sizeof(unsigned short) >= 2, "unsigned short is not at least 2 bytes");

const char *argp_program_version = "cmake-progress " CMAKE_PROGRESS_VERSION;
const char *argp_program_bug_address = "<nicolas@reyland.dev>";
static char doc[] = "Replace cmake output with nice progress bars";
static char args_doc[] = "";
static struct argp_option options[] = {
        {"pass-through", 'p', 0,        0, "Don't stop until the end of compiling.",                 0},
        {"width",        'w', "WIDTH",  0, "Width of progress bar (must be between 5 and 1000).",    0},
        {"output",       'o', "OUTPUT", 0, "File to output the progressbar to (default is stdout).", 0},
        {0}
};

struct arguments {
    enum {
        STRICT_MODE = 0,
        PASS_THROUGH_MODE = CMAKE_PB_PASS_THROUGH,
    } mode;
    short width;
    char *outfile;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 'p':
            arguments->mode = PASS_THROUGH_MODE;
            break;
        case 'w': {
            char *endptr = NULL;
            long width = strtol(arg, &endptr, 10);
            if (endptr == arg)
                return ARGP_HELP_EXIT_ERR;
            if (width < 5 || width >= 1000)
                return ARGP_HELP_EXIT_ERR;
            arguments->width = (short) width;
        }
            break;
        case 'o':
            arguments->outfile = arg;
            break;
        case ARGP_KEY_ARG:
            return 0;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {
        options,
        parse_opt,
        args_doc,
        doc,
        0,
        0,
        0,
};

int main(int argc, char **argv) {
    ssize_t nb_read;
    size_t len = 0;
    char *line = NULL;

    struct arguments arguments;

    arguments.mode = STRICT_MODE;
    arguments.width = 0;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    struct progressbar_t bar = {
            0, 0,
            .bar_width = arguments.width,
            .out = NULL,
            .flags =arguments.mode | (arguments.width ? CMAKE_PB_SET_WIDTH : 0),
    };

    if (arguments.outfile == NULL)
        bar.out = stdout;
    else {
        bar.out = fopen(arguments.outfile, "w");
        if (bar.out == NULL) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
    }

    full_regex = create_regex(CMAKE_FULL_REGEX_PATTERN);

    while ((nb_read = getline(&line, &len, stdin)) != -1) {
        if (nb_read == 0)
            break;
#ifdef CMAKE_DEBUG
        if (line[nb_read - 1] == '\n')
            line[nb_read - 1] = 0;
#endif /* CMAKE_DEBUG */
        if (regexec(&full_regex, line, 0, NULL, 0) == 0) {
#ifdef CMAKE_DEBUG
            printf("Found a match for line '%s'\n", line);
#endif /* CMAKE_DEBUG */
            if (start_progressbar(line, len, &bar)) {
#ifdef CMAKE_DEBUG
                fprintf(stderr, "Progressbar failed\n");
#endif /* CMAKE_DEBUG */
            }
        } else
#ifdef CMAKE_DEBUG
            puts(line);
#else
            fputs(line, stdout);
#endif /* CMAKE_DEBUG */
    }

    free(line);
    regfree(&full_regex);

    return 0;
}
