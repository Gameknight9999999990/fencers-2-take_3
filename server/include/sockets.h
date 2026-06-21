#ifndef FENCERS2_SERVER_SOCKETS_H
#define FENCERS2_SERVER_SOCKETS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct Server {
    int fd;
    struct sockaddr_in addr;
} Server;

Server Server_start(uint16_t port);

void Server_stop(Server* server);

char* Server_recv(Server* server, struct sockaddr_in* other, ssize_t* size);

void Server_send(Server* server, struct sockaddr_in* other, const char* msg);

#endif