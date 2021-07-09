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
