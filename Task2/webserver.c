/* webserver.c */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include <cnaiapi.h>
#include "request.h"
#include "httphandler.h"

#define STATUS_200 "OK"
#define ERROR_400 "Bad Request"
#define ERROR_403 "Forbidden"
#define ERROR_404 "Not Found"
#define ERROR_405 "Method not allowed"
#define ERROR_500 "Internal Server Error"

void* handleConnection(void* aCon)
{
    connection con = *( (connection*) aCon );

    printf( "\nConnection with client %d established.", con );

    char* buffer;
    if ( readHttpRequest( con, &buffer ) != 0 ) {
        //printf( "\nError reading message." );
        sendHttpResponse( con, 500, ERROR_500 );
        end_contact( con );
        return 0;
    }

    struct Request req = parseRequest( buffer );

    printf( "\nReceived request from client:" );
    printRequestData( &req );

    if ( req.invalid != 0 ) {
        printf( "\nInvalid request: %s", buffer );
        sendHttpResponse( con, 400, ERROR_400 );
        freeRequest( &req );
        end_contact( con );
        return 0;
    }

    free( buffer );

    checkCommandValid( &req );

    if ( req.invalid != 0 ) {
        printf( "\nInvalid command: %s", req.command );
        sendHttpResponse( con, 405, ERROR_405 );
        freeRequest( &req );
        end_contact( con );
        return 0;
    }

    checkPathValid( &req );

    if ( req.invalid != 0 ) {
        printf( "\nInvalid path requested: %s", req.path );
        sendHttpResponse( con, 403, ERROR_403 );
        freeRequest( &req );
        end_contact( con );
        return 0;
    }

    getFullPath( &req );

    if ( req.invalid != 0 ) {
        printf( "\nUnknown file: %s", req.path );
        sendHttpResponse( con, 404, ERROR_404 );
        freeRequest( &req );
        end_contact( con );
        return 0;
    }

    FILE * fd = fopen( req.path, "r" );

    if( !fd )
    {
        printf( "\nCould not open file: %s", req.path );
        sendHttpResponse( con, 403, ERROR_403 );
        freeRequest( &req );
        end_contact( con );
        return 0;
    }

    if( sendHttpResponse( con, 200, STATUS_200 ) );
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

        write( con, "\r\n\r\n", 4 );
        fclose( fd );
        free( readbuf );
    }

    printf( "\nClosing Connection %d", con);
    end_contact( con );

    return 0;
}

int start( int aPort )
{
    signal(SIGPIPE, SIG_IGN);
    printf( "\nServer ready.");

    while ( 1 ) {
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
