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

} // namespace packets
} // namespace protocol
} // namespace mcpp
