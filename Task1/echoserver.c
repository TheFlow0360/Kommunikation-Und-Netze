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

int start(int aAppId)
{
    int errorCode;
    while ( 1 ) {
        printf( "Waiting for connection...\n" );
        connection con = await_contact( aAppId );
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
                errorCode = 1;
                break;
            } else if ( 0 == length ) {
                printf( "End of stream.\n" );
                continue;
            }
            printf( "Received message from client.\n" );

            if ( InputEquals( "shutdown", length, buffer ) ) {
                printf( "Shutdown command received.\n" );
                errorCode = 0;
                return errorCode;
            }

            length = write( con, buffer, length );

        }

        end_contact(con);
    }
    return errorCode;
}

int main(int argc, char *argv[])
{
    if ( argc < 2 ) {
        printf( "Argument missing.\n" );
        return 1;
    }

    int appId = atoi( argv[1] );
    if ( appId <= 1024 ) {
        printf( "Invalid AppId: %d\n", appId );
        return 1;
    }

    return start( appId );
}
