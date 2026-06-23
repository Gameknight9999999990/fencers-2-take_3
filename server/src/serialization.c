#include "../include/serialization.h"

inline uint64_t htonll(uint64_t x) {
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        return ((uint64_t)htonl((uint32_t)(x >> 32))) | ((uint64_t)htonl((uint32_t)x) << 32);
    #else
        return x;
    #endif
}

inline uint64_t ntohll(uint64_t x) {
    return htonll(x);
}

inline float htonf(float x) {
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        uint32_t u;
        memcpy(&u, &x, sizeof(u));

        u = htonl(u);

        float f;
        memcpy(&f, &u, sizeof(f));

        return f;
    #else
        return x;
    #endif
}

inline float ntohf(float x) {
    return htonf(x);
}

inline double htond(double x) {
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        uint64_t u;
        memcpy(&u, &x, sizeof(u));

        u = htonll(u);

        double d;
        memcpy(&d, &u, sizeof(d));

        return d;
    #else
        return x;
    #endif
}

inline double ntohd(double x) {
    return htond(x);
}

Buffer Buffer_create(const size_t cap) {
    Buffer buffer = { .size = 0, .cap = cap };
    buffer.raw = (uint8_t*)malloc(cap);
    if (!buffer.raw) {
        perror("malloc");
        buffer.cap = 0;
    }

    return buffer;
}

void Buffer_destroy(Buffer* buffer) {
    if (!buffer) return;

    if (buffer->raw) {
        free(buffer->raw);
        buffer->raw = NULL;
    }

    buffer->cap = 0;
    buffer->size = 0;
}