#include "minecraft_client.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>

#include "protocol/packets/packet.h"
#include "protocol/types.h"
#include "utils/exception.h"
#include "utils/types.h"
#include "minecraft_server.h"

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
	auto status_response = protocol::packets::clientbound_status_response{
	    .json_response = minecraft_server::the().info().status,
	};
	protocol::packets::build_base(status_response);
	m_packet_serializer.serialize_and_send(status_response);
}

void minecraft_client::send_pong(const long payload)
{
	auto ping_response = protocol::packets::clientbound_pong{
	    .payload = payload,
	};
	protocol::packets::build_base(ping_response);
	m_packet_serializer.serialize_and_send(ping_response);
}

void minecraft_client::send_login_success(const protocol::type::string username)
{
	auto login_success = protocol::packets::clientbound_login_success{
	    .uuid = 0,
	    .username = username,
	};
	protocol::packets::build_base(login_success);
	m_packet_serializer.serialize_and_send(login_success);
}

void minecraft_client::send_join_game()
{
	auto join_game = protocol::packets::clientbound_join_game{
	    .entity_id = 0,
	    .is_hardcore = false,
	    .gamemode = 1,
	    .previous_gamemode = 1,
	    .world_count = 1,
	    .world_names = {std::string("world")},
	    .dimension_codec = minecraft_server::the().info().dimension_codec,
	    .dimension = minecraft_server::the().info().dimension,
	    .world_name = std::string("world"),
	    .hashed_seed = 0x1f45813f1432a2d7,
	    .max_player = 1,
	    .view_distance = 2,
	    .reduced_debug_info = false,
	    .enable_respawn_screen = true,
	    .is_debug = true,
	    .is_flat = false,
	};
	protocol::packets::build_base(join_game);
	m_packet_serializer.serialize_and_send(join_game);
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
			    m_packet_parser.parse_next<protocol::packets::serverbound_handshake>(next_packet);

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
				m_packet_parser.parse_next<protocol::packets::serverbound_status_request>(
				    next_packet);

				std::cout << "Got status request, replying" << std::endl;
				send_status();
			} else if (next_packet.packet_id == 0x01) {
				// We are here: https://wiki.vg/Server_List_Ping#Ping
				auto ping_request =
				    m_packet_parser.parse_next<protocol::packets::serverbound_ping>(next_packet);

				std::cout << "Got ping request, replying, payload=" << ping_request.payload
				          << std::endl;
				send_pong(ping_request.payload);
				should_continue = false;
			} else
				throw utils::exception("Invalid packet ID for Status state.");
		} break;

		case state::LOGIN: {
			if (next_packet.packet_id == 0x00) {
				// Login start
				auto login_start =
				    m_packet_parser.parse_next<protocol::packets::serverbound_login_start>(
				        next_packet);
				std::cout << "Got login for user " << login_start.name.value() << std::endl;

				send_login_success(login_start.name);
				m_state = state::PLAY;
				send_join_game();
			}
		} break;

		default:
			utils::exception("Unimplemented state!!");
		}
	}
}
