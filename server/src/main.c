#include "../include/sockets.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Must have the port as an argument\n");
        return -1;
    }

    Server server = Server_start(atoi(argv[1]));
    
    while (1) {
        ssize_t size = 1024;
        
        struct sockaddr_in client;
        char* buf = Server_recv(&server, &client, &size);

        if (strcmp(buf, " ") == 0 || strcmp(buf, "\n") == 0) {
            free(buf);
            break;
        }

        Server_send(&server, &client, buf);

        free(buf);
    }

    Server_stop(&server);

    return 0;
}