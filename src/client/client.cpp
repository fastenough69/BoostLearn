#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <string>
#include <functional>
#include <streambuf>

using namespace boost::asio;
using namespace boost::asio::ip;

int main() {
    io_context service;
    tcp::resolver resolver(service);

    tcp::resolver::results_type endpoints = resolver.resolve("localhost", "80");

    std::cout << "Resolved addresses:" << std::endl;
    for (const auto& result : endpoints)
    {
        tcp::endpoint ep = result.endpoint();
        std::cout << ep.address().to_string() << std::endl;
        auto sock = std::make_shared<tcp::socket>(service);
        sock->connect(ep);
        std::cout << "Connected to server\n";
    }
    return 0;
}