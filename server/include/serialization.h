#ifndef FENCERS2_SERVER_SERIALIZATION_H
#define FENCERS2_SERVER_SERIALIZATION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <arpa/inet.h>

/**
 * @fn htonll
 * @brief Converts a little endian uint64_t into big endian order
 * 
 * Extension of inet.h functions for larger types
 * 
 * @param x The value to swap into big endian
 * @return The big endian value of x
 */
uint64_t htonll(uint64_t x);

/**
 * @fn ntohll
 * @brief Converts a big endian uint64_t into little endian order
 * 
 * Extension of inet.h functions for larger types
 * 
 * @param x The value to swap into little endian
 * @return The little endian value of x
 */
uint64_t ntohll(uint64_t x);

/**
 * @fn htons
 * @brief Converts a little endian float into big endian order
 * 
 * Extension of inet.h functions for larger types
 * 
 * @param x The value to swap into big endian
 * @return The big endian value of x
 */
float htonf(float x);

/**
 * @fn ntohf
 * @brief Converts a big endian float into little endian order
 * 
 * Extension of inet.h functions for larger types
 * 
 * @param x The value to swap into little endian
 * @return The little endian value of x
 */
float ntohf(float x);

/**
 * @fn htond
 * @brief Converts a little endian double into big endian order
 * 
 * Extension of inet.h functions for larger types
 * 
 * @param x The value to swap into big endian
 * @return The big endian value of x
 */
double htond(double x);

/**
 * @fn ntohd
 * @brief Converts a big endian double into little endian order
 * 
 * Extension of inet.h functions for larger types
 * 
 * @param x The value to swap into little endian
 * @return The little endian value of x
 */
double ntohd(double x);

/**
 * @struct Buffer
 * @brief Represents a buffer of binary values
 * 
 * A dynamic array of bytes used for serialization and deserialization
 */
typedef struct Buffer {
    uint8_t* raw; /**< Raw bytes written and read */
    size_t size; /**< Amount of bytes stored */
    size_t cap; /**< Max bytes stored */
} Buffer;

/**
 * @fn Buffer_create
 * @brief Creates an instance of the Buffer struct
 * 
 * Allocates a buffer based on a given size
 * 
 * @warning Must destroy the buffer or memory will be leaked
 * @see Buffer_destroy
 * 
 * @param cap The capacity of the buffer created
 * @return The newly created buffer
 */
Buffer Buffer_create(const size_t cap);

/**
 * @fn Buffer_destroy
 * @brief Destroys an existing buffer
 * 
 * Cleans up a buffer to not leak resources
 * 
 * @param buffer The buffer to be destroyed
 */
void Buffer_destroy(Buffer* buffer);

/**
 * @def WRITE_8
 * @brief Writes an 8 bit value to a buffer
 * 
 * Copies an 8 bit value - char, uint8_t, int8_t, etc - into a buffer
 * 
 * @param buffer A pointer to a buffer to write to
 * @param val The 8 bit value to write
 */
#define WRITE_8(buffer, val) do {                       \
    Buffer* __buffer_8 = (buffer);                      \
    __buffer_8->raw[__buffer_8->size++] = (uint8_t)(val); \
} while (0)

/**
 * @def READ_8
 * @brief Reads an 8 bit value from a buffer
 * 
 * Copies an 8 bit value - char, uint8_t, int8_t, etc - into a value from a buffer
 * 
 * @param buffer A pointer to a buffer to read from
 * @param addr The start address to read from
 * @param val A pointer to the value to write to
 */
#define READ_8(buffer, addr, val) do {            \
    Buffer* __buffer_8 = (buffer);                \
    size_t __addr_8 = (addr);                     \
    uint8_t* __val_8 = (val);                     \
    memset(__val_8, 0, sizeof(uint8_t));          \
    *__val_8 |= __buffer_8->raw[__addr_8] & 0xFF; \
} while (0)

/**
 * @def WRITE_16
 * @brief Writes a 16 bit value to a buffer
 * 
 * Copies a 16 bit value into a buffer
 * 
 * @param buffer A pointer to a buffer to write to
 * @param val The 16 bit value to write
 */
#define WRITE_16(buffer, val) do {                                               \
    Buffer* __buffer_16 = (buffer);                                              \
    uint16_t __val_16 = htons((uint16_t)(val));                                  \
    __buffer_16->raw[__buffer_16->size++] = (uint8_t)((__val_16 & 0xFF00) >> 8); \
    __buffer_16->raw[__buffer_16->size++] = (uint8_t)(__val_16 & 0xFF);          \
} while (0)

/**
 * @def READ_16
 * @brief Reads a 16 bit value from a buffer
 * 
 * Copies a 16 bit value into a value from a buffer
 * 
 * @param buffer A pointer to the buffer to read from
 * @param addr The start address to read from
 * @param val A pointer to the 16 bit value to write to
 */
#define READ_16(buffer, addr, val) do {                                 \
    Buffer* __buffer_16 = (buffer);                                     \
    size_t __addr_16 = (addr);                                          \
    uint16_t* __val_16 = (val);                                         \
    memset(__val_16, 0, sizeof(uint16_t));                              \
    *__val_16 |= ((uint16_t)__buffer_16->raw[__addr_16] << 8) & 0xFF00; \
    *__val_16 |= (uint16_t)__buffer_16->raw[__addr_16+1] & 0xFF;        \
    *__val_16 = ntohl(*__val_16);                                       \
} while (0)

/**
 * @def WRITE_32
 * @brief Writes a 32 bit value into a buffer
 * 
 * Copies a 32 bit value into a buffer
 * 
 * @param buffer A pointer to the buffer to write to
 * @param val The 32 bit value to write
 */
#define WRITE_32(buffer, val) do {                                                    \
    Buffer* __buffer_32 = (buffer);                                                   \
    uint32_t __val_32 = htonl((uint32_t)(val));                                       \
    __buffer_32->raw[__buffer_32->size++] = (uint8_t)((__val_32 & 0xFF000000) >> 24); \
    __buffer_32->raw[__buffer_32->size++] = (uint8_t)((__val_32 & 0xFF0000) >> 16);   \
    __buffer_32->raw[__buffer_32->size++] = (uint8_t)((__val_32 & 0xFF00) >> 8);      \
    __buffer_32->raw[__buffer_32->size++] = (uint8_t)(__val_32 & 0xFF);               \
} while (0)

/**
 * @def READ_32
 * @brief Reads a 32 bit value from a buffer
 * 
 * Copies a 32 bit value from a buffer
 * 
 * @param buffer A pointer to the buffer to read from
 * @param addr The start address to read from
 * @param val A pointer to the 32 bit value to write to
 */
#define READ_32(buffer, addr, val) do {                                      \
    Buffer* __buffer_32 = (buffer);                                          \
    size_t __addr_32 = (addr);                                               \
    uint32_t* __val_32 = (val);                                              \
    memset(__val_32, 0, sizeof(uint32_t));                                   \
    *__val_32 |= ((uint32_t)__buffer_32->raw[__addr_32] << 24) & 0xFF000000; \
    *__val_32 |= ((uint32_t)__buffer_32->raw[__addr_32+1] << 16) & 0xFF0000; \
    *__val_32 |= ((uint32_t)__buffer_32->raw[__addr_32+2] << 8) & 0xFF00;    \
    *__val_32 |= (uint32_t)__buffer_32->raw[__addr_32+3] & 0xFF;             \
    *__val_32 = ntohl(*__val_32);                                            \
} while (0)

/**
 * @def WRITE_64
 * @brief Writes a 64 bit value to a buffer
 * 
 * Copies a 64 bit value into a buffer
 * 
 * @param buffer A pointer to the buffer to write to
 * @param val The 64 bit value to write
 */
#define WRITE_64(buffer, val) do {                                                            \
    Buffer* __buffer_64 = (buffer);                                                           \
    uint64_t __val_64 = htonll((uint64_t)(val));                                              \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF00000000000000) >> 56); \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF000000000000) >> 48);   \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF0000000000) >> 40);     \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF00000000) >> 32);       \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF000000) >> 24);         \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF0000) >> 16);           \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)((__val_64 & 0xFF00) >> 8);              \
    __buffer_64->raw[__buffer_64->size++] = (uint8_t)(__val_64 & 0xFF);                       \
} while (0)

/**
 * @def READ_64
 * @brief Reads a 64 bit value from a buffer
 * 
 * Copies a 64 bit value from a buffer
 * 
 * @param buffer A pointer to the buffer to read from
 * @param addr The start address to read from
 * @param val A pointer to the 64 bit value to write to
 */
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
    *__val_64 |= ntohll(*__val_64);                                                  \
} while (0);

/**
 * @def WRITE_FLOAT
 * @brief Writes a float to a buffer
 * 
 * Copies a floating point into a buffer
 * 
 * @param buffer A pointer to the buffer to write to
 * @param val The float to write
 */
#define WRITE_FLOAT(buffer, val) do {                                                \
    Buffer* __buffer_float = (buffer);                                               \
    float __val_float = htonf((float)(val));                                         \
    memcpy(&__buffer_float->raw[__buffer_float->size], &__val_float, sizeof(float)); \
    __buffer_float->size += sizeof(float);                                           \
} while (0)

/**
 * @def READ_FLOAT
 * @brief Reads a float from a buffer
 * 
 * Copies a floating point from a buffer
 * 
 * @param buffer A pointer to the buffer to read from
 * @param addr The start address to read from
 * @param val A pointer to the float to write to
 */
#define READ_FLOAT(buffer, addr, val) do {                                  \
    Buffer* __buffer_float = (buffer);                                      \
    size_t __addr_float = (addr);                                           \
    float* __val_float = (val);                                             \
    memcpy(__val_float, &__buffer_float->raw[__addr_float], sizeof(float)); \
    *__val_float = ntohf(*__val_float);                                     \
} while (0)

/**
 * @def WRITE_DOUBLE
 * @brief Writes a double to a buffer
 * 
 * Copies a double into a buffer
 * 
 * @param buffer A pointer to the buffer to write to
 * @param val The double to write into the buffer
 */
#define WRITE_DOUBLE(buffer, val) do {                                                   \
    Buffer* __buffer_double = (buffer);                                                  \
    double __val_double = htond((double)(val));                                          \
    memcpy(&__buffer_double->raw[__buffer_double->size], &__val_double, sizeof(double)); \
    __buffer_double->size += sizeof(double); \
} while (0)

/**
 * @def READ_DOUBLE
 * @brief Reads a double from a buffer
 * 
 * Copies a double from a buffer
 * 
 * @param buffer A pointer to the buffer to read from
 * @param addr The start address to read from
 * @param val A pointer to the double to write to
 */
#define READ_DOUBLE(buffer, addr, val) do {                                     \
    Buffer* __buffer_double = (buffer);                                         \
    size_t __addr_double = (addr);                                              \
    double* __val_double = (val);                                               \
    memcpy(__val_double, &__buffer_double->raw[__addr_double], sizeof(double)); \
    *__val_double = ntohd(*__val_double);                                       \
} while (0)

/**
 * @def WRITE_STR
 * @brief Writes a string into a buffer
 * 
 * Copies a string into a buffer - this includes size at the beginning
 * 
 * @param buffer A pointer to the buffer to write to
 * @param val The string to write
 */
#define WRITE_STR(buffer, val) do {           \
    Buffer* __buffer_str = (buffer);          \
    const char* __val_str = (val);            \
    size_t __size_str = strlen(__val_str);    \
    WRITE_64(__buffer_str, __size_str);       \
    for (size_t i = 0; i < __size_str; i++) { \
        WRITE_8(__buffer_str, __val_str[i]);  \
    }                                         \
} while (0)

/**
 * @def READ_STR
 * @brief Reads a string from a buffer
 * 
 * Copies a string from a buffer
 * 
 * @warning The value must be large enough to hold the string - you can find the size of the string by using READ_64 on the same address
 * 
 * @param buffer A pointer to the buffer to read from
 * @param addr The start address
 * @param val The char* to write to
 */
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

/**
 * @typedef SerializeFunc
 * @brief A generic serializer function
 * 
 * A serializer function pointer for any custom structs
 * 
 * @param buffer The buffer to write to
 * @param obj A void* of the object being serialized
 */
typedef void (*SerializeFunc)(Buffer* buffer, void* obj);

/**
 * @typedef DeserializeFunc
 * @brief A generic deserializer function
 * 
 * A deserializer function pointer for any custom structs
 * 
 * @param buffer The buffer to read from
 * @param addr The start address
 * @param obj A void* of the object being written to
 */
typedef void (*DeserializeFunc)(Buffer* buffer, const size_t addr, void* obj);

/**
 * @struct SerializeBase
 * @brief A base vtable for serializable objects
 * 
 * A simple base vtable for objects that are serializable and deserializable. Should not be touched by the user
 */
typedef struct SerializeBase {
    SerializeFunc serialize; /**< The function used to serialize */
    DeserializeFunc deserialize; /**< The function used to deserialize */
    size_t size; /**< The total size of the object */
} SerializeBase;

/**
 * @def SERIALIZER_BASE_NAME
 * @brief The name of the serializer base instance in structs
 */
#define SERIALIZER_BASE_NAME __serializer_base

/**
 * @def OBJECT_SERIALIZABLE
 * @brief Adds the serializer base into a struct
 */
#define OBJECT_SERIALIZABLE SerializeBase SERIALIZER_BASE_NAME;

/**
 * @def ATTACH_SERIALIZER
 * @brief Attaches a serializer function to an object
 * 
 * @warning Must be used on every instance of the object or else a segfault will occur if the objects are serialized/deserialzed
 * 
 * @param obj An object to have a serializer attached to
 * @param func A function pointer of type SerializeFunc that is used to serialize the object
 */
#define ATTACH_SERIALIZER(obj, func) (obj).SERIALIZER_BASE_NAME.serialize = func

/**
 * @def ATTACH_DESERIALIZER
 * @brief Attaches a deserializer function to an object
 * 
 * @warning Must be used on every instance of the object or else a segfault will occur if the objects are serialized/deserialzed
 * 
 * @param obj An object to have a deserializer attached to
 * @param func A function pointer of type DeserializeFunc that is used to deserialize the object
 */
#define ATTACH_DESERIALIZER(obj, func) (obj).SERIALIZER_BASE_NAME.deserialize = func

/**
 * @def ATTACH_SIZE
 * @brief Attaches an objects size to itself
 * 
 * @warning Must be used if the object is contained in other objects that are serialized/deserialized
 * 
 * @param obj An object to have its size attached to
 * @param obj_size A sum of all the objects sizes, gotten with sizeof() if it is a primitive, otherwise with GET_SIZE
 */
#define ATTACH_SIZE(obj, obj_size) (obj).SERIALIZER_BASE_NAME.size = (obj_size)

/**
 * @def GET_SIZE
 * @brief Gets an objects size
 * 
 * @warning This is not equivilent to sizeof, the size should be the sum of the sizes of the data - not including the serializer base and padding
 * 
 * @param obj The object to read size from
 */
#define GET_SIZE(obj) ((obj).SERIALIZER_BASE_NAME.size)

/**
 * @def SERIALIZE
 * @brief Serializes a custom struct
 * 
 * @warning The struct must have a serializer base that is valid
 * 
 * Calls an objects serialize function pointer
 * 
 * @param buffer THe buffer to write to
 * @param obj The object to serialize
 */
#define SERIALIZE(buffer, obj) do { \
    void* __obj_ptr = (void*)(&(obj)); \
    ((SerializeBase*)(__obj_ptr))->serialize(buffer, __obj_ptr); \
} while (0)

/**
 * @def DESERIALIZE
 * @brief Deserializes a custom struct
 * 
 * @warning The struct must have a serializer base that is valid
 * 
 * Calls an objects deserialize function pointer
 * 
 * @param buffer The buffer to read from
 * @param addr The start address
 * @param obj The object to write to - is not a pointer
 */
#define DESERIALIZE(buffer, addr, obj) do { \
    void* __obj_ptr = (void*)(&(obj)); \
    ((SerializeBase*)(__obj_ptr))->deserialize(buffer, addr, __obj_ptr); \
} while (0)

/**
 * @def FIELD_TYPE_LIST
 * @brief All the supported field types
 * 
 * Includes primitives of fixed-width ints, floats, doubles, strings, and custom structs
 */
#define FIELD_TYPE_LIST(X) \
    X(INT8) /**< 8 bit int - signed or unsigned */ \
    X(INT16) /**< 16 bit int - signed or unsigned */ \
    X(INT32) /**< 32 bit int - signed or unsigned */ \
    X(INT64) /**< 64 bit int - signed or unsigned */ \
    X(FLOAT) /**< floating point */ \
    X(DOUBLE) /**< double width floating point */ \
    X(STR) /**< char* string */ \
    X(CUSTOM) /**< custom structs */

/**
 * @enum FieldTypes
 * @brief An enum of the field types supported for serialization and deserialization
 * 
 * Used when creating a description of custom structs for member types
 */
typedef enum FieldTypes {
    // Use X macro
    #define X(type) FIELDTYPE_##type,
    FIELD_TYPE_LIST(X)
    #undef X
} FieldTypes;

/**
 * @struct Field
 * @brief A description of a member of a struct
 * 
 * Describes a single member variable of a struct
 */
typedef struct Field {
    FieldTypes type; /**< The type of the field */
    size_t offset; /**< The offset of the field, found with offsetof */
} Field;

/**
 * @def DECLARE_SERIALIZE_FUNC
 * @brief Declares a serialization function with a name
 * 
 * Declares a function of a certain signature, to be used in header files
 * 
 * @warning Does not implement the function
 * 
 * @param name The name of the function to declare
 */
#define DECLARE_SERIALIZE_FUNC(name) void name(Buffer* buffer, void* obj);

/**
 * @def IMPL_SERIALIZE_FUNC
 * @brief Implements a serialization function with a name
 * 
 * Implements a serialization function of a certain signature, to be used in source files
 * 
 * @param name The name of the function to implement
 * @param fields An array of fields describing a struct
 * @param num_fields The amount of fields given
 */
#define IMPL_SERIALIZE_FUNC(name, fields, num_fields) void name(Buffer* buffer, void* obj) { \
    for (size_t i = 0; i < (num_fields); i++) { \
        switch (fields[i].type) { \
            case FIELDTYPE_INT8: \
                WRITE_8(buffer, *((uint8_t*)((char*)obj + fields[i].offset))); \
                break; \
            case FIELDTYPE_INT16: \
                WRITE_16(buffer, *((uint16_t*)((char*)obj + fields[i].offset))); \
                break; \
            case FIELDTYPE_INT32: \
                WRITE_32(buffer, *((uint32_t*)((char*)obj + fields[i].offset))); \
                break; \
            case FIELDTYPE_INT64: \
                WRITE_64(buffer, *((uint64_t*)((char*)obj + fields[i].offset))); \
                break; \
            case FIELDTYPE_FLOAT: \
                WRITE_FLOAT(buffer, *((float*)((char*)obj + fields[i].offset))); \
                break; \
            case FIELDTYPE_DOUBLE: \
                WRITE_DOUBLE(buffer, *((double*)((char*)obj + fields[i].offset))); \
                break; \
            case FIELDTYPE_STR: \
                WRITE_STR(buffer, (char*)((char*)obj + fields[i].offset)); \
                break; \
            case FIELDTYPE_CUSTOM: ; \
                SerializeBase* base = (SerializeBase*)((char*)obj + fields[i].offset); \
                SerializeFunc func = base->serialize; \
                void* ptr = (void*)((char*)obj + fields[i].offset); \
                func(buffer, ptr); \
                break; \
        } \
    } \
}

/**
 * @def DECLARE_DESERIALIZE_FUNC
 * @brief Declares a deserialization function with a name
 * 
 * Declares a function of a certain signature, to be used in header files
 * 
 * @warning Does not implement the function
 * 
 * @param name The name of the function to declare
 */
#define DECLARE_DESERIALIZE_FUNC(name) void name(Buffer* buffer, const size_t addr, void* obj);

/**
 * @def IMPL_DESERIALIZE_FUNC
 * @brief Implements a deserialization function with a name
 * 
 * Implements a deserialization function of a certain signature, to be used in source files
 * 
 * @param name The name of the function to implement
 * @param fields An array of fields describing a struct
 * @param num_fields The amount of fields given
 */
#define IMPL_DESERIALIZE_FUNC(name, fields, num_fields) void name(Buffer* buffer, const size_t addr, void* obj) { \
    size_t curr = addr; \
    for (size_t i = 0; i < num_fields; i++) { \
        switch (fields[i].type) { \
            case FIELDTYPE_INT8: \
                READ_8(buffer, curr, (uint8_t*)((char*)obj + fields[i].offset)); \
                curr += sizeof(uint8_t); \
                break; \
            case FIELDTYPE_INT16: \
                READ_16(buffer, curr, (uint16_t*)((char*)obj + fields[i].offset)); \
                curr += sizeof(uint16_t); \
                break; \
            case FIELDTYPE_INT32: \
                READ_32(buffer, curr, (uint32_t*)((char*)obj + fields[i].offset)); \
                curr += sizeof(uint32_t); \
                break; \
            case FIELDTYPE_INT64: \
                READ_64(buffer, curr, (uint64_t*)((char*)obj + fields[i].offset)); \
                curr += sizeof(uint64_t); \
                break; \
            case FIELDTYPE_FLOAT: \
                READ_FLOAT(buffer, curr, (float*)((char*)obj + fields[i].offset)); \
                curr += sizeof(float); \
                break; \
            case FIELDTYPE_DOUBLE: \
                READ_DOUBLE(buffer, curr, (double*)((char*)obj + fields[i].offset)); \
                curr += sizeof(double); \
                break; \
            case FIELDTYPE_STR: ; \
                uint64_t size = 0; \
                READ_64(buffer, curr, &size); \
                READ_STR(buffer, curr, (char*)((char*)obj + fields[i].offset)); \
                curr += size; \
                break; \
            case FIELDTYPE_CUSTOM: ; \
                SerializeBase* base = (SerializeBase*)((char*)obj + fields[i].offset); \
                DeserializeFunc func = base->deserialize; \
                void* ptr = (void*)((char*)obj + fields[i].offset); \
                func(buffer, curr, ptr); \
                curr += base->size; \
                break; \
        } \
    } \
}

#endif