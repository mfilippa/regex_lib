// -----------------------------------------------------------------------------
// regex lib - MPF 12/2019
// -----------------------------------------------------------------------------

// includes
#include <stdio.h>
#include <string.h>
#include "regex_lib.h"

// regex error msg size
#define MAX_ERROR_MSG 4096
char regex_err[MAX_ERROR_MSG];

// -----------------------------------------------------------------------------
// compile regex - returns 0 if sucessful
// -----------------------------------------------------------------------------
int compile_regex(regex_t * re, const char * regex_str) {
    int status;
    // compile regex, returns 0 if successful
    status = regcomp(re, regex_str, REGEX_COMPILE_FLAGS);
    if (status!=0){
        regerror(status, re, regex_err, MAX_ERROR_MSG);
        printf("Regex compile error, exit code %u: %s\n", status, regex_err);
    }   
    return status;
}

// -----------------------------------------------------------------------------
// search regex - return 0 if found
// -----------------------------------------------------------------------------
int search_regex(regex_t * re, const char * text, unsigned int* start, 
    unsigned int* len, unsigned int* nsub){
    int status;
    regmatch_t match[MAX_GROUPS];
    // execute search, returns 0 if found
    status = regexec(re, text, MAX_GROUPS, match, 0);
    if (status==0) {
        // return indices to start/end of match
        *nsub = re->re_nsub;
        for (int i=0;i<MAX_GROUPS;i++){
            start[i] = match[i].rm_so;
            len[i] = match[i].rm_eo-match[i].rm_so;
            if (i==re->re_nsub) break;  // last group
        }
    }
    return status;
}

// -----------------------------------------------------------------------------
// extract text - return 0 if successful
// -----------------------------------------------------------------------------
int extract_text(const char * source, char * dest, unsigned int start, 
    unsigned int len, unsigned int max_size){
    int error = 0;
    if ((start+len<max_size)&&(len!=0)){
        memcpy(dest, source+start, len);
        dest[len]=0;    // null termination
    }
    else error = 1;
    return error;
}

// -----------------------------------------------------------------------------
// replace text - return 0 if successful
// -----------------------------------------------------------------------------
int replace_text(const char * source, const char * replace_str, char * dest,
    unsigned int* start, unsigned int* len, unsigned int* nsub){
    return 0;
}
