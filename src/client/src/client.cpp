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
    if (endpoints.empty())
    {
        std::cerr << "Endpoints is not resolve\n";
        return 1;
    }

    std::cout << "Resolved addresses:" << std::endl;
    tcp::socket temp_sock(service);

    try
    {
        connect(temp_sock, endpoints);
    }
    catch(...)
    {
        std::cerr << "No connected\n";
        return 1;
    }

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
    temp_sock.close();
    return 0;
}