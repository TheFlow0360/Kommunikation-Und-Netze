#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "httphandler.h"

#define MASK "([[:alpha:]]+)[[:space:]]+([[:graph:]]+?)[[:space:]]+HTTP\\/([[:graph:]]+?)[[:space:]]*(.*?)\r\n\r\n"
#define GROUP_COUNT 5

struct Request const parseRequest(const char * const aInput)
{
    struct Request result = newRequest();

    regex_t regex;
    regmatch_t matches[GROUP_COUNT];

    if ( regcomp( &regex, MASK, REG_EXTENDED ) )
    {
      printf("\nCould not compile regular expression.");
      result.invalid = 1;
      return result;
    };

    if ( regexec( &regex, aInput, GROUP_COUNT, matches, 0 ) == 0 )
    {
        // command
        int length = matches[1].rm_eo - matches[1].rm_so;
        result.command = malloc( length );
        strncpy( result.command, aInput, length );

        // path
        length = matches[2].rm_eo - matches[2].rm_so;
        result.path = malloc( length );
        strncpy( result.path, aInput + matches[2].rm_so, length);

        // http version
        length = matches[3].rm_eo - matches[3].rm_so;
        result.httpVersion = malloc( length );
        strncpy( result.httpVersion, aInput + matches[3].rm_so, length);

        // headers
        length = matches[4].rm_eo - matches[4].rm_so;
        result.headers = malloc( length );
        strncpy( result.headers, aInput + matches[4].rm_so, length);
    } else {
        printf("\nInvalid Request.");
        result.invalid = 1;
        return result;
    }

    regfree(&regex);

    return result;
}
