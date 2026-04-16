#pragma once
#include "Singleton.h"
#include <queue>
#include <thread>
#include "CSession.h"

#include <map>
#include <functional>
#include "const.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <unordered_map>
#include "data.h"

class CServer;
typedef  function<void(shared_ptr<CSession>, const short& msg_id, const string& msg_data)> FunCallBack;
class LogicSystem :public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	void PostMsgToQue(shared_ptr < LogicNode> msg);
private:
	LogicSystem();
	void DealMsg();
	void RegisterCallBacks();
	void LoginHandler(shared_ptr<CSession> session, const short& msg_id, const string& msg_data);
	std::thread _worker_thread;
	std::queue<shared_ptr<LogicNode>> _msg_que;
	std::mutex _mutex;
	std::condition_variable _consume;
	void SearchInfo(std::shared_ptr<CSession>, const short& msg_id, const string& msg_data);
	bool _b_stop;
	bool isPureDigit(const std::string& str);
	std::map<short, FunCallBack> _fun_callbacks;
	bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);
	void GetUserByUid(std::string uid_str, Json::Value& rtvalue);
	void GetUserByName(std::string uid_str, Json::Value& rtvalue);
	void AddFriendApply(std::shared_ptr<CSession> session, const short& msg_id, const string& msg_data);
};

