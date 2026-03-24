#include "VerifyGrpcClient.h"
#include "ConfigMgr.h"
VerifyGrpcClient::VerifyGrpcClient() {
	auto& gCfMgr = ConfigMgr::Inst();
	std::string host = gCfMgr["VerifyServer"]["Host"];
	std::string port = gCfMgr["VerifyServer"]["Port"];
	pool_.reset(new RPConPool(5, host, port));
}