#include "CServer.h"
#include "HTTPConnection.h"
#include "AsioIOServicePool.h"
CServer::CServer(boost::asio::io_context& io_context, short port) :_io_context(io_context), _port(port), _acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
	cout << "Server start success, listening on port: " << port << endl;
	StartAccept();
}

CServer::~CServer() {
	cout << "Server destruct listener on port: " << _port << endl;
}

void CServer::HandleAccept(shared_ptr<CSession> new_session, const boost::system::error_code& error) {
	if (!error) {
		new_session->Start();
		lock_guard<mutex> lock(_mutex);
		_sessions.insert(make_pair(new_session->GetUid(), new_session)); 
	}
	else {
		cout << "session accept failed, error is " << error.what() << endl;
	}
	StartAccept();
}