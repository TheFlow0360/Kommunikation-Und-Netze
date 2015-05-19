/* webserver.c */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>

#include <cnaiapi.h>
#include "request.h"
#include "httphandler.h"

#define BUFFER_SIZE ( 80 * sizeof(char) )
#define REQUEST_END "\r\n\r\n"
#define HTTP_RESPONSE "HTTP/1.0 %d %s" REQUEST_END

char* knownFileNames[] = {
    "",
    "/index.html",
    "/index.htm",
    "/index.php",
    0
};

char* getFullPath(const char* aPath)
{
    char* cwd = malloc( 256 );
    getcwd( cwd, 256 );

    int cwdLength = strlen( cwd );
    int pathLength = strlen( aPath );

    char* file = malloc( cwdLength + pathLength + 1 );
    sprintf( file, "%s%s", cwd, aPath );

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
            return fullFileName;
        }

        // Temporary resource is no longer needed
        free( fullFileName );

        ++fileExtension;
    }

    free( file );
    return 0;

}

void* handleConnection(void* aCon)
{
    connection con = *( (connection*) aCon );
    char* buffer = (char*) malloc( BUFFER_SIZE );

    printf( "\nConnection with client %d established.", con );

    int length = read( con, buffer, BUFFER_SIZE );

    if ( length == -1 ) {
        printf( "\nError while reading." );
        /*
         *  TODO errorcodes
         */
        return 0;
    } else if ( length == BUFFER_SIZE ) {
        int offset = BUFFER_SIZE;
        while ( 1 ) {
            buffer = (char*) realloc(buffer, BUFFER_SIZE + offset);

            int length = read( con, buffer + offset, BUFFER_SIZE );

            if ( length == -1 ) {
                printf( "\nError while reading." );
                /*
                 *  TODO errorcodes
                 */
                return 0;
            } else if ( length != BUFFER_SIZE ) {
                break;
            }
            offset += BUFFER_SIZE;
        }
    }

    printf("\nMessage from client:\n%s\n", buffer);

    struct Request req = parseRequest( buffer );

    free( buffer );

    printf("\nReceived request from client:");
    printRequestData( req );

    if ( req.invalid != 0 ) {
        /*
         *  TODO errorcodes -> 400
         */
        freeRequest(req);
        return 0;
    }
    if ( isPathValid( req ) == 0 ) {
        printf( "\nInvalid path requested: %s", req.path );
        /*
         *  TODO errorcodes -> 403
         */
        freeRequest(req);
        return 0;
    }

    req.path = getFullPath(req.path);

    if ( !req.path ) {
        printf( "\nUnknown file." );
        /*
         *  TODO errorcodes -> 404
         */
        freeRequest(req);
        return 0;
    }

    FILE * fd = fopen( req.path, "r" );

    if( !fd )
    {
        printf( "\nCould not open file." );
        /*
         *  TODO errorcodes -> 403
         */
        freeRequest(req);
        return 0;
    }

    buffer = (char*) malloc( 1024 );
    sprintf(buffer, HTTP_RESPONSE, 200, "OK");

    if( write( con, buffer, strlen( buffer ) ) > 0 );
    {
        int num_bytes = 0;

        char * readbuf = malloc( 1024 );
        while( !feof( fd ) )
        {
            num_bytes = fread( readbuf, 1, 1, fd );
            if( num_bytes < 0 )
                break;

            if( write( con, readbuf, num_bytes ) < 0 )
                break;

        }

        write( con, REQUEST_END, 4 );
        fclose( fd );
        free( readbuf );
    }

    end_contact( con );

    return 0;
}

int start( int aPort )
{
    printf( "\nStarting server..." );

    while ( 1 ) {
        printf( "\nWaiting for connection..." );
        connection con = await_contact( aPort );
        if ( -1 == con ) {
            printf( "\nError while establishing connection." );
            continue;
        }

        pthread_t dummy;
        int err;
        err = pthread_create(&dummy, NULL, handleConnection, &con);
        if (err != 0) {
            /*
             * Inform the client
             */
            printf("\nCan't create thread: [%s]", strerror(err));
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if ( argc < 2 ) {
        printf( "\nArgument missing: Port" );
        return 1;
    }

    int port = atoi( argv[1] );
    if ( port <= 1024 ) {
        printf( "\nInvalid Port: %d", port );
        return 1;
    }

    return start( port );
}
