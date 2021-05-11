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

	std::thread *m_loop_thread;

	enum state
	{
		HANDSHAKE,
		STATUS,
		LOGIN,
	} m_state;

	void send_status();
	void send_pong(const long payload);

  public:
	minecraft_client(network::tcp_connection *);
	~minecraft_client();

	void start_loop_thread();
	void run_loop();

	const state state() const { return m_state; }
};

} // namespace mcpp
