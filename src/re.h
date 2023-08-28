#ifndef CMAKE_PROGRESS_RE_H
#define CMAKE_PROGRESS_RE_H

#include <regex.h>

extern regex_t full_regex;

regex_t create_regex(const char* pattern);

#endif /* !CMAKE_PROGRESS_RE_H */