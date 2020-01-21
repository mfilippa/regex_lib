// -----------------------------------------------------------------------------
// regex lib - MPF 12/2019
// -----------------------------------------------------------------------------

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regex_lib.h"

// regex error msg size
#define MAX_ERROR_MSG 128
char regex_err[MAX_ERROR_MSG];

// error strings
char * error_msg[] = {
    "No error",     
    "Error: string not found",
    "Error: error in RegEx string",
    "Error: string size error (REGEX_MAX_STR_SIZE)",
    "Error: number of subgroups error (REGEX_MAX_GROUPS)",
    "Error: bad argument",
};

// regex structure
regex_t re;

// -----------------------------------------------------------------------------
// compile regex
// -----------------------------------------------------------------------------
static regex_err_t compile_regex(regex_t * re, const char * regex_str) {
    regex_err_t err = REGEX_ERR_NONE;
    int status;
    // compile regex, returns 0 if successful
    status = regcomp(re, regex_str, REGEX_COMPILE_FLAGS);
    if (status!=0){
        regerror(status, re, regex_err, MAX_ERROR_MSG);
        printf("Regex compile error, exit code %u: %s\n", status, regex_err);
        err = REGEX_ERR_BAD_REGEX;
    }   
    return err;
}

// -----------------------------------------------------------------------------
// replace text
// -----------------------------------------------------------------------------
// example:
//   src_str  : 'this is a string'
//   src_start: 8  src_len: 1
//   rpl_str  : 'one another more'
//   rpl_start: 4  rpl_len: 7
//   >> dest_str : 'this is another string'
static regex_err_t replace_string( const char * dest_str,
    const char * src_str, unsigned int src_start, unsigned int src_len, 
    const char * rpl_str, unsigned int rpl_start, unsigned int rpl_len) {
    unsigned int len = strlen(src_str)-src_len+rpl_len;
    regex_err_t err = REGEX_ERR_NONE;
    if (len<REGEX_MAX_STR_SIZE) {
        char * dest_idx = (char *) dest_str;
        memcpy(dest_idx, src_str, sizeof(char)*src_start);
        dest_idx += src_start;
        memcpy(dest_idx, rpl_str+rpl_start, sizeof(char)*rpl_len);
        dest_idx += rpl_len;
        *dest_idx = 0;
        strcat(dest_idx, src_str+src_start+src_len);
    }
    else err = REGEX_ERR_MAX_STR_LEN;
    return err;
}

// -----------------------------------------------------------------------------
// search regex
// -----------------------------------------------------------------------------
regex_err_t regex_search(const char * source, const char* regex_search, 
    unsigned int* start, unsigned int* len, unsigned int* src_nsub){
    regex_err_t err = REGEX_ERR_NONE;
    regmatch_t match[REGEX_MAX_GROUPS+1]; 
    int status;

    // compile regex, run search if successful
    err = compile_regex(&re, regex_search);
    if (err==REGEX_ERR_NONE){
        // execute search, returns 0 if found
        status = regexec(&re, source, REGEX_MAX_GROUPS+1, match, 0);
        if (status==0) {
            // return indexes to start/end of match
            if (re.re_nsub>REGEX_MAX_GROUPS) err = REGEX_ERR_MAX_GROUP;
            else {
                *src_nsub = re.re_nsub;
                for (int i=0;i<*src_nsub+1;i++){
                    start[i] = match[i].rm_so;
                    len[i] = match[i].rm_eo-match[i].rm_so;
                    if (i==re.re_nsub) break;  // last group
                }
            }
        }
        else err = REGEX_ERR_NOT_FOUND;
    }
    else err = REGEX_ERR_BAD_REGEX;
    return err;
}

// -----------------------------------------------------------------------------
// extract substring
// -----------------------------------------------------------------------------
regex_err_t regex_extract(char * dest, const char * source,
    unsigned int start, unsigned int len){
    regex_err_t err = REGEX_ERR_NONE;
    if ((len<REGEX_MAX_STR_SIZE)&&(len!=0)){
        memcpy(dest, source+start, len);
        dest[len]=0;    // null termination
    }
    else err = REGEX_ERR_MAX_STR_LEN;
    return err;
}


// -----------------------------------------------------------------------------
// regex replace - no escape chars replaced
// -----------------------------------------------------------------------------
static regex_err_t regex_replace_ne(const char * dest, const char * source,
    const char * srch_regex, const char * rplc_regex){
    regex_err_t err = REGEX_ERR_NONE;
    unsigned int src_start[REGEX_MAX_GROUPS+1];
    unsigned int src_len[REGEX_MAX_GROUPS+1];
    unsigned int src_nsub;
    unsigned int rpl_start[REGEX_MAX_GROUPS+1];
    unsigned int rpl_len[REGEX_MAX_GROUPS+1];
    unsigned int rpl_nsub;

    // search source string: get all groups start and len
    err = regex_search(source, srch_regex, src_start, src_len, &src_nsub);
    if (err==REGEX_ERR_NONE){
        // determine if replacement will use subgroups
        char subg_regex[] = "\\$[0-9]+";
        err = regex_search(rplc_regex, subg_regex, rpl_start, rpl_len, 
            &rpl_nsub);
        if (err==REGEX_ERR_NONE){
            // build replacement string
            unsigned int idx;
            char str[REGEX_MAX_STR_SIZE];
            char tempstr[REGEX_MAX_STR_SIZE];
            strcpy(str, rplc_regex);
            do {
                err = regex_search(str, subg_regex, rpl_start, rpl_len, 
                    &rpl_nsub);
                if (err==REGEX_ERR_NONE){
                    regex_extract(tempstr, str, rpl_start[0]+1, rpl_len[0]-1);
                    idx = atoi(tempstr);
                    if (idx<=src_nsub) {
                        err = replace_string(tempstr, 
                            str, rpl_start[0], rpl_len[0],
                            source, src_start[idx], src_len[idx]);
                        if (err==REGEX_ERR_NONE) strcpy(str, tempstr);
                    }
                    else err = REGEX_ERR_BAD_REGEX;
                }
            } while (err == REGEX_ERR_NONE);
            if (err == REGEX_ERR_NOT_FOUND) {
                // replace string
                err = replace_string(dest, 
                    source, src_start[0], src_len[0],
                    str, 0, strlen(str));
            }
        }
        // else: direct replacement (no subgroups)
        else {
            // replace string
            err = replace_string(dest, 
                source, src_start[0], src_len[0],
                rplc_regex, 0, strlen(rplc_regex));
        }
    }
    return err;
}

// -----------------------------------------------------------------------------
// regex replace 
// -----------------------------------------------------------------------------
regex_err_t regex_replace(const char * dest, const char * source,
    const char * srch_regex, const char * rplc_regex){
    regex_err_t err = REGEX_ERR_NONE;
    char str[REGEX_MAX_STR_SIZE];
    // execute search
    err = regex_replace_ne(dest,source,srch_regex,rplc_regex);
    // replace escaped chars
    if (err==REGEX_ERR_NONE){
        // new line '\n'
        do {
            strcpy(str,dest);
            err = regex_replace_ne(dest,str,"\\\\n", "\n");
        } while (err==REGEX_ERR_NONE);
        // tab '\t'
        do {
            strcpy(str,dest);
            err = regex_replace_ne(dest,str,"\\\\t", "\t");
        } while (err==REGEX_ERR_NONE);
        err = REGEX_ERR_NONE;
    }
    return err;
}

// -----------------------------------------------------------------------------
// get error message
// -----------------------------------------------------------------------------
char* regex_error_msg(regex_err_t err_code){
    char * pstr;
    if (err_code<REGEX_ERR_COUNT){
        pstr = error_msg[err_code];
    }
    else pstr = error_msg[REGEX_ERR_GET_ERR];
    return pstr;
}

