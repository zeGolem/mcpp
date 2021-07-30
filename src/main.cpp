#include "minecraft_server.h"
#include "network/tcp_server.h"
#include <iostream>
#include <thread>
#include <vector>

using namespace mcpp;

int main(int argc, char const *argv[])
{
	std::cout << "Hello, world" << std::endl;
	minecraft_server::the().start();
	while (minecraft_server::the().is_running())
		;
	return 0;
}
