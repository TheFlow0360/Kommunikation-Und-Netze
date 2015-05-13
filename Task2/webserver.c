/* webserver.c */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include <cnaiapi.h>
#include "request.h"

#define BUFFER_SIZE ( 80 * sizeof(char) )

void* handleConnection(void* aCon)
{
    connection con = *( (connection*) aCon );
    char* buffer = (char*) malloc( BUFFER_SIZE );

    printf( "\nConnection with client %d established.", con );

    int length = read( con, buffer, BUFFER_SIZE );
    int totalLength = length;

    if ( length == -1 ) {
        printf( "\nError while reading." );
        // TODO errorcodes
        return nullptr;
    } else if ( length == BUFFER_SIZE ) {
        int offset = BUFFER_SIZE;
        while ( 1 ) {
            buffer = (char*) realloc(buffer, BUFFER_SIZE + offset);

            int length = read( con, buffer + offset, BUFFER_SIZE );

            if ( length == -1 ) {
                printf( "\nError while reading." );
                // TODO errorcodes
                return nullptr;
            } else if ( length != BUFFER_SIZE ) {
                totalLength = offset + length;
                break;
            }
            offset += BUFFER_SIZE;
        }
    }

    printf( "\nReceived message from client:\n\t%s", buffer );



    // Check for validity

    // Handle HTTP Request

    write( con, buffer, totalLength );

    end_contact( con );

    free( buffer );

    return nullptr;
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
            // TODO: inform client
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
