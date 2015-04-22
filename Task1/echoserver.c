/* echoserver.c */

#include <stdlib.h>
#include <stdio.h>
#include <cnaiapi.h>

#define BUFFER_SIZE 1024

int start(int aAppId)
{
    int errorCode;
    printf( "Waiting for connection...\n" );
    connection con = await_contact( aAppId );
    if ( con == -1 ) {
        printf( "Error while establishing connection.\n" );
        return 1;
    }
    printf( "Connection established.\n" );
    while( 1 ) {
        char buffer[BUFFER_SIZE];
        int length = read( con, buffer, BUFFER_SIZE );

        if ( length == -1 ) {
            printf( "Error while reading.\n" );
            errorCode = 1;
            break;
        } else if ( length == 0 ) {
            printf( "End of stream.\n" );
            continue;
        }

        length = write( con, buffer, length );

    }

    end_contact(con);

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
