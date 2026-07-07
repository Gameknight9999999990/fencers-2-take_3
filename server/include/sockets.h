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
 * @struct UDPServer
 * @brief A UDP UDPServer struct
 * 
 * Represents a UDP socket and address used to send packets
 */
typedef struct UDPServer {
    int fd; /**< The fd of the UDP socket */
    struct sockaddr_in addr; /**< The address of the UDPServer - ip, port, etc */
} UDPServer;

/**
 * @fn UDPServer_start
 * @brief Starts a UDP UDPServer on a port
 * 
 * Opens a UDP UDPServer on localhost and a specified port
 * 
 * @param port The port to run the UDPServer on
 * @return The newly created and started UDPServer
 */
UDPServer UDPServer_start(uint16_t port);

/**
 * @fn UDPServer_stop
 * @brief Stops an existing UDP UDPServer
 * 
 * Closes a UDP UDPServer and cleans up resources
 * 
 * @param UDPServer The existing UDPServer to stop
 */
void UDPServer_stop(UDPServer* server);

/**
 * @fn UDPServer_recv
 * @brief Reads a message from the network pipe
 * 
 * Reads any message of a specified size, and fills in the client
 * 
 * @param UDPServer The UDPServer using to read
 * @param other A pointer to an empty client address that will be filled with the information of the sender
 * @param size The size to read, which will then be filled in with the actual size read
 */
char* UDPServer_recv(UDPServer* server, struct sockaddr_in* other, ssize_t* size);

/**
 * @fn UDPServer_send
 * @brief Writers a message to the network pipe
 * 
 * Writes a message to a specified client
 * 
 * @param UDPServer The UDPServer using to write
 * @param other The client to write to, must be filled in
 * @param msg The text to write
 */
void UDPServer_send(UDPServer* server, struct sockaddr_in* other, const char* msg);

#define MAX_EVENTS 1024

typedef void (*UDPEventCallback)(UDPServer* server, struct sockaddr_in* other, ssize_t* size);

typedef struct UDPEventLoop {
    int epfd;
    struct epoll_event events[MAX_EVENTS];
    UDPEventCallback callback;
} UDPEventLoop;

UDPEventLoop UDPEventLoop_create(UDPServer* server, UDPEventCallback callback);

void UDPEventLoop_destroy(UDPEventLoop* events);

void UDPEventLoop_read(UDPEventLoop* events, ssize_t* size);

typedef enum TCPEventType {
    EVENT_SERVER,
    EVENT_CLIENT
} TCPEventType;

typedef struct TCPEventData {
    TCPEventType type;
} TCPEventData;

typedef struct TCPServer {
    TCPEventData base;
    int fd;
    struct sockaddr_in addr;
} TCPServer;

typedef struct TCPCon {
    TCPEventData base;
    int fd;
    struct sockaddr_in addr;
    void* data;
} TCPCon;

TCPServer TCPServer_start(uint16_t port);

void TCPServer_stop(TCPServer* server);

TCPCon TCPServer_accept(TCPServer* server);

char* TCPServer_recv(TCPServer* server, TCPCon* con, ssize_t* size);

void TCPServer_send(TCPServer* server, TCPCon* con, const char* msg);

typedef void (*TCPAcceptCallback)(TCPServer* server, int epfd);
typedef void (*TCPRecvCallback)(TCPCon* con, ssize_t* size, int epfd);

typedef struct TCPEventLoop {
    int epfd;
    struct epoll_event events[MAX_EVENTS];
    TCPAcceptCallback accept_callback;
    TCPRecvCallback recv_callback;
} TCPEventLoop;

TCPEventLoop TCPEventLoop_create(TCPServer* server, TCPAcceptCallback accept, TCPRecvCallback recv);

void TCPEventLoop_destroy(TCPEventLoop* events);

void TCPEventLoop_read(TCPEventLoop* events, ssize_t* size);

#endif