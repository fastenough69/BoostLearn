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

BoostClientTcp::~BoostClientTcp()
{
    sock_ptr->close();
    if (write_th.joinable()) write_th.join();
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
                write_th = std::thread([this]() { this->send_message(); });
                this->read_message();
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
    auto mess = std::make_shared<std::string>();
    async_read_until(*sock_ptr, dynamic_buffer(*mess), '\n',
        [this, mess](const boost::system::error_code& ec, std::size_t len)
        {
            if (!ec)
            {
                std::cout << "Message user: " << mess->substr(0, len - 1) << std::endl;
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
    BoostClientTcp client(con, std::string("46.173.18.82"), std::string("2020"));
    con->run();
    return 0;
}