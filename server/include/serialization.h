#ifndef FENCERS2_SERVER_SERIALIZATION_H
#define FENCERS2_SERVER_SERIALIZATION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct Buffer {
    uint8_t* raw;
    size_t size;
    size_t cap;
} Buffer;

Buffer Buffer_create(const size_t cap);

void Buffer_destroy(Buffer* buffer);

#define WRITE_8(buffer, val) do {                       \
    Buffer* __buffer_8 = (buffer);                      \
    __buffer_8->raw[__buffer_8->size++] = (uint8_t)val; \
} while (0)

#define READ_8(buffer, addr, val) do {            \
    Buffer* __buffer_8 = (buffer);                \
    size_t __addr_8 = (addr);                     \
    uint8_t* __val_8 = (val);                     \
    memset(__val_8, 0, sizeof(uint8_t));          \
    *__val_8 |= __buffer_8->raw[__addr_8] & 0xFF; \
} while (0)

#define WRITE_16(buffer, val) do {                                               \
    Buffer* __buffer_16 = (buffer);                                              \
    uint16_t __val_16 = (uint16_t)(val);                                         \
    __buffer_16->raw[__buffer_16->size++] = (uint8_t)((__val_16 & 0xFF00) >> 8); \
    __buffer_16->raw[__buffer_16->size++] = (uint8_t)(__val_16 & 0xFF);          \
} while (0)

#define READ_16(buffer, addr, val) do {                                 \
    Buffer* __buffer_16 = (buffer);                                     \
    size_t __addr_16 = (addr);                                          \
    uint16_t* __val_16 = (val);                                         \
    memset(__val_16, 0, sizeof(uint16_t));                              \
    *__val_16 |= ((uint16_t)__buffer_16->raw[__addr_16] << 8) & 0xFF00; \
    *__val_16 |= (uint16_t)__buffer_16->raw[__addr_16+1] & 0xFF;        \
} while (0)

#define WRITE_32(buffer, val) do {                                                    \
    Buffer* __buffer_32 = (buffer);                                                   \
    uint32_t __val_32 = (uint32_t)(val);                                              \
    __buffer_32->raw[__buffer_32->size++] = (uint8_t)((__val_32 & 0xFF000000) >> 24); \
    __buffer_32->raw[__buffer_32->size++] = (uint8_t)((__val_32 & 0xFF0000) >> 16);   \
    __buffer_32->raw[__buffer_32->size++] = (uint8_t)((__val_32 & 0xFF00) >> 8);      \
    __buffer_32->raw[__buffer_32->size++] = (uint8_t)(__val_32 & 0xFF);               \
} while (0)

#define READ_32(buffer, addr, val) do {                                      \
    Buffer* __buffer_32 = (buffer);                                          \
    size_t __addr_32 = (addr);                                               \
    uint32_t* __val_32 = (val);                                              \
    memset(__val_32, 0, sizeof(uint32_t));                                   \
    *__val_32 |= ((uint32_t)__buffer_32->raw[__addr_32] << 24) & 0xFF000000; \
    *__val_32 |= ((uint32_t)__buffer_32->raw[__addr_32+1] << 16) & 0xFF0000; \
    *__val_32 |= ((uint32_t)__buffer_32->raw[__addr_32+2] << 8) & 0xFF00;    \
    *__val_32 |= (uint32_t)__buffer_32->raw[__addr_32+3] & 0xFF;             \
} while (0)

#define WRITE_64(buffer, val) do {                                                            \
    Buffer* __buffer_64 = (buffer);                                                           \
    uint64_t __val_64 = (uint64_t)(val);                                                      \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF00000000000000) >> 56); \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF000000000000) >> 48);   \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF0000000000) >> 40);     \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF00000000) >> 32);       \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF000000) >> 24);         \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF0000) >> 16);           \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF00) >> 8);              \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)(__val_64 & 0xFF);                       \
} while (0)

#define READ_64(buffer, addr, val) do {                                              \
    Buffer* __buffer_64 = (buffer);                                                  \
    size_t __addr_64 = (addr);                                                       \
    uint64_t* __val_64 = (val);                                                      \
    memset(__val_64, 0, sizeof(uint64_t));                                           \
    *__val_64 |= ((uint64_t)__buffer_64->raw[__addr_64] << 56) & 0xFF00000000000000; \
    *__val_64 |= ((uint64_t)__buffer_64->raw[__addr_64+1] << 48) & 0xFF000000000000; \
    *__val_64 |= ((uint64_t)__buffer_64->raw[__addr_64+2] << 40) & 0xFF0000000000;   \
    *__val_64 |= ((uint64_t)__buffer_64->raw[__addr_64+3] << 32) & 0xFF00000000;     \
    *__val_64 |= ((uint64_t)__buffer_64->raw[__addr_64+4] << 24) & 0xFF000000;       \
    *__val_64 |= ((uint64_t)__buffer_64->raw[__addr_64+5] << 16) & 0xFF0000;         \
    *__val_64 |= ((uint64_t)__buffer_64->raw[__addr_64+6] << 8) & 0xFF00;            \
    *__val_64 |= (uint64_t)__buffer_64->raw[__addr_64+7] & 0xFF;                     \
} while (0);

#define WRITE_FLOAT(buffer, val) do {                                                \
    Buffer* __buffer_float = (buffer);                                               \
    float __val_float = (float)(val);                                                \
    memcpy(&__buffer_float->raw[__buffer_float->size], &__val_float, sizeof(float)); \
    __buffer_float->size += sizeof(float);                                           \
} while (0)

#define READ_FLOAT(buffer, addr, val) do {                                  \
    Buffer* __buffer_float = (buffer);                                      \
    size_t __addr_float = (addr);                                           \
    float* __val_float = (val);                                             \
    memcpy(__val_float, &__buffer_float->raw[__addr_float], sizeof(float)); \
} while (0)

#define WRITE_DOUBLE(buffer, val) do {                                                   \
    Buffer* __buffer_double = (buffer);                                                  \
    double __val_double = (double)(val);                                                 \
    memcpy(&__buffer_double->raw[__buffer_double->size], &__val_double, sizeof(double)); \
    __buffer_double->size += sizeof(double); \
} while (0)

#define READ_DOUBLE(buffer, addr, val) do {                                     \
    Buffer* __buffer_double = (buffer);                                         \
    size_t __addr_double = (addr);                                              \
    double* __val_double = (val);                                               \
    memcpy(__val_double, &__buffer_double->raw[__addr_double], sizeof(double)); \
} while (0)

#define WRITE_STR(buffer, val) do {           \
    Buffer* __buffer_str = (buffer);          \
    const char* __val_str = (val);            \
    size_t __size_str = strlen(__val_str);    \
    WRITE_64(__buffer_str, __size_str);       \
    for (size_t i = 0; i < __size_str; i++) { \
        WRITE_8(__buffer_str, __val_str[i]);  \
    }                                         \
} while (0)

#define READ_STR(buffer, addr, val) do {                           \
    Buffer* __buffer_str = (buffer);                               \
    (void)__buffer_str;                                            \
    size_t __addr_str = (addr);                                    \
    size_t __size_str = 0;                                         \
    size_t __start_addr_str = __addr_str;                          \
    READ_64(__buffer_str, __start_addr_str, &__size_str);          \
    char* __val_str = (val);                                       \
    memset(__val_str, 0, __size_str);                              \
    __addr_str = __start_addr_str + sizeof(uint64_t) - 1;          \
    for (size_t i = 0; i < __size_str; i++) {                      \
        __addr_str++;                                              \
        READ_8(__buffer_str, __addr_str, (uint8_t*)&__val_str[i]); \
    }                                                              \
} while (0)

typedef void (*SerializeFunc)(Buffer*, const void*);
typedef void (*DeserializeFunc)(Buffer*, const size_t, void*);

typedef struct SerializeBase {
    SerializeFunc serialize;
    DeserializeFunc deserialize;
} SerializeBase;

#define SERIALIZER_BASE_NAME __serializer_base

#define OBJECT_SERIALIZABLE SerializeBase SERIALIZER_BASE_NAME;

#define ATTACH_SERIALIZER(obj, func) obj.SERIALIZER_BASE_NAME.serialize = func
#define ATTACH_DESERIALIZER(obj, func) obj.SERIALIZER_BASE_NAME.deserialize = func

#define SERIALIZE(buffer, obj) obj.SERIALIZER_BASE_NAME.serialize(buffer, (const void*)&obj)
#define DESERIALIZE(buffer, addr, obj) obj.SERIALIZER_BASE_NAME.deserialize(buffer, addr, (void*)&obj)

#endif