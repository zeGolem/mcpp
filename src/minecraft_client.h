#pragma once
#include "network/tcp_connection.h"
#include "protocol/packet_parser.h"
#include "protocol/packet_serializer.h"
#include <thread>

namespace mcpp
{

class minecraft_client
{
  private:
	network::tcp_connection *m_connection;
	protocol::packet_parser m_packet_parser;
	protocol::packet_serializer m_packet_serializer;

	enum state
	{
		HANDSHAKE,
		STATUS,
		LOGIN,
	} m_state;

	void send_status();

  public:
	minecraft_client(network::tcp_connection *);
	~minecraft_client();

	void run_loop();
	void handle();

	const state state() const { return m_state; }
};

} // namespace mcpp
