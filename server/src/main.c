#include "../include/sockets.h"
#include "../include/serialization.h"

int main(/*int argc, char* argv[]*/) {
    /*
    if (argc < 2) {
        printf("Must have the port as an argument\n");
        return -1;
    }

    Server server = Server_start(atoi(argv[1]));
    if (server.fd < 0) {
        return -1;
    }
    
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
    */

    Buffer buffer = Buffer_create(1024);

    WRITE_FLOAT(&buffer, 1.25);
    WRITE_DOUBLE(&buffer, 3.14159);

    FILE* file = fopen("test.bin", "wb");
    fwrite(buffer.raw, sizeof(uint8_t), buffer.size, file);
    fclose(file);

    float f_val;
    double d_val;

    READ_FLOAT(&buffer, 0, &f_val);
    READ_DOUBLE(&buffer, sizeof(float), &d_val);

    printf("Float: %f\nDouble: %f\n", f_val, d_val);

    Buffer_destroy(&buffer);

    return 0;
}