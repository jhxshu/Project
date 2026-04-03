#pragma once
#include "const.h"
#include <memory> // 添加此头文件以使用 std::enable_shared_from_this

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	CServer(boost::asio::io_context& ioc, unsigned short& port);
	void Start();
private:
	tcp::acceptor _acceptor;
	net::io_context& _ioc;        
};