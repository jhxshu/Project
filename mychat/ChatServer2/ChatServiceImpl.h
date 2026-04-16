#pragma once
#include "const.h"
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <mutex>
#include "data.h"

using grpc::Channel;
using grpc::Status;

using message::AddFriendReq;
using message::AddFriendRsp;
using grpc::ServerContext;
using message::AuthFriendReq;
using message::AuthFriendRsp;
using grpc::Status;
using grpc::ServerBuilder;

using message::GetChatServerReq;
using message::LoginRsp;
using message::LoginReq;
using message::ChatService;

using message::TextChatMsgReq;
using message::TextChatMsgRsp;
using message::TextChatData;

class ChatServiceImpl final: public ChatService::Service
{
public:
	ChatServiceImpl();
	Status NotifyAddFriend(ServerContext* context, const AddFriendReq* request,
		AddFriendRsp* reply) override;

	Status NotifyAuthFriend(ServerContext* context,
		const AuthFriendReq* request, AuthFriendRsp* response) override;

	Status NotifyTextChatMsg(::grpc::ServerContext* context,
		const TextChatMsgReq* request, TextChatMsgRsp* response) override;

	bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);
private:

};

