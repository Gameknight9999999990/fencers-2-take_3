#ifndef FENCERS2_SERVER_SOCKETS_H
#define FENCERS2_SERVER_SOCKETS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

/**
 * @struct Server
 * @brief A UDP server struct
 * 
 * Represents a UDP socket and address used to send packets
 */
typedef struct Server {
    int fd; /**< The fd of the UDP socket */
    struct sockaddr_in addr; /**< The address of the server - ip, port, etc */
} Server;

/**
 * @fn Server_start
 * @brief Starts a UDP server on a port
 * 
 * Opens a UDP server on localhost and a specified port
 * 
 * @param port The port to run the server on
 * @return The newly created and started server
 */
Server Server_start(uint16_t port);

/**
 * @fn Server_stop
 * @brief Stops an existing UDP server
 * 
 * Closes a UDP server and cleans up resources
 * 
 * @param server The existing server to stop
 */
void Server_stop(Server* server);

/**
 * @fn Server_recv
 * @brief Reads a message from the network pipe
 * 
 * Reads any message of a specified size, and fills in the client
 * 
 * @param server The server using to read
 * @param other A pointer to an empty client address that will be filled with the information of the sender
 * @param size The size to read, which will then be filled in with the actual size read
 */
char* Server_recv(Server* server, struct sockaddr_in* other, ssize_t* size);

/**
 * @fn Server_send
 * @brief Writers a message to the network pipe
 * 
 * Writes a message to a specified client
 * 
 * @param server The server using to write
 * @param other The client to write to, must be filled in
 * @param msg The text to write
 */
void Server_send(Server* server, struct sockaddr_in* other, const char* msg);

#define MAX_EVENTS 1024

typedef void (*EventCallback)(Server* server, struct sockaddr_in* other, ssize_t* size);

typedef struct EventLoop {
    int epfd;
    struct epoll_event events[MAX_EVENTS];
    EventCallback callback;
} EventLoop;

EventLoop EventLoop_create(Server* server, EventCallback callback);

void EventLoop_destroy(EventLoop* events);

void EventLoop_read(EventLoop* events, ssize_t* size);

#endif