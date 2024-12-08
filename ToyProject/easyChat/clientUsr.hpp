#ifndef __CLIENT_USER_H__
#define __CLIENT_USER_H__

#include <cstdint>
#include <cstdlib>
#include <string>

// 用户结构体, 用于记录 已登录客户端的用户信息
struct ClientUser {
	ClientUser(const std::string& ip = "", uint16_t port = -1, const std::string& account = "")
		: _ip(ip)
		, _port(port)
		, _account(account) {}

	std::string _ip;
	uint16_t _port;
	std::string _account;
};

struct PrivateChat {
	PrivateChat(const std::string& user1Account, const std::string& user2Account, int user1Fd = -1, int user2Fd = -1)
		: _user1Fd(user1Fd)
		, _user1Account(user1Account)
		, _user2Fd(user2Fd)
		, _user2Account(user2Account) {}

	int _user1Fd;
    std::string _user1Account;
	int _user2Fd;
    std::string _user2Account;
};

#endif
