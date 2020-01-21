# Regex implementation in C

This code implements basic search, extract, replace functions using 'regex.h' library

More info on regex here:

https://www.regular-expressions.info/posix.html

## Configuration

Configure the following in 'regex_lib.h':

* __REGEX_MAX_STR_SIZE__: this library does not use malloc, it uses a fixed string size. Use this define to set the size of destination strings for all functions
* __REGEX_MAX_GROUPS__: this sets how many parenthesized groups are to be handled by a single regex expression. E.g. '([0-9]+)/([0-9]+)/([0-9]+)' has 3 groups
* __REGEX_COMPILE_FLAGS__: regex flags, see h file for explanation, use search engine if more info is needed. These come from 'regex.h'

## Usage

Add 'regex_lib.c' and 'regex_lib.h' in your project, configure 'regex_lib.h', include 'regex_lib.h' in your C code and call the functions.

Example of search and extraction of data:

```
// source string
char src_str[]="In the USA, the date today is 12/25/2019";

// regex string    
char srch_regex[] ="([0-9]+)/([0-9]+)/([0-9]+)";

// search
error = regex_search(src_str, srch_regex, start, len, &nsub);
if (error==REGEX_ERR_NONE) {
    regex_extract(src_str, str, start[0], len[0]);
    printf("Match: '%s'\n", str);
}
```

If there are subgroups, the start and length of each are stored in 'start[]' and 'len[]' arrays. 'nsub' will indicate how many subgroups were retrieved (size of arrays)

```
// extract subgroups
printf("Nr of groups: %u\n", nsub);
if (nsub!=0) {
    for (int i=1;i<nsub+1;i++)
        printf("Group %u start %u len %u\n", i, start[i], len[i]);
}
```

Example of replace:

```
// replace string
char rpl_str[]="For the rest of the world it is $2/$1/$3. Merry Xmas!";
error = regex_replace(str, src_str, srch_regex, rpl_str);
if (error==REGEX_ERR_NONE){
    printf("Replaced string: '%s'\n", str);
}
```

## Limitations

* This runs on linux only. Need more work to port to windows (http://www.pcre.org/)
* This implements POSIX regular expressions, watch for differences with other implementations, like lack of '\d' and '\D'
* A lot of sanity error checking is missing... just pass the right arguments and you won't have to worry about it (:

