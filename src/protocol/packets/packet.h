#pragma once
#include "protocol/types.h"
#include "utils/types.h"
#include <string>
#include <vector>

namespace mcpp
{
namespace protocol
{
namespace packets
{

template <typename T> void build_base(T &packet);

struct packet {
	type::varint length;
	type::varint packet_id;
	std::vector<utils::byte> raw_data;
};

struct serverboud_handshake : packet {
	type::varint protocol_version;
	type::string server_address;
	ushort server_port;
	type::varint next_state;
};

struct serverboud_status_request : packet {
};

struct clientboud_status_response : packet {
	type::string json_response;
};

struct serverboud_ping : packet {
	long payload;
};
struct clientboud_pong : packet {
	long payload;
};

struct serverbound_login_start : packet {
	type::string name;
};

struct clientbound_login_success : packet {
	type::uuid uuid;
	type::string username;
};

struct clientboud_join_game : packet {
	int entity_id;
	bool is_hardcore;
	utils::byte gamemode;
	char previous_gamemode; // TODO: Use a better type
	type::varint world_count;
	std::vector<type::identifier> world_names;
	type::nbt_data dimension_codec;
	type::nbt_data dimension;
	type::identifier world_name;
	long hashed_seed;
	type::varint max_player;
	type::varint view_distance;
	bool reduced_debug_info;
	bool enable_respawn_screen;
	bool is_debug;
	bool is_flat;
};

} // namespace packets
} // namespace protocol
} // namespace mcpp
