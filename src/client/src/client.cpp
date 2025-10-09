#include "client.hpp"

using namespace boost::asio;
using namespace boost::asio::ip;

BoostClientTcp::BoostClientTcp(std::shared_ptr<io_context> con, const std::string& host, const std::string& port) :
    BoostClientAny(con, host, port), resolver_(*con)
{
    endpoints_ = resolver_.resolve(info.host, info.port);
    sock_ptr = std::make_shared<tcp::socket>(*context_.lock());
    try_connect();
}

void BoostClientTcp::try_connect()
{
    async_connect(*sock_ptr, endpoints_, 
        [this](const boost::system::error_code& ec, const tcp::endpoint& ep) 
        {
            if (!ec)
            {
                std::cout << "Connected\n";
                this->accept = true;
                this->send_message();
                std::thread([this]() {  this->read_message(); }).detach();
            }
            else
            {
                std::cerr << ec.message() << std::endl;
                return;
            }
        });
}

void BoostClientTcp::read_message()
{
    auto mess = std::make_shared<std::vector<char>>(1024);
    async_read(*sock_ptr, buffer(mess->data(), mess->size()),
        [this, mess](const boost::system::error_code& ec, std::size_t len)
        {
            if (!ec)
            {
                std::cout << mess->data() << std::endl;
                read_message();
            }
            else
            {
                std::cerr << ec.message() << std::endl;
            }
        }
    );
}

void BoostClientTcp::send_message()
{
    std::string line;
    while(sock_ptr->is_open())
    {
        if (!std::getline(std::cin, line))
        {
            break;
        }
        
        if (line.empty()) continue;

        auto mess = std::make_shared<std::string>(std::move(line));
        async_write(*sock_ptr, buffer(*mess),
            [this, mess](const boost::system::error_code& ec, std::size_t len)
            {
                if (ec)
                {
                    std::cerr << ec.message() << std::endl;
                }
            }
        );
    }

    if (sock_ptr && sock_ptr->is_open())
    {
        sock_ptr->close();
    }
}

int main() {
    auto con = std::make_shared<io_context>();
    BoostClientTcp client(con, std::string("127.0.0.1"), std::string("2020"));
    con->run();
    return 0;
}