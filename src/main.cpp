#include "minecraft_client.h"
#include "network/tcp_server.h"
#include <iostream>
#include <thread>
#include <vector>

using namespace mcpp;

int main(int argc, char const *argv[])
{
	std::cout << "Hello, world" << std::endl;

	std::vector<minecraft_client*> clients;

	try {
		network::tcp_server serv;
		serv.start();
		while (true) {
			auto client = serv.accept();
			auto *mcc = new minecraft_client(client);
			mcc->start_loop_thread();
			clients.push_back(mcc);
		}
	} catch (std::exception const &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}
