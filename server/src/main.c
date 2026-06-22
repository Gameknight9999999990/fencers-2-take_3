#include "../include/sockets.h"
#include "../include/serialization.h"

typedef struct Point {
    OBJECT_SERIALIZABLE
    float x;
    float y;
} Point;

void Point_serialize(Buffer* buffer, const void* obj) {
    const Point* point = (Point*)obj;

    WRITE_FLOAT(buffer, point->x);
    WRITE_FLOAT(buffer, point->y);
}

void Point_deserialize(Buffer* buffer, const size_t addr, void* obj) {
    Point* point = (Point*)obj;

    READ_FLOAT(buffer, addr, &point->x);
    READ_FLOAT(buffer, addr + sizeof(float), &point->y);
}

Point Point_create(float x, float y) {
    Point point = { .x = x, .y = y };
    ATTACH_SERIALIZER(point, Point_serialize);
    ATTACH_DESERIALIZER(point, Point_deserialize);

    return point;
}

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

    Point point = Point_create(0.5f, 10.25f);
    SERIALIZE(&buffer, point);

    FILE* file = fopen("test.bin", "wb");
    fwrite(buffer.raw, sizeof(uint8_t), buffer.size, file);
    fclose(file);

    Point read = Point_create(0.0f, 0.0f);
    DESERIALIZE(&buffer, 0, read);

    printf("%f %f\n", read.x, read.y);

    Buffer_destroy(&buffer);

    return 0;
}