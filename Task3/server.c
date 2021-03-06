/* webserver.c */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "connectionhandler.h"
#include "request.h"
#include "httphandler.h"
#include "robot.h"

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


    FILE* fd;

    if ( req.cmdHeartbeat ) {
        sendRobHeartbeat();
        sendHttpResponse( con, 200, STATUS_200 );
        freeRequest( &req );
        end_contact( con );
        return 0;
    } else if ( req.cmdDrive ) {
        float left, right;

        if ( parseDriveCmd( req.path, &left, &right ) != 0 ) {
            printf( "\nInvalid command syntax: %s", req.path );
            sendHttpResponse( con, 400, ERROR_400 );
            freeRequest( &req );
            end_contact( con );
            return 0;
        }
        setRobSpeed( left, right );
        sendHttpResponse( con, 200, STATUS_200 );
        end_contact( con );
        return 0;

    } else if ( req.cmdImage ) {

        //fd =
        end_contact( con );
        return 0;
    } else {

        fd = fopen( req.path, "r" );

        if( !fd )
        {
            printf( "\nCould not open file: %s", req.path );
            sendHttpResponse( con, 403, ERROR_403 );
            freeRequest( &req );
            end_contact( con );
            return 0;
        }

    }

    if( sendHttpResponse( con, 200, STATUS_200 ) ) {
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
    while ( 1 ) {
        connection con = await_contact( aPort );
        if ( -1 == con ) {
            printf( "\nError while establishing connection." );
            continue;
        }

        handleConnection( &con );
    }

    return 0;
}

int main(int argc,char* argv[])
{
	signal(SIGPIPE, SIG_IGN);
	
    if(-1 == initializeRob()) {
        printf("initializeRob()\n");
        exit(1);
    }
	
    if ( argc < 2 ) {
        printf( "\nArgument missing: Port" );
        return 1;
    }

    int port = atoi( argv[1] );
    if ( port <= 1024 ) {
        printf( "\nInvalid Port: %d", port );
        return 1;
    }

    start( port );
	
	// shutdown
	sleep(1);	// required to perform last robot command
	printf("Simulation finished\n");
    shutdownRob();

    return(0);
}

