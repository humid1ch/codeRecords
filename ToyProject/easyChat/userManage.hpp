#include <mysql/mysql.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <ostream>
#include <string>
#include "logMessage.hpp"
#include "util.hpp"

#define HOST "127.0.0.1"	 // 连接数据库地址
#define USER "root"			 // 连接数据库用户名
#define PASSWD "Dxyt200209." // 用户名密码
#define NAME "Chat_Users"	 // 数据库名

// mysqlInit() 初始化操作
// 主要实现 数据库句柄获取  数据库连接 以及 客户端字符集设置 相关操作
static MYSQL* mysqlInit() {
	MYSQL* mysql = mysql_init(NULL);
	if (mysql == NULL) {
		LOG(WARNING, "mysqlInit():: get mysql handler failed!");
		return NULL;
	}
	LOG(NOTICE, "mysqlInit():: get mysql handler success!");

	// 获取数据库句柄之后, 就可以连接数据库的
	mysql = mysql_real_connect(mysql, HOST, USER, PASSWD, NAME, 0, NULL, 0);
	if (mysql == NULL) {
		LOG(WARNING, "mysqlInit():: connect mysql server failed!");
		mysql_close(mysql);
		return NULL;
	}
	LOG(NOTICE, "mysqlInit():: connect mysql server success!");

	int ret = mysql_set_character_set(mysql, "utf8");
	if (ret != 0) {
		LOG(WARNING, "mysqlInit():: mysql client set character failed!");
		mysql_close(mysql);
		return NULL;
	}
	LOG(NOTICE, "mysqlInit():: mysql client set character success!");

	return mysql;
}

// mysqlDestroy()
// 主要实现 数据库句柄销毁的相关操作
static void mysqlDestroy(MYSQL* mysql) {
	if (mysql != NULL) {
		mysql_close(mysql);
		LOG(NOTICE, "mysql handler destroy success!");
	}
	else {
		LOG(NOTICE, "mysqlDestroy()::mysql handler does not exists, do nothing!");
	}
}

// mysqlQuery()
// 主要实现 数据库的查询操作
// 需要通过此接口实现各种查询操作
static bool mysqlQuery(MYSQL* mysql, const std::string& sql) {
	if (mysql == NULL) {
		LOG(WARNING, "mysqlDestroy()::mysql handler does not exists!");
		return false;
	}

	int ret = mysql_query(mysql, sql.c_str());
	if (ret != 0) {
		LOG(WARNING, "mysqlQuery():: sql(%s) query failed! error:: %s!", sql.c_str(), mysql_error(mysql));
		return false;
	}
	LOG(WARNING, "mysqlQuery():: sql(%s) query success!", sql.c_str());

	return true;
}

// tb_users: u_id, u_account, u_passwd
class ChatUsers {
public:
	ChatUsers() {
		_mysql = mysqlInit();
		if (_mysql == NULL) {
			LOG(FATAL, "ChatUsers():: mysqlInit() failed!, process is exiting!");
			sleep(1);
			LOG(FATAL, "process exit success!");
			exit(-1);
		}
	}
	~ChatUsers() {
		mysqlDestroy(_mysql);
	}

	// 账号: 6~16 密码:8~18
	// 插入数据, 都是反序列化之后的数据
	bool insertUser(const Json::Value& user) {
		std::string sql;
		sql.resize(128);

#define INSERT_USER "INSERT INTO tb_users(u_account, u_passwd) VALUE('%s', '%s');"
		sprintf(&sql[0], INSERT_USER, user["account"].asCString(), user["passwd"].asCString());
		// 将 sql 内容格式化为
		// INSERT INTO tb_users(u_account, u_passwd) VALUE(account, passwd)

		_mutex.lock();
		// 该执行sql了, 先上锁
		bool ret = mysqlQuery(_mysql, sql);
		_mutex.unlock();

		return ret;
	}

	// 删除操作
	bool deleteUser(const std::string& account) {
		std::string sql;
		sql.resize(128);

#define DELETE_USER "DELETE FROM tb_users WHERE u_account='%s';"
		sprintf(&sql[0], DELETE_USER, account.c_str());

		_mutex.lock();
		// 该执行sql了, 先上锁
		bool ret = mysqlQuery(_mysql, sql);
		_mutex.unlock();

		return ret;
	}

	// 查询数据操作:
	// 查询指定用户, 并获取用户键值
	bool selectUser(const std::string& account, Json::Value* user) {
		std::string sql;
		sql.resize(128);
#define SELECT_USER "SELECT * FROM tb_users WHERE u_account='%s';"
		sprintf(&sql[0], SELECT_USER, account.c_str());

		// 查询
		_mutex.lock();
		bool ret = mysqlQuery(_mysql, sql);
		if (!ret) {
			LOG(WARNING, "selectUser():: mysqlQuery(%s) failed!", sql.c_str());
			_mutex.unlock();
			return false;
		}
		// 获取结果集
		MYSQL_RES* result = mysql_store_result(_mysql);
		if (result == NULL) {
			// 只有查询失败才会为NULL
			LOG(WARNING, "selectUser():: get query result failed!");
			_mutex.unlock();
			return false;
		}
		_mutex.unlock();
		// 此次获取最多只有一行结果, 因为是根据 u_account 查询的, 此字段是唯一的
		if (mysql_num_rows(result) == 0) {
			// result 中 数据行数为0, 表示没有查到数据, 但并不是查询失败
			LOG(WARNING, "selectUser():: select nothing!");
			return false;
		}

		MYSQL_ROW data = mysql_fetch_row(result);
		(*user)["account"] = data[1];
		(*user)["passwd"] = data[2];

		mysql_free_result(result);

		return true;
	}

	// 查询用户是否存在
	bool selectUser(const std::string& account) {
		std::string sql;
		sql.resize(128);
#define SELECT_USER "SELECT * FROM tb_users WHERE u_account='%s';"
		sprintf(&sql[0], SELECT_USER, account.c_str());

		// 查询
		_mutex.lock();
		bool ret = mysqlQuery(_mysql, sql);
		if (!ret) {
			LOG(WARNING, "selectUser():: mysqlQuery(%s) failed!", sql.c_str());
			_mutex.unlock();
			return false;
		}
		// 获取结果集
		MYSQL_RES* result = mysql_store_result(_mysql);
		if (result == NULL) {
			// 只有查询失败才会为NULL
			LOG(WARNING, "selectUser():: get query result failed!");
			_mutex.unlock();
			return false;
		}
		_mutex.unlock();

		if (mysql_num_rows(result) == 0) {
			// result 中 数据行数为0, 表示没有查到数据, 但并不是查询失败
			LOG(WARNING, "selectUser():: select nothing!");
			return false;
		}

		mysql_free_result(result);

		return true;
	}

private:
	MYSQL* _mysql;
	std::mutex _mutex;
};
