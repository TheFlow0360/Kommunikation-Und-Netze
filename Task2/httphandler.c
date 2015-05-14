#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "httphandler.h"

#define MASK "\(\[a-zA-Z\]\+\?\)\\s\(\.\+\?\)\\s\(\.\*\?\)\\n\\n"
#define GROUP_COUNT 4

struct Request const parseRequest(const char * const aInput)
{
    struct Request result;

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

        // headers
        length = matches[3].rm_eo - matches[3].rm_so;
        result.path = malloc( length );
        strncpy( result.path, aInput + matches[3].rm_so, length);
    } else {
        printf("\nInvalid Request.");
        result.invalid = 1;
        return result;
    }

    regfree(&regex);

    return result;
}
