#include "tcp_server.h"

#include "../utils/exception.h"
#include <iostream>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace mcpp::network;

tcp_server::tcp_server(int port) : m_port(port)
{
	m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket_fd < 0) {
		perror("socket");
		throw utils::exception("Failed to create socket");
	}

	bool true_ = true; // C APIs are wonderful
	// Make it so that we can reuse the socket.
	if (setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &true_, sizeof(int)) < 0) {
		perror("setsockopt");
		throw utils::exception("Failed to set socket options");
	}
}

tcp_server::~tcp_server()
{
	close(m_socket_fd);
}

void tcp_server::start()
{
	sockaddr_in server_addr{
	    .sin_family = AF_INET,
	    .sin_port = htons(m_port),
	    .sin_addr = {.s_addr = htonl(INADDR_ANY)}, // Listen on all addresses
	};

	if (bind(m_socket_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("bind");
		throw utils::exception("Failed to bind socket");
	}

	if (listen(m_socket_fd, BACKLOG) < 0) {
		perror("listen");
		throw utils::exception("Failed to listen on socket");
	}

	m_started = true;
}

tcp_connection *tcp_server::accept()
{
	if (!started()) throw utils::exception("Server not started!");

	sockaddr_in client_addr;
	socklen_t cadder_len = sizeof(sockaddr_in);
	int client_fd = ::accept(m_socket_fd, (sockaddr *)&client_addr, &cadder_len);
	if (client_fd < 0) {
		perror("accept");
		throw utils::exception("Failed to accept the client");
	}

	return new tcp_connection(client_fd, client_addr);
}
