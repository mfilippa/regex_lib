// -----------------------------------------------------------------------------
// regex lib - MPF 12/2019
// -----------------------------------------------------------------------------

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "regex_lib.h"

// regex error msg size
#define MAX_ERROR_MSG 128
char regex_err[MAX_ERROR_MSG];

// error strings
char * error_msg[] = {
    "No error",     
    "Error: string not found",
    "Error: error in RegEx string",
    "Error: string size exceeds REGEX_MAX_STR_SIZE",
    "Error: number of subgroups exceeds REGEX_MAX_GROUPS",
    "Error: bad argument",
};

// regex structure
regex_t re;

// -----------------------------------------------------------------------------
// compile regex
// -----------------------------------------------------------------------------
regex_err_t compile_regex(regex_t * re, const char * regex_str) {
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
// search regex
// -----------------------------------------------------------------------------
regex_err_t regex_search(const char * source, const char* regex_search, 
    unsigned int* start, unsigned int* len, unsigned int* nsub){
    regex_err_t err = REGEX_ERR_NONE;
    int status;

    // [0] stores search indexes, [1..REGEX_MAX_GROUPS+1] stores subgroup indexes
    regmatch_t match[REGEX_MAX_GROUPS+1]; 

    // compile regex, run search if successful
    err = compile_regex(&re, regex_search);
    if (err==REGEX_ERR_NONE){
        // execute search, returns 0 if found
        status = regexec(&re, source, REGEX_MAX_GROUPS+1, match, 0);
        if (status==0) {
            // return indexes to start/end of match
            if (re.re_nsub>REGEX_MAX_GROUPS) err = REGEX_ERR_MAX_GROUP;
            else {
                *nsub = re.re_nsub;
                for (int i=0;i<*nsub+1;i++){
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
// extract text
// -----------------------------------------------------------------------------
regex_err_t regex_extract(const char * source, char * dest, 
    unsigned int start, unsigned int len){
    regex_err_t err = REGEX_ERR_NONE;
    if ((len<REGEX_MAX_STR_SIZE-1)&&(len!=0)){
        memcpy(dest, source+start, len);
        dest[len]=0;    // null termination
    }
    else err = REGEX_ERR_BAD_ARG;
    return err;
}

// -----------------------------------------------------------------------------
// replace text
// -----------------------------------------------------------------------------
regex_err_t regex_replace(const char * source, char * dest, 
    const char * srch_regex, const char * rpl_str){
    regex_err_t err = REGEX_ERR_NONE;
    int status;
    unsigned int sub_start[REGEX_MAX_GROUPS+1];
    unsigned int sub_len[REGEX_MAX_GROUPS+1];
    unsigned int nsub, nrep;
    #define CHECK_LEN(v) if (v>=REGEX_MAX_STR_SIZE-1) { \
        err = REGEX_ERR_MAX_STR_LEN; break; }
    // search source string: get all groups start and len
    err = regex_search(source, srch_regex, sub_start, sub_len, &nsub);
    if (err==REGEX_ERR_NONE){
        // perform replacement
        regmatch_t rpl_match;
        unsigned int rpl_idx, dest_idx, len, idx;
        #define TEMP_SIZE 3
        char temp[TEMP_SIZE];
        char sub_regex[] = "\\$[0-9]+";
        compile_regex(&re, sub_regex);  // no need to check for err
        rpl_idx = dest_idx = nrep = 0;
        do {
            status = regexec(&re, rpl_str+rpl_idx, 1, &rpl_match, 0);
            if (status==0) {
                // copy string until $ char
                CHECK_LEN(dest_idx+rpl_match.rm_so);
                memcpy(dest+dest_idx, rpl_str+rpl_idx, rpl_match.rm_so);
                dest_idx += rpl_match.rm_so;
                // match nr to subgroup to insert
                len = rpl_match.rm_eo-rpl_match.rm_so-1;    // minus $ char
                if ((len>0)&&(len<=TEMP_SIZE-1)){
                    memset(temp,0,TEMP_SIZE);
                    memcpy(temp, rpl_str+rpl_idx+rpl_match.rm_so+1, len);
                    idx = atoi(temp);
                    if ((idx>0)&&(idx<=nsub)) {                        
                        // insert subgroup
                        CHECK_LEN(dest_idx+rpl_match.rm_so);
                        memcpy(dest+dest_idx, source+sub_start[idx], 
                            sub_len[idx]);
                        dest_idx += sub_len[idx];                
                        rpl_idx += rpl_match.rm_eo;
                        nrep++;
                    }
                    else err = REGEX_ERR_BAD_REGEX;
                }
                else err = REGEX_ERR_BAD_REGEX;
            } 
            else {
                // last replacement or no replacements
                CHECK_LEN(dest_idx+strlen(rpl_str+rpl_idx))
                memcpy(dest+dest_idx, rpl_str+rpl_idx, 
                    strlen(rpl_str+rpl_idx));
                *(dest+dest_idx+strlen(rpl_str+rpl_idx))=0;
                err = REGEX_ERR_NOT_FOUND;
            }
        } while (err==REGEX_ERR_NONE);
        if (err==REGEX_ERR_NOT_FOUND) err = REGEX_ERR_NONE;
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