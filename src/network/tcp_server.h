#pragma once
#include "tcp_connection.h"

namespace mcpp
{
namespace network
{

class tcp_server
{
  private:
	int m_socket_fd;
	int m_port;
	bool m_started;

  public:
	const static int BACKLOG = 16;

	tcp_server(int port = 25565);
	~tcp_server();

	void start();
	tcp_connection *accept();

	bool started() const { return m_started; }
};

} // namespace network
} // namespace mcpp
