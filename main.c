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

    int status;
    char str[MAX_STR_SIZE];
    unsigned int nsub;
    unsigned int start[MAX_GROUPS];
    unsigned int len[MAX_GROUPS];

    // regex structure
    regex_t re;

    // text string
    char text_str[]="Test string 1234 #test# [NSFW-10] <p></p> end";
    // char text_str[]="line 1 [NSFW-10] test\nline 2 [NSW-35] test\nline 3 [DCEV-257] test";
    // char text_str[]="2019_01_03 filename.txt";
    // char text_str[]="# h1\n## h2\n  - text #bold#\n";
    printf("Text string: '%s'\n", text_str);

    // regex string    
    // char regex_str[] ="string.+?4";
    char regex_str[] ="\\[(.+?)-([0-9]+?)\\]";
    // char regex_str[] ="([0-9]+)_([0-9]+)_([0-9]+)";
    // char regex_str[] ="^## .+";
    printf("Regex string: '%s'\n", regex_str);

    // compile regex
    status = compile_regex(&re, regex_str);
    if (status!=0) exit(1);

    // execute search
    status = search_regex(&re, text_str, start, len, &nsub);
    if (status==0) {
        extract_text(text_str, str, start[0], len[0], MAX_STR_SIZE);
        printf("Match: '%s'\n", str);
        printf("Nr of groups: %u\n", nsub);
        if (nsub!=0){
            for (int i=1;i<nsub+1;i++) {
                extract_text(text_str, str, start[i], len[i], MAX_STR_SIZE);
                printf("Group %u: '%s'\n", i, str);
            }
        }
    }
    else {
        printf("Not found\n");
    }

    return 0;
}