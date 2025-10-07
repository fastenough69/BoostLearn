#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <string>
#include <functional>
#include <set>

using namespace boost::asio;
using namespace ip;
using socket_ptr = std::shared_ptr<tcp::socket>;
using func_hand = void(*)(socket_ptr sock, tcp::acceptor& ac, const boost::system::error_code& code);

void handle_accept(socket_ptr sock, tcp::acceptor& ac, const boost::system::error_code& code);

class BoostServer {
	std::weak_ptr<io_context> context_ptr;
	int port;
	tcp::endpoint ep_;
	tcp::acceptor ac_;
	socket_ptr sock_ptr = nullptr;
	bool accepted = false;
	std::set<tcp::socket> clients;

	void swap(BoostServer copy) noexcept;

public:
	BoostServer() = default;
	BoostServer(std::shared_ptr<io_context> con, const int& port);
	BoostServer(const BoostServer& right) = delete;
	BoostServer& operator=(const BoostServer& right) = delete;
	BoostServer(BoostServer&& right) noexcept;
	BoostServer& operator=(BoostServer&& right) noexcept;
	void socket_accept(func_hand func);
	inline bool is_acepted() const noexcept;
	void server_run() noexcept;
};