#include "../include/packets.h"

void Packet_serialize(Buffer* buffer, const void* obj) {
    const Packet* packet = (const Packet*)obj;

    WRITE_8(buffer, packet->stage);

    switch (packet->stage) {
        case PROTOCOL_HANDSHAKE_START:
            SERIALIZE(buffer, *(HandshakeStart*)packet->next);
            break;
        case PROTOCOL_HANDSHAKE_MID_CLIENT:
            SERIALIZE(buffer, *(HandshakeMidClient*)packet->next);
            break;
        case PROTOCOL_HANDSHAKE_MID_SERVER:
            SERIALIZE(buffer, *(HandshakeMidServer*)packet->next);
            break;
        case PROTOCOL_HANDSHAKE_END:
            SERIALIZE(buffer, *(HandshakeEnd*)packet->next);
            break;
        case PROTOCOL_SPAWN_REQ:
            SERIALIZE(buffer, *(SpawnReq*)packet->next);
            break;
        case PROTOCOL_SPAWN_RES:
            SERIALIZE(buffer, *(SpawnRes*)packet->next);
            break;
        case PROTOCOL_MOVE:
            SERIALIZE(buffer, *(Move*)packet->next);
            break;
        case PROTOCOL_ATTACK:
            SERIALIZE(buffer, *(Attack*)packet->next);
            break;
        case PROTOCOL_MAP:
            SERIALIZE(buffer, *(Map*)packet->next);
            break;
        case PROTOCOL_DISCONNECT:
            SERIALIZE(buffer, *(Disconnect*)packet->next);
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
        case PROTOCOL_HANDSHAKE_MID_CLIENT:
            DESERIALIZE(buffer, curr, *(HandshakeMidClient*)packet->next);
            break;
        case PROTOCOL_HANDSHAKE_MID_SERVER:
            DESERIALIZE(buffer, curr, *(HandshakeMidServer*)packet->next);
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

Field handshake_start_fields[] = { { FIELDTYPE_INT16, offsetof(HandshakeStart, magic) } };
IMPL_SERIALIZE_FUNC(HandshakeStart_serialize, handshake_start_fields, sizeof(handshake_start_fields) / sizeof(handshake_start_fields[0]))
IMPL_DESERIALIZE_FUNC(HandshakeStart_deserialize, handshake_start_fields, sizeof(handshake_start_fields) / sizeof(handshake_start_fields[0]))

Field handshake_mid_client_fields[] = { { FIELDTYPE_INT16, offsetof(HandshakeMidClient, magic) }, { FIELDTYPE_INT64, offsetof(HandshakeMidClient, id) } };
IMPL_SERIALIZE_FUNC(HandshakeMidClient_serialize, handshake_mid_client_fields, sizeof(handshake_mid_client_fields) / sizeof(handshake_mid_client_fields[0]))
IMPL_DESERIALIZE_FUNC(HandshakeMidClient_deserialize, handshake_mid_client_fields, sizeof(handshake_mid_client_fields) / sizeof(handshake_mid_client_fields[0]))

Field handshake_mid_server_fields[] = { { FIELDTYPE_INT16, offsetof(HandshakeMidServer, magic) }, { FIELDTYPE_INT64, offsetof(HandshakeMidServer, id) } };
IMPL_SERIALIZE_FUNC(HandshakeMidServer_serialize, handshake_mid_server_fields, sizeof(handshake_mid_server_fields) / sizeof(handshake_mid_server_fields[0]))
IMPL_DESERIALIZE_FUNC(HandshakeMidServer_deserialize, handshake_mid_server_fields, sizeof(handshake_mid_server_fields) / sizeof(handshake_mid_server_fields[0]))

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

void Map_serialize(Buffer* buffer, const void* obj) {
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