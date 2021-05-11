#include "tcp_server.h"

#include "../utils/exception.h"
#include <iostream>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace mcpp::network;

tcp_server::tcp_server(int port) : m_port(port)
{
	auto socket_err =
	    !(m_socket_fd = socket(AF_INET, SOCK_STREAM, 0)); // Error is -1, so we invert it.
	perror("socket");
	if (socket_err) throw utils::exception("Failed to create socket");

	bool true_ = true; // C APIs are wonderful
	// Make it so that we can reuse the socket.
	setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &true_, sizeof(int));
	perror("setsockopt");
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

	auto bind_err = bind(m_socket_fd, (sockaddr *)&server_addr, sizeof(server_addr));
	perror("bind");
	if (bind_err) throw utils::exception("Failed to bind socket");

	auto listen_err = listen(m_socket_fd, BACKLOG);
	perror("listen");
	if (listen_err) throw utils::exception("Failed to listen on socket");

	m_started = true;
}

tcp_connection *tcp_server::accept()
{
	if (!started()) throw utils::exception("Server not started!");

	sockaddr_in client_addr;
	socklen_t cadder_len = sizeof(sockaddr_in);
	int client_fd = 0;
	auto err = // Error is -1
	    !(client_fd = ::accept(m_socket_fd, (sockaddr *)&client_addr, &cadder_len));
	perror("accept");
	if (err) throw utils::exception("Failed to accept the client");

	return new tcp_connection(client_fd, client_addr);
}
