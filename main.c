// -----------------------------------------------------------------------------
// regex test - MPF 12/2019
// -----------------------------------------------------------------------------

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "regex_lib.h"

// -----------------------------------------------------------------------------
// main
// -----------------------------------------------------------------------------
int main(void){

    regex_err_t error;
    char str[REGEX_MAX_STR_SIZE];
    unsigned int start[REGEX_MAX_GROUPS];
    unsigned int len[REGEX_MAX_GROUPS];
    unsigned int nsub;

    // source string
    char src_str[]="This is a string with a date: 12/25/2019";

    // regex string    
    char srch_regex[] ="([0-9]+)/([0-9]+)/([0-9]+)";
    // char srch_regex[] ="01/08/2020";

    // replace string
    char rplc_str[]="$2/$1/$3";
    // char rplc_str[]="25/15/2019";

    // search and extract
    printf("--------- Search and extract example -------------------\n");
    printf("Source string: '%s'\n", src_str);
    printf("Search regex: '%s'\n", srch_regex);
    error = regex_search(src_str, srch_regex, start, len, &nsub);
    if (error==REGEX_ERR_NONE) {
        regex_extract(str, src_str, start[0], len[0]);
        printf("Extracted text: '%s'\n", str);
        printf("Nr of groups: %u\n", nsub);
        for (int i=0;i<nsub+1;i++) {
            printf("Group %u start %u len %u\n", i, start[i], len[i]);
        }
    }
    else printf("%s\n", regex_error_msg(error));

    // replace
    printf("--------- Replace example ------------------------------\n");
    printf("Source string: '%s'\n", src_str);
    printf("Search regex: '%s'\n", srch_regex);
    printf("Replacement string: '%s'\n", rplc_str);
    error = regex_replace(str, src_str, srch_regex, rplc_str);
    if (error==REGEX_ERR_NONE){
        printf("Replaced string: '%s'\n", str);
    }
    else printf("%s\n", regex_error_msg(error));

    return 0;
}