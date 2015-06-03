#include "connectionhandler.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCKET_NAME "socket"

computer cname_to_comp(char *cname)
{
    return 0; // TODO
}

connection await_contact(appnum a)
{
    static int sockfd;
    static char initialized = 0;

    struct sockaddr_in serv_addr, cli_addr;
    int confd;
    socklen_t clilen;

    if ( !initialized ) {

        initialized = 1;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0) {
            perror("ERROR opening socket");
            return -1;
        }

        bzero( (char *) &serv_addr, sizeof( serv_addr ) );

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons( a );

        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            perror("ERROR on binding");
            return -1;
        }
    }

    listen( sockfd, 5 );
    clilen = sizeof( cli_addr );

    confd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (confd < 0){
        perror("ERROR on accept");
        return -1;
    }

    return confd;
}

connection make_contact(computer c, appnum a)
{
    return 0; // TODO
}

int end_contact(connection c)
{
    if ( close(c) )
        return 0;
    else
        return -1;
}

int get_server_socket(appnum a)
{
    return 0; // TODO
}
