// -----------------------------------------------------------------------------
// regex lib - MPF 12/2019
// -----------------------------------------------------------------------------

#ifndef _REGEX_LIB_H
#define _REGEX_LIB_H

// max string size
#define REGEX_MAX_STR_SIZE 4096

// max of parenthesized substrings (groups) 
// e.g. '(\d+?)_(\d+?)_(\d+?)' has 3
#define REGEX_MAX_GROUPS  10

// regex compile flags, OR all needed
// REG_EXTENDED: use extended >> use ? + { | } ( )
// REG_ICASE: ignore case
// REG_NOSUB: report only success/fail in search
// REG_NEWLINE: newline \n divides string into multiple lines
#define REGEX_COMPILE_FLAGS (REG_EXTENDED|REG_ICASE|REG_NEWLINE)

// errors
typedef enum regex_err_enum {
    REGEX_ERR_NONE = 0,
    REGEX_ERR_NOT_FOUND,
    REGEX_ERR_BAD_REGEX,
    REGEX_ERR_MAX_STR_LEN,
    REGEX_ERR_MAX_GROUP,
    REGEX_ERR_BAD_ARG,
    REGEX_ERR_GET_ERR,
    REGEX_ERR_COUNT,
} regex_err_t;

// search regular expression
regex_err_t regex_search(const char * source, const char* regex_search, 
    unsigned int* start, unsigned int* len, unsigned int* nsub);

// extract text
regex_err_t regex_extract(const char * source, char * dest, 
    unsigned int start, unsigned int len);

// replace text
regex_err_t regex_replace(const char * source, char * dest, 
    const char * srch_regex, const char * rpl_str);

// get error message
char* regex_error_msg(regex_err_t err_code);

#endif // _REGEX_LIB_H