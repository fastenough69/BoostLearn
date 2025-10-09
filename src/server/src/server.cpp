#include "server.hpp"

BoostServer::BoostServer(std::shared_ptr<io_context> con, const int& port): 
	context_ptr(con), port(port), ep_(ip::address_v4::any(), this->port), ac_(*(context_ptr.lock()), ep_)
{
	sock_ptr = std::make_shared<tcp::socket>(*(context_ptr.lock()));
	socket_accept();
	server_run();
}

BoostServer::~BoostServer()
{
	sock_ptr->close();
}

void BoostServer::socket_accept()
{
	auto new_sock = std::make_shared<tcp::socket>(*context_ptr.lock());
	ac_.async_accept(*new_sock,
		[this, new_sock](const boost::system::error_code& ec)
		{
			if (!ec)
			{
				std::cout << "New user join in chat\n";
				clients.insert(new_sock);
				broadcast(std::string("New user join in chat"), new_sock);
				start_reading(new_sock);
			}
			socket_accept();
		}
	);
}

void BoostServer::broadcast(const std::string& mess, socket_ptr reciver)
{
	auto msg = std::make_shared<std::string>(mess);
	for (auto& client : clients)
	{
		if (client != reciver)
		{
			async_write(*client, buffer(*msg),
				[msg](const boost::system::error_code& ec, std::size_t bytes_transferred)
				{
					if (!ec)
					{
						std::cout << "Message successfuly delivery\n";
					}
					else
					{
						std::cerr << ec.message() << std::endl;
					}
				}
			);
		}
	}
}

inline bool BoostServer::is_acepted() const noexcept
{
	return accepted;
}

void BoostServer::server_run() noexcept
{
	std::cout << "server is up in port " << port << std::endl;
	context_ptr.lock()->run();
}

void BoostServer::start_reading(socket_ptr sock)
{
	auto buff = std::make_shared<std::vector<char>>(1024);
	sock->async_read_some(buffer(buff->data(), buff->size()), 
		[this, buff, sock](const boost::system::error_code& ec, std::size_t len)
		{
			if (!ec)
			{	
				std::string mess(buff->data(), len);
				broadcast(mess);
				start_reading(sock);
			}
			else
			{
				sock->close();
				clients.erase(sock);
				broadcast("User leave this chat", sock);
			}
		});
}

int main(int argc, char** argv)
{
	auto con_ptr = std::make_shared<io_context>();
	BoostServer serv(con_ptr, 2020);
	return 0;
}