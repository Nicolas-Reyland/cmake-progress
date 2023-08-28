#include "re.h"

#include <stdio.h>
#include <stdlib.h>

regex_t full_regex;

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
