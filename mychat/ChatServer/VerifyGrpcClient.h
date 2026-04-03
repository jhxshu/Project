#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVerifyReq;
using message::GetVerifyRsp;
using message::VerifyService;

class RPConPool {
public:
	RPConPool(size_t poolsize, std::string host, std::string port) : poolSize_(poolsize), host_(host), port_(port), b_stop_(false) {
		for (size_t i = 0; i < poolSize_; ++i) {
			std::shared_ptr<Channel> Channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());	
			connections_.push(VerifyService::NewStub(Channel)); //uniqueptr不支持拷贝构造函数
		}
	}
	~RPConPool() {
		std::lock_guard<std::mutex> lock(mutex_);
		while(!connections_.empty()) {
			connections_.pop();  //还没有回收线程
		}
	}

	void Close() {
		b_stop_ = true;
		cond_.notify_all();
	}

	std::unique_ptr<VerifyService::Stub> getConnection() {
		std::unique_lock<std::mutex> lock(mutex_);
		cond_.wait(lock, [this] { return !connections_.empty() || b_stop_; });
		if (b_stop_) {
			return nullptr;  //连接池已关闭
		}
		auto context = std::move(connections_.front());
		connections_.pop();  //从队列中移除连接
		return context;  
	}

	void returnConnection(std::unique_ptr<VerifyService::Stub> context) {
		std::lock_guard<std::mutex> lock(mutex_);
		if (b_stop_) {
			return;
		}
		connections_.push(std::move(context));  //将连接放回队列
		cond_.notify_one(); 
	}
private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<VerifyService::Stub>> connections_;
	std::condition_variable cond_;
	std::mutex mutex_;
};

class VerifyGrpcClient:public Singleton<VerifyGrpcClient>
{
	friend class Singleton<VerifyGrpcClient>;
public:
	GetVerifyRsp GetVerifyCode(std::string email) {
		ClientContext context;
		GetVerifyRsp reply;
		GetVerifyReq request;
		request.set_email(email);  //设置成员
		auto stub = pool_->getConnection();

		Status status = stub->GetVerifyCode(&context, request, &reply);
		if (status.ok()) {
			pool_->returnConnection(std::move(stub));  //将连接放回连接池
			return reply;
		}
		else {
			pool_->returnConnection(std::move(stub));  
			reply.set_error(ErrorCodes::RPCFailed);
			return reply;
		}
	}
private:
	VerifyGrpcClient();
	std::unique_ptr<RPConPool> pool_;
};

