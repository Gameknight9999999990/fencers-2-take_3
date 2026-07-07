#include "../include/sockets.h"

UDPServer UDPServer_start(uint16_t port) {
    UDPServer server;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        server.fd = -1;
        return server;
    }

    int flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server.fd = fd;

    server.addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server.addr.sin_port = htons(port);
    server.addr.sin_family = AF_INET;

    if (bind(fd, (struct sockaddr*)&server.addr, sizeof(server.addr)) < 0) {
        perror("bind");
        close(server.fd);
        server.fd = -1;
    }

    return server;
}

void UDPServer_stop(UDPServer* server) {
    if (!server) return;

    if (server->fd) {
        close(server->fd);
        server->fd = 0;
    }
}

char* UDPServer_recv(UDPServer* server, struct sockaddr_in* other, ssize_t* size) {
    char* buf = (char*)malloc(*size);
    memset(buf, 0, *size);

    socklen_t len = sizeof(*other);
    *size = recvfrom(server->fd, buf, *size, 0, (struct sockaddr*)other, &len);

    return buf;
}

void UDPServer_send(UDPServer* server, struct sockaddr_in* other, const char* msg) {
    sendto(server->fd, msg, strlen(msg), 0, (struct sockaddr*)other, sizeof(*other));
}

UDPEventLoop UDPEventLoop_create(UDPServer* server, UDPEventCallback callback) {
    UDPEventLoop events;
    events.epfd = epoll_create(1);
    events.callback = callback;

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = server;

    epoll_ctl(events.epfd, EPOLL_CTL_ADD, server->fd, &ev);

    return events;
}

void UDPEventLoop_destroy(UDPEventLoop* events) {
    if (!events) return;

    if (events->epfd) {
        close(events->epfd);
    }
    events->epfd = -1;
}

void UDPEventLoop_read(UDPEventLoop* events, ssize_t* size) {
    int num_events = epoll_wait(events->epfd, events->events, MAX_EVENTS, -1);
    
    for (int i = 0; i < num_events; i++) {
        struct sockaddr_in other;
        events->callback(events->events[i].data.ptr, &other, size);
    }
}

TCPServer TCPServer_start(uint16_t port) {
    TCPServer server;
    server.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server.fd < 0) {
        perror("socket");
        return server;
    }

    int flags = fcntl(server.fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(server.fd, F_SETFL, flags);

    int opt = 1;
    setsockopt(server.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server.addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server.addr.sin_port = htons(port);
    server.addr.sin_family = AF_INET;

    if (bind(server.fd, (struct sockaddr*)&server.addr, sizeof(server.addr)) < 0) {
        perror("bind");
        close(server.fd);
        server.fd = -1;
        return server;
    }

    if (listen(server.fd, -1) < 0) {
        perror("listen");
        close(server.fd);
        server.fd = -1;
        return server;
    }

    server.base.type = EVENT_SERVER;

    return server;
}

void TCPServer_stop(TCPServer* server) {
    if (!server) return;

    if (server->fd) {
        close(server->fd);
        server->fd = 0;
    }
}

TCPCon TCPServer_accept(TCPServer* server) {
    TCPCon con;
    con.data = NULL;
    socklen_t len = sizeof(struct sockaddr_in);

    con.fd = accept(server->fd, (struct sockaddr*)&con.addr, &len);
    con.base.type = EVENT_CLIENT;

    return con;
}

char* TCPServer_recv(TCPServer* server, TCPCon* con, ssize_t* size) {
    (void)server;
    ssize_t recieved = 0;
    char* buf = (char*)malloc(*size);

    while (recieved < *size) {
        ssize_t cur = recv(con->fd, buf + recieved, *size - recieved, 0);
        if (cur == 0) {
            break;
        } else if (cur < 0) {
            perror("recv");
            break;
        } else {
            recieved += cur;
        }
    }

    *size = recieved;

    return buf;
}

void TCPServer_send(TCPServer* server, TCPCon* con, const char* msg) {
    (void)server;
    ssize_t sent = 0;
    ssize_t size = (ssize_t)strlen(msg);

    while (sent < size) {
        ssize_t cur = send(con->fd, msg + sent, size - sent, 0);
        if (cur == 0) {
            break;
        } else if (cur < 0) {
            perror("recv");
        } else {
            sent += cur;
        }
    }
}

TCPEventLoop TCPEventLoop_create(TCPServer* server, TCPAcceptCallback accept, TCPRecvCallback recv) {
    TCPEventLoop events;
    events.epfd = epoll_create(1);
    events.accept_callback = accept;
    events.recv_callback = recv;

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = server;

    epoll_ctl(events.epfd, EPOLL_CTL_ADD, server->fd, &ev);

    return events;
}

void TCPEventLoop_destroy(TCPEventLoop* events) {
    if (!events) return;

    if (events->epfd) {
        close(events->epfd);
    }
    events->epfd = -1;
}

void TCPEventLoop_read(TCPEventLoop* events, ssize_t* size) {
    int num_events = epoll_wait(events->epfd, events->events, MAX_EVENTS, -1);
    
    for (int i = 0; i < num_events; i++) {
        TCPEventData* data = events->events[i].data.ptr;

        switch (data->type) {
            case EVENT_SERVER:
                events->accept_callback((TCPServer*)data, events->epfd);
                break;
            case EVENT_CLIENT:
                events->recv_callback((TCPCon*)data, size, events->epfd);
                break;
        }
    }
}