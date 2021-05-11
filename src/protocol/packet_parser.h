#pragma once
#include "network/tcp_connection.h"
#include "protocol/packets/packet.h"
#include "protocol/types.h"

namespace mcpp
{
namespace protocol
{

class packet_parser
{
  private:
	network::tcp_connection *m_connection;
	const type::varint read_varint();
	const type::string read_string(unsigned int max_length);
	const unsigned short read_ushort();

  public:
	packet_parser(network::tcp_connection *connection);
	~packet_parser() {}

	packets::packet parse_base(bool store_raw_data = false);
	template <typename T> T parse_next(packets::packet base);
	template <typename T> inline T parse() { return parse_next<T>(parse_base()); };
};

} // namespace protocol
} // namespace mcpp
