#pragma once

#include "minecraft_client.h"
#include "network/tcp_server.h"
#include <thread>
#include <vector>

namespace mcpp
{

struct server_info {
	std::string status;
};

class minecraft_server
{
  private:
	minecraft_server(){};

	std::vector<minecraft_client *> m_clients;
	network::tcp_server m_tcpserver;
	std::thread *m_loop_thread;
	bool m_running;
	server_info m_server_info;

	void load_infos();

  public:
	void start();
	void accept();

	const bool is_running() const { return m_running; }
	const std::vector<minecraft_client *> clients() const { return m_clients; }
	const server_info info() const { return m_server_info; }

	static minecraft_server &the()
	{
		static minecraft_server instance;
		return instance;
	};
};

} // namespace mcpp
