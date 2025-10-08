#include "client.hpp"

using namespace boost::asio;
using namespace boost::asio::ip;

BoostClientTcp::BoostClientTcp(std::shared_ptr<io_context> con, const std::string& host, const std::string& port) :
    BoostClientAny(con, host, port), resolver_(*con)
{
    endpoints_ = resolver_.resolve(info.host, info.port);
    if (endpoints_.empty())
    {
        std::cerr << "Endpoints is not resolve\n";
        return;
    }
    else
    {
        std::cout << "Resolved addresses:" << std::endl;
    }
    sock_ptr = std::make_shared<tcp::socket>(*context_.lock());
}

void BoostClientTcp::try_connect()
{
    try
    {
        connect(*sock_ptr, endpoints_);
        accept = true;
        std::cout << "Connected\n";
    }
    catch (...)
    {
        std::cerr << "No connected\n";
        return;
    }
}

void BoostClientTcp::read_message()
{
    if (!is_accepted())
    {
        std::cerr << "Cant reading message\n";
        return;
    }
    streambuf buff;
    boost::system::error_code ec;
    read(*sock_ptr, buff, ec);

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

void BoostClientTcp::send_message()
{
    while (true)
    {
        std::string buff;
        boost::system::error_code ec;
        std::getline(std::cin, buff);
        if (buff.empty())
        {
            return;
        }

        write(*sock_ptr, buffer(buff), ec);
        if (!ec)
        {
            std::cout << "Message sucessful delivery!\n";
        }
        else
        {
            std::cerr << "Message not delivery to reviver\n";
            return;
        }
        read_message();
    }
}

int main() {
    auto con = std::make_shared<io_context>();
    BoostClientTcp client(con, std::string("127.0.0.1"), std::string("2020"));
    client.try_connect();
    client.read_message();
    client.send_message();
    return 0;
}

