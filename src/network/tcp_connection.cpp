#include "tcp_connection.h"
#include <iostream>
#include <unistd.h>

#include "utils/exception.h"
#include "utils/types.h"

using namespace mcpp;
using namespace mcpp::network;

tcp_connection::tcp_connection(int fd, sockaddr_in client_addr)
    : m_socket_fd(fd), m_addr(client_addr)
{
}

void tcp_connection::close()
{
	if (::close(m_socket_fd) != 0) {
		std::cerr << "Failed to close socket. Assuming it's already closed" << std::endl;
	} else
		std::cout << "Closed a socket" << std::endl;
}

// TODO: Doesn't work
std::fstream tcp_connection::stream() const
{
	// auto* f = fdopen(m_socket_fd, "a");
	// std::fstream fs(f, std::ios_base::in | std::ios_base::out);
	// return fs;
	return {};
}

std::vector<utils::byte> tcp_connection::read(unsigned int len)
{
	std::vector<utils::byte> buffer;
	buffer.resize(len);
	auto read_len = ::recv(m_socket_fd, buffer.data(), len, false);
	if (read_len < 0) {
		perror("recv");
		throw utils::exception("Read error!");
	}
	return buffer;
}

void tcp_connection::write(std::vector<unsigned char> data)
{
	if (::send(m_socket_fd, data.data(), data.size(), false) < 0) {
		perror("send");
		throw utils::exception("Send error!");
	}
}

void tcp_connection::write_byte(utils::byte data)
{
	if (::send(m_socket_fd, &data, sizeof(utils::byte), false) < 0) {
		perror("send");
		throw utils::exception("Send error!");
	}
}

utils::byte tcp_connection::read_byte()
{
	utils::byte buff[1];
	if (::recv(m_socket_fd, buff, 1, false) < 0) {
		perror("recv");
		throw utils::exception("Read error!");
	}
	return buff[0];
}
