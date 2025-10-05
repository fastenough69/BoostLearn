#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <string>
#include <functional>
#include <sstream>

using namespace boost::asio;
using namespace boost::asio::ip;

int main() {
    io_context service;
    tcp::resolver resolver(service);

    tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "2020");

    std::cout << "Resolved addresses:" << std::endl;

    for (const auto& res : endpoints)
    {
        tcp::socket temp_sock(service);
        connect(temp_sock, endpoints);
        std::cout << "Connected!\n";

        streambuf buff;
        boost::system::error_code ec;
        read(temp_sock, buff, ec);

        if (ec && ec != error::eof)
        {
            std::cout << "Read error\n";
        }
        else 
        {
            std::ostringstream ss;
            ss << &buff;
            std::string mess = ss.str();
            std::cout << "Recived message: " << mess << std::endl;
        }
    }
    return 0;
}