#include "minecraft_server.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

using namespace mcpp;

void minecraft_server::load_infos()
{
	// Server status JSON
	{
		std::ifstream file("server_status.json");
		std::stringstream buff;
		buff << file.rdbuf();
		file.close();
		m_server_info.status = buff.str();
	}
}

void minecraft_server::start()
{
	m_tcpserver.start();
	load_infos();
	m_loop_thread = new std::thread([this]() {
		while (true)
			accept();
	});
	m_running = true;
}

void minecraft_server::accept()
{
	try {
		auto client = m_tcpserver.accept();
		auto *mc_client = new minecraft_client(client);
		mc_client->start_loop_thread();
		m_clients.push_back(mc_client);
	} catch (std::exception const &e) {
		std::cerr << "Error in client: " << e.what() << std::endl;
	}
}
