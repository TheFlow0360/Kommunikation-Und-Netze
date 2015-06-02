#include "connectionhandler.h"

#include <sys/types.h>
#include <sys/socket.h>
 #include <sys/un.h>
#include <stdio.h>

#define SOCKET_NAME "socket"

computer cname_to_comp(char *cname)
{
    return 0; // TODO
}

connection await_contact(appnum a)
{
    static int server_socket;
    static char initialized = 0;
    struct sockaddr_un server;

    if ( !initialized ) {
        unlink(SOCKET_NAME);

        server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
        if (server_socket < 0) {
            perror("opening stream socket");
            return -1;
        }

        server.sun_family = AF_UNIX;
        strcpy(server.sun_path, SOCKET_NAME);
        if (bind(server_socket, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
            perror("binding stream socket");
            return -1;
        }
    }

    listen(server_socket, 5);
    return accept(server_socket, 0, 0);
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
