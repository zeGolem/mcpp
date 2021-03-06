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
	type::varint length = 0;
	type::varint packet_id = 0;
};

struct serverbound_handshake : packet {
	type::varint protocol_version;
	type::string server_address;
	ushort server_port;
	type::varint next_state;
};

struct serverbound_status_request : packet {
};

struct clientbound_status_response : packet {
	type::string json_response;
};

struct serverbound_ping : packet {
	long payload;
};
struct clientbound_pong : packet {
	long payload;
};

struct serverbound_login_start : packet {
	type::string name;
};

struct clientbound_login_success : packet {
	type::uuid uuid;
	type::string username;
};

struct clientbound_join_game : packet {
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
	type::varint simulation_distance;
	bool reduced_debug_info;
	bool enable_respawn_screen;
	bool is_debug;
	bool is_flat;
};

struct serverbound_client_settings : packet {
	type::string locale;
	utils::byte view_distance;
	type::varint chat_mode;
	bool chat_colors;
	utils::byte displayed_skin_parts;
	type::varint main_hand;
	bool disable_text_filtering;
};

struct _plugin_message : packet {
	type::identifier channel;
	std::vector<utils::byte> data;
};

typedef _plugin_message serverbound_plugin_message;
typedef _plugin_message clientbound_plugin_message;

struct clientbound_held_item_change : packet {
	utils::byte slot;
};

struct clientbound_declare_recipes : packet {
	type::varint num_recipes;
	std::vector<type::recipe> recipes;
};

} // namespace packets
} // namespace protocol
} // namespace mcpp
