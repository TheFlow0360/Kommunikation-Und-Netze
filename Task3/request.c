#include "request.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char* knownFileNames[] = {
    "",
    "/index.html",
    "/index.htm",
    "/index.php",
    0
};

void freeRequest(struct Request* const req)
{
    free( (*req).command );
    free( (*req).path );
    free( (*req).httpVersion );
    free( (*req).headers );
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

void printRequestData(struct Request* const req)
{
    printf("\nCommand: %s\nPath: %s\nHTTP-Version: %s\nAdditional Headers: %s\n\n", (*req).command, (*req).path, (*req).httpVersion, (*req).headers);
    if ( (*req).invalid ) {
        printf( "The request is invalid.\n\n" );
    }
}

void checkPathValid(struct Request* const req)
{
    int count1 = 0;
    int count2 = 0;
    const char* tmp = (*req).path;
    while( ( tmp = strstr(tmp, "..") ) != 0 )
    {
       count1++;
       tmp++;
    }
    tmp = (*req).path;
    while( ( tmp = strstr(tmp, "/") ) != 0 )
    {
       count2++;
       tmp++;
    }

    count2-= count1;

    (*req).invalid = count2 <= 0;
}

void getFullPath(struct Request* const req)
{
    char* cwd = malloc( 256 );
    getcwd( cwd, 256 );

    int cwdLength = strlen( cwd );
    int pathLength = strlen( (*req).path );

    char* file = malloc( cwdLength + pathLength + 1 );
    sprintf( file, "%s%s", cwd, (*req).path );

    char** fileExtension = knownFileNames;
    while( *fileExtension ) {
        // New file name buffer
        char * fullFileName = malloc( cwdLength + pathLength + strlen( *fileExtension ) + 1 );

        // Combine path + extension
        sprintf( fullFileName, "%s%s", file, *fileExtension );

        // Check if the new path is a file
        struct stat s;
        if( stat( fullFileName, &s ) == 0 && s.st_mode & S_IFREG ) {
            free( file );
            if ( !realloc( (*req).path, strlen( fullFileName ) ) ) {
                (*req).invalid = 1;
                return;
            }
            (*req).path = fullFileName;
            return;
        }

        // Temporary resource is no longer needed
        free( fullFileName );

        ++fileExtension;
    }

    free( file );
    (*req).invalid = 1;
}


void checkCommandValid(struct Request* const req)
{
    if ( strcmp( "GET", (*req).command ) != 0 ) {
        (*req).invalid = 1;
    }
}
