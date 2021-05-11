#include "packet.h"

using namespace mcpp;
using namespace protocol;
using namespace packets;

template <> void packets::build_base(clientboud_status_response &packet) {
	packet.packet_id = 0x00;
	packet.length = packet.packet_id.size() + packet.json_response.size();
}

