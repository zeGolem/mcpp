#include "minecraft_client.h"
#include "network/tcp_server.h"
#include <iostream>
#include <thread>
#include <vector>

using namespace mcpp;

int main(int argc, char const *argv[])
{
	std::cout << "Hello, world" << std::endl;

	try {
		network::tcp_server serv;
		serv.start();
		while (true) {
			auto client = serv.accept();

			auto new_thread = new std::thread([client] {
				auto mcc = minecraft_client(client);
				mcc.run_loop();
			});
		}
	} catch (std::exception const &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}
