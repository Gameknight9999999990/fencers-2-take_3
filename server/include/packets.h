#ifndef FENCERS2_SERVER_PACKETS_H
#define FENCERS2_SERVER_PACKETS_H

#include <stdio.h>
#include <stdlib.h>
#include "sockets.h"
#include "serialization.h"

#define PROTOCOL_STAGE_LIST(X) \
    X(HANDSHAKE_START) \
    X(HANDSHAKE_MID) \
    X(HANDSHAKE_END) \
    X(SPAWN_REQ) \
    X(SPAWN_RES) \
    X(MOVE) \
    X(ATTACK) \
    X(MAP) \
    X(DISCONNECT)

typedef enum ProtocolStage {
    #define X(name) PROTOCOL_##name,
    PROTOCOL_STAGE_LIST(X)
    #undef X
} ProtocolStage;

typedef struct Packet {
    OBJECT_SERIALIZABLE
    ProtocolStage stage;
    void* next;
} Packet;
DECLARE_SERIALIZE_FUNC(Packet_serialize)
DECLARE_DESERIALIZE_FUNC(Packet_deserialize)

Packet Packet_empty();

#define MAX_NAME_LEN 64
#define HANDSHAKE_START_MAGIC 0x4645
// Sent by client
typedef struct HandshakeStart {
    OBJECT_SERIALIZABLE
    uint16_t magic;
    char name[MAX_NAME_LEN];
} HandshakeStart;
extern Field handshake_start_fields[];
DECLARE_SERIALIZE_FUNC(HandshakeStart_serialize)
DECLARE_DESERIALIZE_FUNC(HandshakeStart_deserialize)

HandshakeStart HandshakeStart_empty();

#define HANDSHAKE_MID_MAGIC 0x4E43
// Sent by server and client
typedef struct HandshakeMid {
    OBJECT_SERIALIZABLE
    uint16_t magic;
    uint64_t id;
    char name[MAX_NAME_LEN];
} HandshakeMid;
extern Field handshake_mid_fields[];
DECLARE_SERIALIZE_FUNC(HandshakeMid_serialize)
DECLARE_DESERIALIZE_FUNC(HandshakeMid_deserialize)

HandshakeMid HandshakeMid_empty();

#define HANDSHAKE_END_MAGIC 0x4552
// Sent by server
typedef struct HandshakeEnd {
    OBJECT_SERIALIZABLE
    uint16_t magic;
    bool confirm;
} HandshakeEnd;
extern Field handshake_end_fields[];
DECLARE_SERIALIZE_FUNC(HandshakeEnd_serialize)
DECLARE_DESERIALIZE_FUNC(HandshakeEnd_deserialize)

HandshakeEnd HandshakeEnd_empty();

// Sent by client
typedef struct SpawnReq {
    OBJECT_SERIALIZABLE
    uint64_t id;
} SpawnReq;
extern Field spawn_req_fields[];
DECLARE_SERIALIZE_FUNC(SpawnReq_serialize)
DECLARE_DESERIALIZE_FUNC(SpawnReq_deserialize)

SpawnReq SpawnReq_empty();

// Sent by server
typedef struct SpawnRes {
    OBJECT_SERIALIZABLE
    double x;
    double y;
} SpawnRes;
extern Field spawn_res_fields[];
DECLARE_SERIALIZE_FUNC(SpawnRes_serialize)
DECLARE_DESERIALIZE_FUNC(SpawnRes_deserialize)

SpawnRes SpawnRes_empty();

// Sent by client
typedef struct Move {
    OBJECT_SERIALIZABLE
    uint64_t id;
    double dx;
    double dy;
} Move;
extern Field move_fields[];
DECLARE_SERIALIZE_FUNC(Move_serialize)
DECLARE_DESERIALIZE_FUNC(Move_deserialize)

Move Move_empty();

// Sent by client
typedef struct Attack {
    OBJECT_SERIALIZABLE
    uint64_t id;
    uint8_t attack;
} Attack;
extern Field attack_fields[];
DECLARE_SERIALIZE_FUNC(Attack_serialize)
DECLARE_DESERIALIZE_FUNC(Attack_deserialize)

Attack Attack_empty();

// Sent by server
typedef struct Map {
    OBJECT_SERIALIZABLE
    uint64_t num_players;
    double* xs;
    double* ys;
    uint8_t* healths;
} Map;
DECLARE_SERIALIZE_FUNC(Map_serialize)
DECLARE_DESERIALIZE_FUNC(Map_deserialize)

Map Map_empty();

// Sent by client
typedef struct Disconnect {
    OBJECT_SERIALIZABLE
    uint64_t id;
} Disconnect;
extern Field disconnect_fields[];
DECLARE_SERIALIZE_FUNC(Disconnect_serialize)
DECLARE_DESERIALIZE_FUNC(Disconnect_deserialize)

Disconnect Disconnect_empty();

#endif