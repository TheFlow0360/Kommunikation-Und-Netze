#include "httphandler.h"

#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MASK "([[:alpha:]]+)[[:space:]]+([[:graph:]]+?)[[:space:]]+HTTP\\/([[:graph:]]+?)[[:space:]]*(.*?)\r\n\r\n"
#define GROUP_COUNT 5

#define BUFFER_SIZE ( 64 * sizeof(char) )

#define REQUEST_END "\r\n\r\n"
#define HTTP_RESPONSE_HEAD "HTTP/1.0 %d %s" REQUEST_END
#define HTTP_RESPONSE_CONTENT "<html><body><h1>%d %s</h1></body></html>" REQUEST_END

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
        result.command = malloc( length + 1 );
        strncpy( result.command, aInput, length );
        result.command[ length ] = '\0';

        // path
        length = matches[2].rm_eo - matches[2].rm_so;
        result.path = malloc( length + 1 );
        strncpy( result.path, aInput + matches[2].rm_so, length);
        result.path[ length ] = '\0';

        // http version
        length = matches[3].rm_eo - matches[3].rm_so;
        result.httpVersion = malloc( length + 1 );
        strncpy( result.httpVersion, aInput + matches[3].rm_so, length);
        result.httpVersion[ length ] = '\0';

        // headers
        length = matches[4].rm_eo - matches[4].rm_so;
        result.headers = malloc( length + 1 );
        strncpy( result.headers, aInput + matches[4].rm_so, length);
        result.headers[ length ] = '\0';
    } else {
        result.invalid = 1;
        return result;
    }

    regfree(&regex);

    return result;
}

char const sendHttpResponse(connection aCon, unsigned int const aCode, char const* aText)
{
    char* buffer = (char*) malloc( 1024 );
    sprintf( buffer, HTTP_RESPONSE_HEAD, aCode, aText );

    if ( aCode != 200 ) {
        sprintf( buffer + strlen( buffer ), HTTP_RESPONSE_CONTENT, aCode, aText );
    }

    return write( aCon, buffer, strlen( buffer ) ) > 0;
}


char readHttpRequest(connection aCon, char** const aBuffer)
{
    int totalLength = 0;
    int totalRead = 0;
    int length = 0;

    *aBuffer = (char*) malloc( BUFFER_SIZE );

    do {
        totalLength += BUFFER_SIZE;
        *aBuffer = (char*) realloc( *aBuffer, totalLength);

        length = read( aCon, *aBuffer + totalRead, totalLength - totalRead - 1 );

        if ( length == -1 ) {
            break;
        }

        (*aBuffer)[ totalRead + length ] = '\0';
        char* posEnd = strstr( *aBuffer, REQUEST_END );
        if( posEnd ) {
            posEnd += strlen( REQUEST_END ) + 1;
            *posEnd = '\0';
            return 0;
        }

        totalRead += length;
    } while( length > 0 && totalRead <= 4096 );

    free( *aBuffer );

    return -1;
}
