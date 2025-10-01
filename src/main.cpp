#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using namespace boost::asio;
using namespace boost::asio::ip;
using socket_ptr = std::shared_ptr<tcp::socket>;

int main(int argc, char** argv)
{	
	io_context ser;
	tcp::endpoint ep(tcp::v4(), 2020);
	tcp::acceptor ac(ser, ep);
	socket_ptr sc_ptr = std::make_shared<tcp::socket>(ser);
	sc_ptr->async_connect(ep);
	ser.run();

	return 0;
}