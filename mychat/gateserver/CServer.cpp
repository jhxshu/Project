#include "CServer.h"
#include "HTTPConnection.h"
CServer::CServer(boost::asio::io_context& ioc, unsigned short& port) : _ioc(ioc), _acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _socket(ioc) {


}

void CServer::Start() {
	auto self = shared_from_this();  //如果直接用智能指针会出现两个智能指针管理一个对象
	_acceptor.async_accept(_socket, [self](beast::error_code ec) {
		try {
			//出错就放弃连接， 继续监听其他连接
			if (ec) {
				self->Start();
				return;
			}
			//创建新连接， 并创建HTTPConnection类管理连接
			std::make_shared<HttpConnection>(std::move(self->_socket))->Start();

			//继续监听
			self->Start();
		}
		catch (std::exception& exp) {

		}
		});
}