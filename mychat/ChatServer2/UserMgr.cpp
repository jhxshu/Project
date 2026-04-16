#include "UserMgr.h"
#include "RedisMgr.h"
UserMgr::~UserMgr()
{
	_uid_to_session.clear();
}

std::shared_ptr<CSession> UserMgr::GetSession(int uid)
{
	std::lock_guard<std::mutex> lock(_session_mutex);
	auto iter = _uid_to_session.find(uid);
	if (iter != _uid_to_session.end()) {
		return iter->second;
	}
	return nullptr;
}

void UserMgr::SetUserSession(int uid, std::shared_ptr<CSession> session)
{
	std::lock_guard<std::mutex> lock(_session_mutex);
	_uid_to_session[uid] = session;
}

void UserMgr::RmvUserSession(int uid)
{
	auto uid_str = std::to_string(uid);
	{
		std::lock_guard<std::mutex> lock(_session_mutex);
		_uid_to_session.erase(uid);
	}
}
UserMgr::UserMgr()
{
}