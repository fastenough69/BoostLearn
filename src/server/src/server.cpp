#include "server.hpp"

BoostServer::BoostServer(std::shared_ptr<io_context> con, const int& port): 
	context_ptr(con), port(port), ep_(ip::address_v4::any(), port), ac_(*(context_ptr.lock()), ep_)
{
	sock_ptr = std::make_shared<tcp::socket>(*(context_ptr.lock()));
}

BoostServer::~BoostServer()
{
	sock_ptr->close();
}

void BoostServer::socket_accept()
{
	/*try
	{
		ac_.async_accept(*(context_ptr.lock()), std::bind(func, sock_ptr, std::ref(ac_), std::placeholders::_1));
		accepted = true;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what();
	}*/
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
	std::string buff;
	sock_ptr->async_read_some(buffer(buff), 
		[this, buff, sock](boost::system::error_code ec)
		{
			if (!ec)
			{
				broadcast(buff);
				start_reading(sock);
			}
			else
			{
				//clients.erase(sock);
				broadcast("User leave this chat");
			}
		});
}

//void handle_accept(socket_ptr sock, tcp::acceptor& ac, const boost::system::error_code& code)
//{
//	if (code) return;
//	std::cout << "Sock is accepted\n";
//	boost::system::error_code ec;
//	auto client_ep = sock->remote_endpoint(ec);
//	if (!ec)
//	{
//		std::cout << "Client addr " << client_ep.address().to_string() << std::endl;
//	}
//	else
//	{
//		std::cerr << ec.message();
//	}
//	clients.insert(sock);
//	static const std::string msg("Privet boost!\n");
//	async_write(*sock, buffer(msg),
//		[sock, &client_ep](const boost::system::error_code& ec, std::size_t /*length*/)
//		{
//			if (!ec) {
//				std::cout << "Message sent successfully\n";
//			}
//			else {
//				std::cout << "Send error: " << ec.message() << "\n";
//			}
//		});
//	auto new_sock = std::make_shared<tcp::socket>(ac.get_executor());
//	ac.async_accept(*new_sock, std::bind(handle_accept, new_sock, std::ref(ac), std::placeholders::_1));
//}

int main(int argc, char** argv)
{
	auto con_ptr = std::make_shared<io_context>();
	BoostServer serv(con_ptr, 2020);
	serv.socket_accept();
	if (serv.is_acepted())
	{
		serv.server_run();
	}
	return 0;
}