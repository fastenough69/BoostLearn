#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <functional>
#include <string>

using namespace boost::asio;
using namespace boost::asio::ip;
using socket_ptr = std::shared_ptr<tcp::socket>;

static void handle_accept(socket_ptr sock, tcp::acceptor& ac, const boost::system::error_code& code)
{
	if (code) return;
	std::cout << "Sock is accepted\n";
	static const std::string msg("Privet boost!\n");
	async_write(*sock, buffer(msg),
		[sock](const boost::system::error_code& ec, std::size_t /*length*/)
		{
			if (!ec) {
				std::cout << "Message sent successfully\n";
			}
			else {
				std::cout << "Send error: " << ec.message() << "\n";
			}
		});
	auto new_sock = std::make_shared<tcp::socket>(ac.get_executor());
	ac.async_accept(*new_sock, std::bind(handle_accept, new_sock, std::ref(ac), std::placeholders::_1));
}

int main(int argc, char** argv)
{	
	io_context ser;
	tcp::endpoint ep(ip::address_v4::any(), 2020);
	tcp::acceptor ac(ser, ep);
	socket_ptr sc_ptr = std::make_shared<tcp::socket>(ser);
	ac.async_accept(*sc_ptr, std::bind(handle_accept, sc_ptr, std::ref(ac), std::placeholders::_1));
	std::cout << "server is up in port 2020\n";
	ser.run();
	return 0;
}