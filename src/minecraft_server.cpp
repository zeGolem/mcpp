#include "minecraft_server.h"
#include "utils/types.h"
#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

using namespace mcpp;

void minecraft_server::load_infos()
{
	auto read_bin = [](const char *filename) -> std::vector<utils::byte> {
		// There is probably a better way to do that...
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		auto size = file.tellg();
		file.seekg(0, std::ios::beg);
		auto buff = new utils::byte[size];
		file.read((char *)buff, size);
		std::vector<utils::byte> data(buff, buff + size);
		return data;
	};
	// Server status JSON
	{
		std::ifstream file("server_status.json");
		std::stringstream buff;
		buff << file.rdbuf();
		file.close();
		m_server_info.status = buff.str();
	}
	// NBT Dimension stuff, see https://wiki.vg/NBT
	m_server_info.dimension_codec = read_bin("nbt/dimension_codec.nbt");
	m_server_info.dimension = read_bin("nbt/dimension.nbt");
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
