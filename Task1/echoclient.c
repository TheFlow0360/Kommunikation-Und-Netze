/* echoclient.c */

#include <cnaiapi.h>
#include <stdlib.h>
#include <stdio.h>

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

int start( int appId )
{
    computer comp = cname_to_comp( "localhost" );
    connection con = make_contact( comp, appId );
    char buffer[BUFFER_SIZE];
    while ( 1 ) {
    printf( "Input:\n" );
        int length = read( 0, buffer, BUFFER_SIZE );

        if ( InputEquals( "exit", length, buffer ) ) {
            break;
        }

        printf( "Sending to server...\n" );
        write( con, buffer, length );
        printf( "Receiving from server...\n" );
        length = read( con, buffer, BUFFER_SIZE );
        if ( -1 == length ) {
            printf( "Error while reading server response. Terminating...\n" );
            return 1;
        } else if ( 0 == length ) {
            printf( "No output returned.\n" );
            continue;
        }

        write( 1, buffer, length );
    }

    return 0;
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
