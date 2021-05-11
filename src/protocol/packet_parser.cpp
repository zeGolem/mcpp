#include "packet_parser.h"
#include "packets/packet.h"
#include "protocol/types.h"
#include "utils/exception.h"
#include "utils/types.h"

using namespace mcpp;
using namespace protocol;

packet_parser::packet_parser(network::tcp_connection *connection) : m_connection(connection) {}

const type::varint packet_parser::read_varint()
{
	int numRead = 0;
	int result = 0;
	utils::byte read;
	do {
		read = m_connection->read_byte();
		int value = (read & 0b01111111);
		result |= (value << (7 * numRead));

		numRead++;
		if (numRead > 5) {
			throw utils::exception("VarInt is too big");
		}
	} while ((read & 0b10000000) != 0);

	return result;
}

const type::string packet_parser::read_string(unsigned int max_len)
{
	int len = read_varint();
	if (len > (int)max_len) throw utils::exception("String too long.");

	auto raw_data = m_connection->read(len);

	return std::string(raw_data.begin(), raw_data.end());
}

const unsigned short packet_parser::read_ushort()
{
	unsigned short target = 0;
	auto raw = m_connection->read(2);
	target = raw[0] << 8;
	target |= raw[1];
	return target;
}

packets::packet packet_parser::parse_base(bool store_raw_data)
{
	auto p = packets::packet{
	    .length = read_varint(),
	    .packet_id = read_varint(),
	};

	if (store_raw_data) { // TODO
	}

	return p;
}

template <> packets::serverboud_handshake packet_parser::parse_next(packets::packet base)
{
	auto p = packets::serverboud_handshake{};
	p.length = base.length;
	p.packet_id = base.packet_id;

	if (p.packet_id != 0x00)
		throw utils::exception("Trying to parse serverbound handshake, but packet ID isn't 0");

	p.protocol_version = read_varint();
	p.server_address = read_string(255);
	p.server_port = read_ushort();
	p.next_state = read_varint();

	return p;
}

template <> packets::serverboud_status_request packet_parser::parse_next(packets::packet base)
{
	auto p = packets::serverboud_status_request{};
	p.length = base.length;
	p.packet_id = base.packet_id;

	if (p.packet_id != 0x00)
		throw utils::exception("Trying to parse serverbound status request, but packet ID isn't 0");

	return p;
}
