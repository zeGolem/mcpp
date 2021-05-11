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
	if (!::close(m_socket_fd)) {
		throw utils::exception("Failed to close socket");
	} else
		std::cout << "closed a socket" << std::endl;
}

// TODO: Doesn't work
std::fstream tcp_connection::stream() const
{
	// auto* f = fdopen(m_socket_fd, "a");
	// std::fstream fs(f, std::ios_base::in | std::ios_base::out);
	// return fs;
	return {};
}

std::vector<unsigned char> tcp_connection::read(unsigned int len)
{
	unsigned char buff[len];
	auto read_len = ::recv(m_socket_fd, buff, len, false);
	if (read_len < 0) {
		perror("recv");
		throw utils::exception("Read error!");
	}
	std::vector<unsigned char> data(read_len);
	data.assign(buff, buff + read_len);
	return data;
}

void tcp_connection::write(std::vector<unsigned char> data)
{
	::send(m_socket_fd, data.data(), data.size(), false);
	perror("send");
}

void tcp_connection::write_byte(utils::byte data)
{
	::send(m_socket_fd, &data, sizeof(utils::byte), false);
	perror("send");
}

utils::byte tcp_connection::read_byte()
{
	utils::byte buff[1];
	::recv(m_socket_fd, buff, 1, false);
	perror("recv");
	return buff[0];
}
