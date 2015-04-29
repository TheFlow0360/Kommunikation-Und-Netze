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

int start( int aPort , char* aHost )
{
    computer comp = cname_to_comp( aHost );
    connection con = make_contact( comp, aPort );

    if ( -1 == con ) {
        printf( "Could not connect to server.\n" );
        return 1;
    }

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
        printf( "Argument missing: Port.\n" );
        return 1;
    }

    int port = atoi( argv[1] );
    if ( port <= 1024 ) {
        printf( "Invalid Port: %d\n", port );
        return 1;
    }

    if ( argc < 3 ) {
        return start( port , "localhost" );
    }

    return start( port, argv[2] );
}
