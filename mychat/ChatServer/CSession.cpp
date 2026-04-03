#include <iostream>
#include <sstream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "LogicSystem.h"
CSession::CSession(boost::asio::io_context& io_context, CServer* server) : _socket(io_context), _server(server),
_b_close(false), _b_head_parse(false) {
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	_uuid = boost::uuids::to_string(a_uuid);
	_recv_head_node = make_shared<MsgNode>(HEAD_TOTAL_LEN);
}

CSession::~CSession() {
	cout << "Session destruct" << endl;
}

tcp::acceptor& CSession::GetAcceptor() {
	return _socket;
}

std::string& CSession::GetUid() {
	return _uuid;
}

void CSession::Start() {
	AsyncReadHead(HEAD_TOTAL_LEN);
}

void CSession::AsyncReadHead(int total_len) {
	auto self = shared_from_this();
	asyncReadFull(HEAD_TOTAL_LEN, [self, this](const boost::system::error_code& ec, std::size_t bytes_transfered) {
		try {
			if (ec) {
				std::cout << "handle read failed, error is " << ec.what() << std::endl;
				Close();
				_server->ClearSession(_uuid);
				return;
			} 
		}
		})
}