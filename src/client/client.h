#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <functional>

using namespace boost::asio;
using namespace boost::asio::ip;
using socket_ptr = std::shared_ptr<tcp::socket>;