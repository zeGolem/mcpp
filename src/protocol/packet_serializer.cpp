#include "packet_serializer.h"

#include <vector>

#include "network/tcp_connection.h"
#include "packets/packet.h"
#include "protocol/types.h"
#include "utils/exception.h"
#include "utils/types.h"

using namespace mcpp;
using namespace protocol;

void packet_serializer::write_varint(type::varint varint)
{
	auto value = varint.value();
	do {
		utils::byte temp = (utils::byte)(value & 0b01111111);
		value >>= 7;
		if (value != 0) {
			temp |= 0b10000000;
		}
		m_connection->write_byte(temp);
	} while (value != 0);
}

void packet_serializer::write_string(type::string str, unsigned int max_len)
{
	auto data = str.value();
	if (data.length() > max_len) throw utils::exception("String too long");
	write_varint(data.length());
	m_connection->write(std::vector<unsigned char>(data.begin(), data.end()));
}

void packet_serializer::write_long(long val)
{
	for (std::size_t i = 0; i < 8; ++i) { // For each byte in the long
		utils::byte current = val >> 8 * i;
		m_connection->write_byte(current);
	}
}

void packet_serializer::write_uuid(type::uuid uuid)
{
	for (std::size_t i = 0; i < 16; ++i) { // For each byte in the uuid
		utils::byte current = uuid >> 8 * i;
		m_connection->write_byte(current);
	}
}

packet_serializer::packet_serializer(network::tcp_connection *connection) : m_connection(connection)
{
}

void packet_serializer::serialize_and_send_base(packets::packet base)
{
	write_varint(base.length);
	write_varint(base.packet_id);
}

template <> void packet_serializer::serialize_and_send(packets::clientboud_status_response packet)
{
	serialize_and_send_base(packet);
	write_string(packet.json_response, 32767);
}

template <> void packet_serializer::serialize_and_send(packets::clientboud_pong packet)
{
	serialize_and_send_base(packet);
	write_long(packet.payload);
}

template <> void packet_serializer::serialize_and_send(packets::clientbound_login_success packet)
{
	serialize_and_send_base(packet);
	write_uuid(packet.uuid);
	write_string(packet.username, 16);
}
