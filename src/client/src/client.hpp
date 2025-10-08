#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <string>
#include <functional>
#include <sstream>

using namespace boost::asio;
using namespace boost::asio::ip;
using socket_ptr = std::shared_ptr<tcp::socket>;

class BoostClientAny {
protected:
	struct host_info {
		std::string host;
		std::string port;
	};
	std::weak_ptr<io_context> context_;
	host_info info;
	
public:
	BoostClientAny(std::shared_ptr<io_context> con, const std::string& host, const std::string& port) : context_(con), info{ host, port }
		{	}
	virtual ~BoostClientAny() = default;
	virtual void read_message() = 0;
	virtual void send_message() = 0;
	BoostClientAny(const BoostClientAny& right) = delete;
	BoostClientAny& operator=(const BoostClientAny& right) = delete;
};

class BoostClientTcp: public BoostClientAny {
	tcp::resolver resolver_;
	tcp::resolver::results_type endpoints_;
	socket_ptr sock_ptr;
	bool accept = false;
public:
	BoostClientTcp(std::shared_ptr<io_context> con, const std::string& host, const std::string& port);
	~BoostClientTcp() { sock_ptr->close(); }
	BoostClientTcp(const BoostClientTcp&) = delete;
	BoostClientTcp& operator=(const BoostClientTcp&) = delete;
	BoostClientTcp(BoostClientTcp&& right) noexcept;
	BoostClientTcp& operator=(BoostClientTcp&& right) noexcept;
	void try_connect();
	inline bool is_accepted() const { return accept; }
	virtual void read_message() override;
	virtual void send_message() override;
};