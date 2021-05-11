#pragma once
#include "utils/types.h"
#include <cstddef>
#include <fstream>
#include <netinet/in.h>
#include <vector>

namespace mcpp
{
namespace network
{

class tcp_connection
{
  private:
	int m_socket_fd;
	sockaddr_in m_addr;

  public:
	tcp_connection(int fd, sockaddr_in client_addr);
	~tcp_connection() { close(); }

	void close();
	std::fstream stream() const;
	// TODO: Add a read_raw that returns the raw C char array.
	std::vector<unsigned char> read(unsigned int lenght);
	void write(std::vector<unsigned char> data);
	void write_byte(utils::byte);

	utils::byte read_byte();
};

} // namespace network
} // namespace mcpp
