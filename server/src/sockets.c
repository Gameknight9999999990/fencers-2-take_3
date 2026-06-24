#include "../include/sockets.h"

Server Server_start(uint16_t port) {
    Server server;

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

void Server_stop(Server* server) {
    if (!server) return;

    if (server->fd) {
        close(server->fd);
        server->fd = 0;
    }
}

char* Server_recv(Server* server, struct sockaddr_in* other, ssize_t* size) {
    char* buf = (char*)malloc(*size);
    memset(buf, 0, *size);

    socklen_t len = sizeof(*other);
    *size = recvfrom(server->fd, buf, *size, 0, (struct sockaddr*)other, &len);

    return buf;
}

void Server_send(Server* server, struct sockaddr_in* other, const char* msg) {
    sendto(server->fd, msg, strlen(msg), 0, (struct sockaddr*)other, sizeof(*other));
}

EventLoop EventLoop_create(Server* server, EventCallback callback) {
    EventLoop events;
    events.epfd = epoll_create(1);
    events.callback = callback;

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = server;

    epoll_ctl(events.epfd, EPOLL_CTL_ADD, server->fd, &ev);

    return events;
}

void EventLoop_destroy(EventLoop* events) {
    if (!events) return;

    if (events->epfd) {
        close(events->epfd);
    }
    events->epfd = -1;
}

void EventLoop_read(EventLoop* events, ssize_t* size) {
    int num_events = epoll_wait(events->epfd, events->events, MAX_EVENTS, -1);
    
    for (int i = 0; i < num_events; i++) {
        struct sockaddr_in other;
        events->callback(events->events[i].data.ptr, &other, size);
    }
}