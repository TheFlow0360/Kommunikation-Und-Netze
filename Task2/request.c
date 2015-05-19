#include "request.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char const isPathValid(struct Request const req)
{
    int count1 = 0;
    int count2 = 0;
    const char* tmp = req.path;
    while( ( tmp = strstr(tmp, "..") ) != 0 )
    {
       count1++;
       tmp++;
    }
    tmp = req.path;
    while( ( tmp = strstr(tmp, "/") ) != 0 )
    {
       count2++;
       tmp++;
    }

    count2-= count1;

    return count2 > 0;
}


void freeRequest(struct Request const req)
{
    free( req.command );
    free( req.path );
    free( req.httpVersion );
    free( req.headers );
}


struct Request newRequest()
{
    struct Request result;
    result.invalid = 0;
    result.command = NULL;
    result.path = NULL;
    result.httpVersion = NULL;
    result.headers = NULL;
    return result;
}


void printRequestData(struct Request const req)
{
    printf("\nCommand: %s\nPath: %s\nHTTP-Version: %s\nAdditional Headers: %s\n\n", req.command, req.path, req.httpVersion, req.headers);
    if ( req.invalid ) {
        printf( "The request is invalid.\n\n" );
    }
}
