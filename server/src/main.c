#include <stddef.h>
#include "../include/sockets.h"
#include "../include/serialization.h"

typedef struct Point {
    OBJECT_SERIALIZABLE
    uint8_t x;
    uint8_t y;
} Point;

Field point_fields[] = { { FIELDTYPE_INT8, offsetof(Point, x) }, { FIELDTYPE_INT8, offsetof(Point, y) } };

GENETATE_SERIALIZE_FUNC(Point_serialize, point_fields, 2)
GENERATE_DESERIALIZE_FUNC(Point_deserialize, point_fields, 2)

Point Point_create(uint8_t x, uint8_t y) {
    Point point = { .x = x, .y = y };
    ATTACH_SERIALIZER(point, Point_serialize);
    ATTACH_DESERIALIZER(point, Point_deserialize);
    ATTACH_SIZE(point, Point);

    return point;
}

typedef struct Test {
    OBJECT_SERIALIZABLE
    Point p1;
} Test;

Field test_fields[] = { { FIELDTYPE_CUSTOM, offsetof(Test, p1) } };

GENETATE_SERIALIZE_FUNC(Test_serialize, test_fields, 1)
GENERATE_DESERIALIZE_FUNC(Test_deserialize, test_fields, 1)

Test Test_create(Point p1) {
    Test test = { .p1 = p1 };
    ATTACH_SERIALIZER(test, Test_serialize);
    ATTACH_DESERIALIZER(test, Test_deserialize);
    ATTACH_SIZE(test, Test);

    return test;
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

    Buffer buffer = Buffer_create(2048);

    Point point = Point_create(123, 213);
    Test test = Test_create(point);

    SERIALIZE(&buffer, test);

    FILE* file = fopen("test.bin", "wb");
    fwrite(buffer.raw, sizeof(uint8_t), buffer.size, file);
    fclose(file);

    Test read = Test_create(Point_create(0, 0));
    DESERIALIZE(&buffer, 0, read);
    printf("%d %d\n", read.p1.x, read.p1.y);

    Buffer_destroy(&buffer);

    return 0;
}