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
    char src_str[]="In the USA, the date today is 12/25/2019";

    // regex string    
    char srch_regex[] ="([0-9]+)/([0-9]+)/([0-9]+)";

    // replace string
    char rpl_str[]="For the rest of the world it is $2/$1/$3. Merry Xmas!";
    
    // search
    printf("Search example -------------------------------\n");
    printf("Source string: '%s'\n", src_str);
    printf("Search regex: '%s'\n", srch_regex);
    error = regex_search(src_str, srch_regex, start, len, &nsub);
    if (error==REGEX_ERR_NONE) {
        regex_extract(src_str, str, start[0], len[0]);
        printf("Match: '%s'\n", str);
        printf("Nr of groups: %u\n", nsub);
        // if (nsub!=0){
        //     for (int i=1;i<nsub+1;i++) {
        //         printf("Group %u start %u len %u\n", i, start[i], len[i]);
        //     }
        // }
    }
    else printf("%s\n", regex_error_msg(error));

    // replace
    printf("Replace example ------------------------------\n");
    printf("Source string: '%s'\n", src_str);
    printf("Search regex: '%s'\n", srch_regex);
    printf("Replacement string: '%s'\n", rpl_str);
    error = regex_replace(src_str, str, srch_regex, rpl_str);
    if (error==REGEX_ERR_NONE){
        printf("Replaced string: '%s'\n", str);
    }
    else printf("%s\n", regex_error_msg(error));

    return 0;
}