#pragma once
#include "network/tcp_connection.h"
#include "protocol/packet_parser.h"
#include "protocol/packet_serializer.h"
#include "protocol/packets/packet.h"
#include "utils/types.h"
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
		PLAY,
	} m_state;

	enum chat_modes
	{
		enabled = 0,
		commands_only = 1,
		hidden = 2,
	};

	enum main_hands
	{
		left = 0,
		right = 1,
	};

	struct client_info {
		// Information from client settings packet
		std::string locale;
		utils::byte view_distance;
		chat_modes chat_mode;
		bool chat_color;
		utils::byte displayed_skin_parts;
		main_hands main_hand;
		bool disable_text_filtering;
	} m_client_info;

	void send_status();
	void send_pong(const long payload);
	void send_login_success(const protocol::type::string username);
	void send_join_game();
	void send_held_item_change(const utils::byte);
	void send_recipes();

	void update_settings(const protocol::packets::serverbound_client_settings &);

	void handle_plugin_message(const protocol::packets::serverbound_plugin_message &);

  public:
	minecraft_client(network::tcp_connection *);
	~minecraft_client();

	void start_loop_thread();
	void run_loop();

	const state state() const { return m_state; }
};

} // namespace mcpp
