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

unsigned short packet_parser::read_ushort()
{
	unsigned short target = 0;
	auto raw = m_connection->read(2);
	target = raw[0] << 8;
	target |= raw[1];
	return target;
}

long int packet_parser::read_long()
{
	long target = 0;
	auto raw = m_connection->read(8);
	for (std::size_t i = 0; i < raw.size(); ++i)
		target |= raw[i] << 8 * i;
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

template <> packets::serverbound_handshake packet_parser::parse_next(packets::packet base)
{
	auto p = packets::serverbound_handshake{};
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

template <> packets::serverbound_status_request packet_parser::parse_next(packets::packet base)
{
	auto p = packets::serverbound_status_request{};
	p.length = base.length;
	p.packet_id = base.packet_id;

	if (p.packet_id != 0x00)
		throw utils::exception("Trying to parse serverbound status request, but packet ID isn't 0");

	return p;
}

template <> packets::serverbound_ping packet_parser::parse_next(packets::packet base)
{
	auto p = packets::serverbound_ping{};
	p.length = base.length;
	p.packet_id = base.packet_id;

	if (p.packet_id != 0x01)
		throw utils::exception("Trying to parse serverbound ping request, but packet ID isn't 1");

	p.payload = read_long();

	return p;
}

template <> packets::serverbound_login_start packet_parser::parse_next(packets::packet base)
{
	auto p = packets::serverbound_login_start{};
	p.length = base.length;
	p.packet_id = base.packet_id;

	if (p.packet_id != 0x00)
		throw utils::exception("Trying to parse serverbound login start, but packet ID isn't 0");

	p.name = read_string(16);

	return p;
}

template <> packets::serverbound_client_settings packet_parser::parse_next(packets::packet base)
{
	auto p = packets::serverbound_client_settings{};
	p.length = base.length;
	p.packet_id = base.packet_id;

	if (p.packet_id != 0x05)
		throw utils::exception(
		    "Trying to parse serverbound client settings, but packet ID isn't 5");

	p.locale = read_string(16);
	p.view_distance = read_byte();
	p.chat_mode = read_varint();
	p.chat_colors = read_bool();
	p.displayed_skin_parts = read_byte();
	p.main_hand = read_varint();
	p.disable_text_filtering = read_bool();

	return p;
}

template <> packets::serverbound_plugin_message packet_parser::parse_next(packets::packet base)
{
	auto p = packets::serverbound_plugin_message{};
	p.length = base.length;
	p.packet_id = base.packet_id;

	if (p.packet_id != 0x0a)
		throw utils::exception("Trying to parse serverbound login start, but packet ID isn't 0x0a");

	p.channel = read_string(32767);
	std::vector<utils::byte> data_buff;
	auto data_len = p.length - p.packet_id.size() - p.channel.size();
	p.data = m_connection->read(data_len); // Should maybe write a wrapper for that,
	                                       // I don't like calling function on the TCP
	                                       // connection directly like that...
	return p;
}
