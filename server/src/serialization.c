#include "../include/serialization.h"

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