#include "../include/packets.h"

void Packet_serialize(Buffer* buffer, void* obj) {
    Packet* packet = (Packet*)obj;
    ATTACH_SIZE(*packet, sizeof(uint8_t));

    WRITE_8(buffer, packet->stage);

    switch (packet->stage) {
        case PROTOCOL_HANDSHAKE_START: ;
            HandshakeStart* start_ptr = (HandshakeStart*)packet->next;
            SERIALIZE(buffer, *start_ptr);
            ATTACH_SIZE(*packet, GET_SIZE(*packet) + GET_SIZE(*start_ptr));
            break;
        case PROTOCOL_HANDSHAKE_MID: ;
            HandshakeMid* client_ptr = (HandshakeMid*)packet->next;
            SERIALIZE(buffer, *client_ptr);
            ATTACH_SIZE(*packet, GET_SIZE(*packet) + GET_SIZE(*client_ptr));
            break;
            break;
        case PROTOCOL_HANDSHAKE_END: ;
            HandshakeEnd* end_ptr = (HandshakeEnd*)packet->next;
            SERIALIZE(buffer, *end_ptr);
            ATTACH_SIZE(*packet, GET_SIZE(*packet) + GET_SIZE(*end_ptr));
            break;
        case PROTOCOL_SPAWN_REQ: ;
            SpawnReq* req_ptr = (SpawnReq*)packet->next;
            SERIALIZE(buffer, *req_ptr);
            ATTACH_SIZE(*packet, GET_SIZE(*packet) + GET_SIZE(*req_ptr));
            break;
        case PROTOCOL_SPAWN_RES: ;
            SpawnRes* res_ptr = (SpawnRes*)packet->next;
            SERIALIZE(buffer, *(SpawnRes*)packet->next);
            ATTACH_SIZE(*packet, GET_SIZE(*packet) + GET_SIZE(*res_ptr));
            break;
        case PROTOCOL_MOVE: ;
            Move* move_ptr = (Move*)packet->next;
            SERIALIZE(buffer, *(Move*)packet->next);
            ATTACH_SIZE(*packet, GET_SIZE(*packet) + GET_SIZE(*move_ptr));
            break;
        case PROTOCOL_ATTACK: ;
            Attack* attack_ptr = (Attack*)packet->next;
            SERIALIZE(buffer, *attack_ptr);
            ATTACH_SIZE(*packet, GET_SIZE(*packet) + GET_SIZE(*attack_ptr));
            break;
        case PROTOCOL_MAP: ;
            Map* map_ptr = (Map*)packet->next;
            SERIALIZE(buffer, *map_ptr);
            ATTACH_SIZE(*packet, GET_SIZE(*packet) + GET_SIZE(*map_ptr));
            break;
        case PROTOCOL_DISCONNECT: ;
            Disconnect* disconnect_ptr = (Disconnect*)packet->next;
            SERIALIZE(buffer, *(Disconnect*)packet->next);
            ATTACH_SIZE(*packet, GET_SIZE(*packet) + GET_SIZE(*disconnect_ptr));
            break;
    }
}

void Packet_deserialize(Buffer* buffer, const size_t addr, void* obj) {
    Packet* packet = (Packet*)obj;

    READ_8(buffer, addr, (uint8_t*)&packet->stage);

    size_t curr = addr + sizeof(uint8_t);
    switch (packet->stage) {
        case PROTOCOL_HANDSHAKE_START:
            DESERIALIZE(buffer, curr, *(HandshakeStart*)packet->next);
            break;
        case PROTOCOL_HANDSHAKE_MID:
            DESERIALIZE(buffer, curr, *(HandshakeMid*)packet->next);
            break;
        case PROTOCOL_HANDSHAKE_END:
            DESERIALIZE(buffer, curr, *(HandshakeEnd*)packet->next);
            break;
        case PROTOCOL_SPAWN_REQ:
            DESERIALIZE(buffer, curr, *(SpawnReq*)packet->next);
            break;
        case PROTOCOL_SPAWN_RES:
            DESERIALIZE(buffer, curr, *(SpawnRes*)packet->next);
            break;
        case PROTOCOL_MOVE:
            DESERIALIZE(buffer, curr, *(Move*)packet->next);
            break;
        case PROTOCOL_ATTACK:
            DESERIALIZE(buffer, curr, *(Attack*)packet->next);
            break;
        case PROTOCOL_MAP:
            DESERIALIZE(buffer, curr, *(Map*)packet->next);
            break;
        case PROTOCOL_DISCONNECT:
            DESERIALIZE(buffer, curr, *(Disconnect*)packet->next);
            break;
    }
}

Field handshake_start_fields[] = { { FIELDTYPE_INT16, offsetof(HandshakeStart, magic) }, { FIELDTYPE_STR, offsetof(HandshakeStart, name) } };
IMPL_SERIALIZE_FUNC(HandshakeStart_serialize, handshake_start_fields, sizeof(handshake_start_fields) / sizeof(handshake_start_fields[0]))
IMPL_DESERIALIZE_FUNC(HandshakeStart_deserialize, handshake_start_fields, sizeof(handshake_start_fields) / sizeof(handshake_start_fields[0]))

Field handshake_mid_fields[] = { { FIELDTYPE_INT16, offsetof(HandshakeMid, magic) }, { FIELDTYPE_INT64, offsetof(HandshakeMid, id) }, { FIELDTYPE_STR, offsetof(HandshakeStart, name) } };
IMPL_SERIALIZE_FUNC(HandshakeMid_serialize, handshake_mid_fields, sizeof(handshake_mid_fields) / sizeof(handshake_mid_fields[0]))
IMPL_DESERIALIZE_FUNC(HandshakeMid_deserialize, handshake_mid_fields, sizeof(handshake_mid_fields) / sizeof(handshake_mid_fields[0]))

Field handshake_end_fields[] = { { FIELDTYPE_INT16, offsetof(HandshakeEnd, magic) }, { FIELDTYPE_INT8, offsetof(HandshakeEnd, confirm) } };
IMPL_SERIALIZE_FUNC(HandshakeEnd_serialize, handshake_end_fields, sizeof(handshake_end_fields) / sizeof(handshake_end_fields[0]))
IMPL_DESERIALIZE_FUNC(HandshakeEnd_deserialize, handshake_end_fields, sizeof(handshake_end_fields) / sizeof(handshake_end_fields[0]))

Field spawn_req_fields[] = { { FIELDTYPE_INT64, offsetof(SpawnReq, id) } };
IMPL_SERIALIZE_FUNC(SpawnReq_serialize, spawn_req_fields, sizeof(spawn_req_fields) / sizeof(spawn_req_fields[0]))
IMPL_DESERIALIZE_FUNC(SpawnReq_deserialize, spawn_req_fields, sizeof(spawn_req_fields) / sizeof(spawn_req_fields[0]))

Field spawn_res_fields[] = { { FIELDTYPE_DOUBLE, offsetof(SpawnRes, x) }, { FIELDTYPE_DOUBLE, offsetof(SpawnRes, y) } };
IMPL_SERIALIZE_FUNC(SpawnRes_serialize, spawn_res_fields, sizeof(spawn_res_fields) / sizeof(spawn_res_fields[0]))
IMPL_DESERIALIZE_FUNC(SpawnRes_deserialize, spawn_res_fields, sizeof(spawn_res_fields) / sizeof(spawn_res_fields[0]))

Field move_fields[] = { { FIELDTYPE_INT64, offsetof(Move, id) }, { FIELDTYPE_DOUBLE, offsetof(Move, dx) }, { FIELDTYPE_DOUBLE, offsetof(Move, dy) } };
IMPL_SERIALIZE_FUNC(Move_serialize, move_fields, sizeof(move_fields) / sizeof(move_fields[0]))
IMPL_DESERIALIZE_FUNC(Move_deserialize, move_fields, sizeof(move_fields) / sizeof(move_fields[0]))

Field attack_fields[] = { { FIELDTYPE_INT64, offsetof(Attack, id) }, { FIELDTYPE_INT8, offsetof(Attack, attack) } };
IMPL_SERIALIZE_FUNC(Attack_serialize, attack_fields, sizeof(attack_fields) / sizeof(attack_fields[0]))
IMPL_DESERIALIZE_FUNC(Attack_deserialize, attack_fields, sizeof(attack_fields) / sizeof(attack_fields[0]))

void Map_serialize(Buffer* buffer, void* obj) {
    const Map* map = (const Map*)obj;

    WRITE_64(buffer, map->num_players);

    for (uint64_t i = 0; i < map->num_players; i++) {
        WRITE_DOUBLE(buffer, map->xs[i]);
    }
    for (uint64_t i = 0; i < map->num_players; i++) {
        WRITE_DOUBLE(buffer, map->ys[i]);
    }
    for (uint64_t i = 0; i < map->num_players; i++) {
        WRITE_8(buffer, map->healths[i]);
    }
}

void Map_deserialize(Buffer* buffer, const size_t addr, void* obj) {
    Map* map = (Map*)obj;
    ATTACH_SIZE(*map, sizeof(uint64_t) + (sizeof(double) * map->num_players) + (sizeof(double) * map->num_players) + (sizeof(uint8_t) * map->num_players));
    size_t curr = addr;

    READ_64(buffer, curr, &map->num_players);
    curr += sizeof(uint64_t);

    if (map->xs) {
        free(map->xs);
    }
    if (map->ys) {
        free(map->ys);
    }
    if (map->healths) {
        free(map->healths);
    }

    map->xs = (double*)malloc(sizeof(double) * map->num_players);
    map->ys = (double*)malloc(sizeof(double) * map->num_players);
    map->healths = (uint8_t*)malloc(sizeof(uint8_t) * map->num_players);

    for (uint64_t i = 0; i < map->num_players; i++) {
        READ_DOUBLE(buffer, curr, &map->xs[i]);
        curr += sizeof(double);
    }
    for (uint64_t i = 0; i < map->num_players; i++) {
        READ_DOUBLE(buffer, curr, &map->ys[i]);
        curr += sizeof(double);
    }
    for (uint64_t i = 0; i < map->num_players; i++) {
        READ_8(buffer, curr, &map->healths[i]);
        curr += sizeof(uint8_t);
    }
}

Field disconnect_fields[] = { { FIELDTYPE_INT64, offsetof(Disconnect, id) } };
IMPL_SERIALIZE_FUNC(Disconnect_serialize, disconnect_fields, sizeof(disconnect_fields) / sizeof(disconnect_fields[0]))
IMPL_DESERIALIZE_FUNC(Disconnect_deserialize, disconnect_fields, sizeof(disconnect_fields) / sizeof(disconnect_fields[0]))

Packet Packet_empty() {
    Packet packet = {0};
    ATTACH_SERIALIZER(packet, Packet_serialize);
    ATTACH_DESERIALIZER(packet, Packet_deserialize);
    // Size will be added later

    return packet;
}

HandshakeStart HandshakeStart_empty() {
    HandshakeStart start = {0};
    ATTACH_SERIALIZER(start, HandshakeStart_serialize);
    ATTACH_DESERIALIZER(start, HandshakeStart_deserialize);
    ATTACH_SIZE(start, sizeof(uint16_t) + (sizeof(char) * MAX_NAME_LEN));

    return start;
}

HandshakeMid HandshakeMid_empty() {
    HandshakeMid client = {0};
    ATTACH_SERIALIZER(client, HandshakeMid_serialize);
    ATTACH_DESERIALIZER(client, HandshakeMid_deserialize);
    ATTACH_SIZE(client, sizeof(uint16_t) + sizeof(uint64_t) + (sizeof(char) * MAX_NAME_LEN));

    return client;
}

HandshakeEnd HandshakeEnd_empty() {
    HandshakeEnd end = {0};
    ATTACH_SERIALIZER(end, HandshakeEnd_serialize);
    ATTACH_DESERIALIZER(end, HandshakeEnd_deserialize);
    ATTACH_SIZE(end, sizeof(uint16_t) + sizeof(uint8_t));

    return end;
}

SpawnReq SpawnReq_empty() {
    SpawnReq req = {0};
    ATTACH_SERIALIZER(req, SpawnReq_serialize);
    ATTACH_DESERIALIZER(req, SpawnReq_deserialize);
    ATTACH_SIZE(req, sizeof(uint64_t));

    return req;
}

SpawnRes SpawnRes_empty() {
    SpawnRes res = {0};
    ATTACH_SERIALIZER(res, SpawnRes_serialize);
    ATTACH_DESERIALIZER(res, SpawnRes_deserialize);
    ATTACH_SIZE(res, sizeof(double) + sizeof(double));

    return res;
}

Move Move_empty() {
    Move move = {0};
    ATTACH_SERIALIZER(move, Move_serialize);
    ATTACH_DESERIALIZER(move, Move_deserialize);
    ATTACH_SIZE(move, sizeof(uint64_t) + sizeof(double) + sizeof(double));

    return move;
}

Attack Attack_empty() {
    Attack attack = {0};
    ATTACH_SERIALIZER(attack, Attack_serialize);
    ATTACH_DESERIALIZER(attack, Attack_deserialize);
    ATTACH_SIZE(attack, sizeof(uint64_t) + sizeof(uint8_t));

    return attack;
}

Map Map_empty() {
    Map map = {0};
    ATTACH_SERIALIZER(map, Map_serialize);
    ATTACH_DESERIALIZER(map, Map_deserialize);
    // Size will be added later

    return map;
}

Disconnect Disconnect_empty() {
    Disconnect disconnect = {0};
    ATTACH_SERIALIZER(disconnect, Disconnect_serialize);
    ATTACH_DESERIALIZER(disconnect, Disconnect_deserialize);
    ATTACH_SIZE(disconnect, sizeof(uint64_t));

    return disconnect;
}