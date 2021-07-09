#include "packet.h"

using namespace mcpp;
using namespace protocol;
using namespace packets;

template <> void packets::build_base(clientboud_status_response &packet)
{
	packet.packet_id = 0x00;
	packet.length = packet.packet_id.size() + packet.json_response.size();
}

template <> void packets::build_base(clientboud_pong &packet)
{
	packet.packet_id = 0x01;
	packet.length = packet.packet_id.size() + sizeof(packet.payload);
}

template <> void packets::build_base(clientbound_login_success &packet)
{
	packet.packet_id = 0x02;
	packet.length = packet.packet_id.size() + packet.uuid.size() + packet.username.size();
}

template <> void packets::build_base(clientboud_join_game &packet)
{
	packet.packet_id = 0x26;

	long len = 0;
	len += packet.packet_id.size();
	len += sizeof(packet.entity_id);
	len += sizeof(packet.is_hardcore);
	len += sizeof(packet.gamemode);
	len += sizeof(packet.previous_gamemode);
	len += packet.world_count.size();
	for (auto &world_name : packet.world_names) {
		len += world_name.size();
	}
	len += packet.dimension_codec.size();
	len += packet.dimension.size();
	len += packet.world_name.size();
	len += sizeof(packet.hashed_seed);
	len += packet.max_player.size();
	len += packet.view_distance.size();
	len += sizeof(packet.reduced_debug_info);
	len += sizeof(packet.enable_respawn_screen);
	len += sizeof(packet.is_debug);
	len += sizeof(packet.is_flat);
	packet.length = len;
}
