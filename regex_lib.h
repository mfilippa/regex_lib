// -----------------------------------------------------------------------------
// regex lib - MPF 12/2019
// -----------------------------------------------------------------------------

// regex include
#include <regex.h>

#ifndef _REGEX_LIB_H
#define _REGEX_LIB_H

// max string size
#define MAX_STR_SIZE 4096

// max of parenthesized substrings (groups) 
// e.g. '(\d+?)_(\d+?)_(\d+?)' has 3
#define MAX_GROUPS  10

// regex compile flags, OR all needed
// REG_EXTENDED: use extended >> use ? + { | } ( )
// REG_ICASE: ignore case
// REG_NOSUB: report only success/fail in regexec()
// REG_NEWLINE: new line handling, see documentation
#define REGEX_COMPILE_FLAGS (REG_EXTENDED|REG_ICASE|REG_NEWLINE)


int compile_regex(regex_t * re, const char * regex_str);

int search_regex(regex_t * re, const char * text, unsigned int* start, 
    unsigned int* len, unsigned int* nsub);

int extract_text(const char * source, char * dest, unsigned int start, 
    unsigned int len, unsigned int max_size);

int replace_text(const char * source, const char * replace_str, char * dest,
    unsigned int* start, unsigned int* len, unsigned int* nsub);

#endif // _REGEX_LIB_H