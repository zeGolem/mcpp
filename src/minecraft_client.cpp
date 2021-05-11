#include "minecraft_client.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <thread>

#include "protocol/packets/packet.h"
#include "utils/exception.h"

using namespace mcpp;

minecraft_client::minecraft_client(network::tcp_connection *connection)
    : m_connection(connection), m_packet_parser(m_connection), m_packet_serializer(m_connection),
      m_state(state::HANDSHAKE)
{
}

minecraft_client::~minecraft_client()
{
	m_connection->close();
	delete m_connection;
}

void minecraft_client::send_status()
{
	std::ifstream status_info("server_status.json");
	std::stringstream buff;
	buff << status_info.rdbuf();
	status_info.close();

	auto status_response = protocol::packets::clientboud_status_response{
	    .json_response = buff.str(),
	};
	protocol::packets::build_base(status_response);
	m_packet_serializer.serialize_and_send(status_response);
}

void minecraft_client::send_pong(const long payload)
{
	auto ping_response = protocol::packets::clientboud_pong{
	    .payload = payload,
	};
	protocol::packets::build_base(ping_response);
	m_packet_serializer.serialize_and_send(ping_response);
}

void minecraft_client::start_loop_thread()
{
	m_loop_thread = new std::thread([this] { this->run_loop(); });
}

void minecraft_client::run_loop()
{
	bool should_continue = true;
	while (should_continue) {
		protocol::packets::packet next_packet = m_packet_parser.parse_base();
		std::cout << "Got a packet: id=" << next_packet.packet_id << ",len=" << next_packet.length
		          << std::endl;

		switch (m_state) {
		case state::HANDSHAKE: {
			if (next_packet.packet_id != 0x00)
				throw utils::exception("Invalid packet ID for handshake");

			auto handshake =
			    m_packet_parser.parse_next<protocol::packets::serverboud_handshake>(next_packet);

			std::cout << "Got handshake, port=" << handshake.server_port << std::endl;

			if (handshake.next_state == 1)
				m_state = state::STATUS;
			else if (handshake.next_state == 2)
				m_state = state::LOGIN;
			else
				throw utils::exception("Invalid state after handshake");
		} break;
		case state::STATUS: {
			if (next_packet.packet_id == 0x00) {
				// We are here : https://wiki.vg/Server_List_Ping#Request
				m_packet_parser.parse_next<protocol::packets::serverboud_status_request>(
				    next_packet);

				std::cout << "Got status request, replying" << std::endl;
				send_status();
			} else if (next_packet.packet_id == 0x01) {
				// We are here: https://wiki.vg/Server_List_Ping#Ping
				auto ping_request =
				    m_packet_parser.parse_next<protocol::packets::serverboud_ping>(next_packet);

				std::cout << "Got ping request, replying, payload=" << ping_request.payload
				          << std::endl;
				send_pong(ping_request.payload);
				should_continue = false;
			} else
				throw utils::exception("Invalid packet ID for Status state.");
		} break;
		default:
			utils::exception("Unimplemented state!!");
		}
	}
}
