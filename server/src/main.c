#include <stddef.h>
#include <signal.h>
#include "../include/sockets.h"
#include "../include/serialization.h"

typedef struct Point {
    OBJECT_SERIALIZABLE
    float x;
    float y;
} Point;

Field point_fields[] = { { FIELDTYPE_FLOAT, offsetof(Point, x) }, { FIELDTYPE_FLOAT, offsetof(Point, y) } };

IMPL_SERIALIZE_FUNC(Point_serialize, point_fields, 2)
IMPL_DESERIALIZE_FUNC(Point_deserialize, point_fields, 2)

Point Point_create(float x, float y) {
    Point point = { .x = x, .y = y };
    ATTACH_SERIALIZER(point, Point_serialize);
    ATTACH_DESERIALIZER(point, Point_deserialize);
    ATTACH_SIZE(point, sizeof(x) + sizeof(y));

    return point;
}

typedef struct Test {
    OBJECT_SERIALIZABLE
    Point p1;
    Point p2;
} Test;

Field test_fields[] = { { FIELDTYPE_CUSTOM, offsetof(Test, p1) }, { FIELDTYPE_CUSTOM, offsetof(Test, p2) } };

IMPL_SERIALIZE_FUNC(Test_serialize, test_fields, 2)
IMPL_DESERIALIZE_FUNC(Test_deserialize, test_fields, 2)

Test Test_create(Point p1, Point p2) {
    Test test = { .p1 = p1, .p2 = p2 };
    ATTACH_SERIALIZER(test, Test_serialize);
    ATTACH_DESERIALIZER(test, Test_deserialize);
    ATTACH_SIZE(test, GET_SIZE(p1) + GET_SIZE(p2));

    return test;
}

bool stop = false;

void handle_int(int sig) {
    (void)sig;
    stop = true;
}

void callback(Server* server, struct sockaddr_in* other, ssize_t* size) {
    char* msg = Server_recv(server, other, size);

    Server_send(server, other, msg);

    free(msg);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Must have the port as an argument\n");
        return -1;
    }

    signal(SIGINT, handle_int);

    Server server = Server_start(atoi(argv[1]));
    if (server.fd < 0) {
        return -1;
    }

    EventLoop loop = EventLoop_create(&server, callback);
    
    while (!stop) {
        ssize_t size = 2048;
        
        EventLoop_read(&loop, &size);
    }

    Server_stop(&server);

    Buffer buffer = Buffer_create(2048);

    Point point = Point_create(0.5f, 10.25f);
    Point point2 = Point_create(100.001f, 3.14159f);
    Test test = Test_create(point, point2);

    SERIALIZE(&buffer, test);

    FILE* file = fopen("test.bin", "wb");
    fwrite(buffer.raw, sizeof(uint8_t), buffer.size, file);
    fclose(file);

    Test read = Test_create(Point_create(0, 0), Point_create(0, 0));
    DESERIALIZE(&buffer, 0, read);
    printf("%f %f\n", read.p1.x, read.p1.y);
    printf("%f %f\n", read.p2.x, read.p2.y);

    Buffer_destroy(&buffer);

    return 0;
}