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
    // char src_str[]="Test string 1234 #test# [CODE-10] <p>content</p> end";
    // char src_str[]="Multiline line 1 [CODE-10]\nline 2 [CODE-35] test\nline 3 [CODE-257] test";
    // char src_str[]="date 2019_01_03 filename.txt";
    char src_str[]="this is a 50 char long long long long long string";
    // char src_str[]="# h1\n## h2\n  - text #bold#";
    // char src_str[]="This is $1 a sub $2 string $3";
    // char src_str[]="substring test 11-22-33-44-55-66-77-88-99-00-11-22 end";


    // regex string    
    // char srch_regex[] ="string.+?4";
    // char srch_regex[] ="\\[(.+?)-([0-9]+?)\\]";
    // char srch_regex[] ="([0-9]+)_([0-9]+)_([0-9]+)";
    char srch_regex[] ="(long) (long)";
    // char srch_regex[] ="([0-9]+)-([0-9]+)-([0-9]+)-([0-9]+)-([0-9]+)-([0-9]+)-([0-9]+)-([0-9]+)-([0-9]+)-([0-9]+)";
    // char srch_regex[] ="^## .+";
    // char srch_regex[] ="00";
    // char srch_regex[] ="\\$([0-9]+)";

    // replace string
    // char rpl_str[]="Year is $1, month is $2, day is $3";
    char rpl_str[]="This is a $1 $2 string";
    // char rpl_str[]="Sub $1 $2 $3 $4 $5 $6 $7 $8 $9 $10";
    
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