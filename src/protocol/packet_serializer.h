#pragma once

#include "network/tcp_connection.h"
#include "protocol/packet_parser.h"
#include "protocol/packets/packet.h"
#include "protocol/types.h"
#include "utils/types.h"

namespace mcpp
{
namespace protocol
{

class packet_serializer
{
  private:
	network::tcp_connection *m_connection;

	void write_varint(type::varint);
	void write_string(type::string, unsigned int max_length);
	void write_long(long);
	void write_int(int);
	void write_uuid(type::uuid);
	void write_bool(bool);

	void write_byte(utils::byte byte) { m_connection->write_byte(byte); }

  public:
	packet_serializer(network::tcp_connection *connection);
	~packet_serializer() {}
	void serialize_and_send_base(packets::packet);
	template <typename T> void serialize_and_send(T packet);
};

} // namespace protocol
} // namespace mcpp
