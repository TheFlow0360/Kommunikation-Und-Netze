/* echoserver.c */

#include <stdlib.h>
#include <stdio.h>
#include <cnaiapi.h>

#define BUFFER_SIZE 1024

int InputEquals( char const aText[], int aTextLength, char const aInput[] )
{
    for ( int i = 0; i < ( aTextLength - 1 ); i++ ) {
        if ( aText[i] != aInput[i] ) {
            return 0;
        }
    }
    return 1;
}

int start( int aPort )
{
    while ( 1 ) {
        printf( "Waiting for connection...\n" );
        connection con = await_contact( aPort );
        if ( -1 == con ) {
            printf( "Error while establishing connection.\n" );
            return 1;
        }
        printf( "Connection with client established.\n" );
        while ( 1 ) {
            char buffer[BUFFER_SIZE];
            int length = read( con, buffer, BUFFER_SIZE );

            if ( -1 == length ) {
                printf( "Error while reading.\n" );
                break;
            } else if ( 0 == length ) {
                printf( "Connection closed by client.\n" );
                break;
            }
            printf( "Received message from client.\n" );

            if ( InputEquals( "shutdown", length, buffer ) ) {
                printf( "Shutdown command received.\n" );
                return 0;
            }

            write( con, buffer, length );
        }

        end_contact(con);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if ( argc < 2 ) {
        printf( "Argument missing: Port.\n" );
        return 1;
    }

    int port = atoi( argv[1] );
    if ( port <= 1024 ) {
        printf( "Invalid Port: %d\n", port );
        return 1;
    }

    return start( port );
}
