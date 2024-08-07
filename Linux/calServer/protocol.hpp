#pragma once

#include <cassert>
#include <cstring>
#include <cctype>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <string>

#define CRLF "\r\n"
#define CRLF_LEN strlen(CRLF)
#define SPACE " "
#define SPACE_LEN strlen(SPACE)

#define OPS "+-*/%"

#define BUFFER_SIZE 1024

//#define MY_SELF 1

std::string encode(const std::string& inS, uint32_t len) {
	std::string encodeIn = std::to_string(len); // len
	encodeIn += CRLF;							// len\r\n
	encodeIn += inS;							// len\r\ninS
	encodeIn += CRLF;							// len\r\n\inS\r\n

	return encodeIn;
}

// strPackage: 长度字段\r\n有效载荷\r\n
std::string decode(std::string& inS, uint32_t* len) {
	assert(len);

	*len = 0;
	// 1. 确认inBuffer存在 "\r\n"
	size_t pos = inS.find(CRLF);
	if (std::string::npos == pos) {
		// 没有找到"\r\n" 表示没有一个完整的表示有效载荷长度的字段
		// 即 strPackage 不完整
		return "";
	}

	// 2. 获取长度
	// 已经获取了第一个"\r\n"位置, 前面即为长度字段
	std::string inLen = inS.substr(0, pos);
	int intLen = atoi(inLen.c_str());

	// 3. 确认有效载荷完整
	// 已经获取了有效载荷的长度, 就可以判断有效载荷是否完整了
	int surplus = inS.size() - 2 * CRLF_LEN - pos; // 计算inBuffer中 减去长度字段的长度 再减去两个"\r\n", 得到inBuffer的剩余长度
	if (surplus < intLen) {
		// inBuffer剩余长度小于长度字段所记录的长度, 表示有效载荷不完整
		return "";
	}

	// 4. 获取有效载荷
	// pos 是第一个"\r\n"的位置, 之后就是有效载荷的位置
	std::string package = inS.substr(pos + CRLF_LEN, intLen);
	*len = intLen;

	// 5. 将完整的strPackage从inBuffer中剔除
	int removeLen = inLen.size() + 2 * CRLF_LEN + intLen; // 需要剔除的长度为 长度字段的长度 有效载荷的长度 以及 2个"\r\n"的长度
	inS.erase(0, removeLen);							  // std::string::erase() 第二个参数表示要删除的字符长度

	return package;
}

// 定制请求的协议
class request {
public:
	request() {}
	~request() {}

	// 序列化 -- 结构化的数据 -> 字符串
	// 我们序列化的结构是 : "_x _op _y", 即 空格分割
	void serialize(std::string* out) {
#ifdef MY_SELF
		std::string xStr = std::to_string(get_x());
		std::string yStr = std::to_string(get_y());

		*out += xStr;
		*out += SPACE;
		*out += get_op();
		*out += SPACE;
		*out += yStr;
#else
		Json::Value root;
		root["x"] = _x;	  // Json::Value 是key:value类型的结构, 这里相当于 在root中添加 key: "x" 对应 value: _x的值
		root["y"] = _y;	  // 同上
		root["op"] = _op; // 同上

		Json::FastWriter fw;
		*out = fw.write(root);
#endif
	}

	// 反序列化 -- 字符串 -> 结构化的数据
	bool deserialize(const std::string& in) {
#ifdef MY_SELF
		// in 的格式 1 + 1
		// 先查找两个空格的位置
		size_t posSpaceOne = in.find(SPACE);
		if (posSpaceOne == std::string::npos)
			return false;
		size_t posSpaceTwo = in.rfind(SPACE);
		if (posSpaceTwo == std::string::npos)
			return false;

		// 再获取三段字符串
		std::string dataOne = in.substr(0, posSpaceOne);
		std::string dataTwo = in.substr(posSpaceTwo + SPACE_LEN, std::string::npos);
		std::string oper = in.substr(posSpaceOne + SPACE_LEN,
									 posSpaceTwo - (posSpaceOne + SPACE_LEN));
		if (oper.size() != 1)
			return false; // 操作符不是一位

		_x = atoi(dataOne.c_str());
		_y = atoi(dataTwo.c_str());
		_op = oper[0];

		return true;
#else
		Json::Value root;
		Json::Reader rd;
		rd.parse(in, root); // 将使用Json序列化过的字符串, 再转换存储到 Json::Value root 中

		_x = root["x"].asInt();
		_y = root["y"].asInt();
		_op = root["op"].asInt();

		return true;
#endif
	}

	int get_x() const {
		return _x;
	}
	int get_y() const {
		return _y;
	}
	char get_op() const {
		return _op;
	}
	void set_x(int x) {
		_x = x;
	}
	void set_y(int y) {
		_y = y;
	}
	void set_op(char op) {
		_op = op;
	}

	void debug() {
		std::cout << _x << " " << _op << " " << _y << std::endl;
	}

private:
	int _x;
	int _y;
	char _op;
};

// 定制响应的协议
class response {
public:
	response()
		: _exitCode(0)
		, _result(0) {}

	~response() {}

	void serialize(std::string* out) {
#ifdef MY_SELF
		std::string exitCode = std::to_string(_exitCode);
		std::string result = std::to_string(_result);

		*out = exitCode;
		*out += SPACE;
		*out += result;
#else
		Json::Value root;
		root["exitCode"] = _exitCode;
		root["result"] = _result;

		Json::FastWriter fw;
		*out = fw.write(root);
#endif
	}

	// 反序列化
	bool deserialize(const std::string& in) {
#ifdef MY_SELF
		size_t posSpace = in.find(SPACE);
		if (posSpace == std::string::npos) {
			return false;
		}

		std::string exitCodeStr = in.substr(0, posSpace);
		std::string resultStr = in.substr(posSpace + SPACE_LEN, std::string::npos);

		_exitCode = atoi(exitCodeStr.c_str());
		_result = atoi(resultStr.c_str());

		return true;
#else
		Json::Value root;
		Json::Reader rd;
		rd.parse(in, root);

		_exitCode = root["exitCode"].asInt();
		_result = root["result"].asInt();

		return true;

#endif
	}
	void set_exitCode(int exitCode) {
		_exitCode = exitCode;
	}
	void set_result(int result) {
		_result = result;
	}
	int get_exitCode() const {
		return _exitCode;
	}
	int get_result() const {
		return _result;
	}

	void debug() {
		std::cout << _exitCode << " " << _result << std::endl;
	}

private:
	int _exitCode;
	int _result;
};

bool makeRequest(const std::string& message, request* req) {
	// 首先消除指令消息中的空格
	std::string tmpMsg;
	std::string opStr = OPS;
	for (int i = 0; i < message.size(); i++) {
		char e = message[i];
		if ((e <= '9' && e >= '0') || (std::string::npos != opStr.find(e))) {
			tmpMsg += e;
		}
		else if (e != ' ') {
			return false;
		}
	}
	// 这里要分两种情况来判断
	// 因为有可能 操作数前有 + 或 - 号
	// 所以要分清 + 或 -是否属于数值 或 运算符
	// 然后找到真正运算符的位置

	int opPos = 0;

	int first_pos = tmpMsg.find_first_of(opStr);
	int last_pos = tmpMsg.find_last_of(opStr);
	if ((tmpMsg[last_pos] != '-' && tmpMsg[last_pos] != '+') && !isdigit(tmpMsg[last_pos - 1])) {
		// 当最后一个操作符不是 - + 也不是真正运算符时, 输入错误
		return false;
	}
	if ((tmpMsg[first_pos] == '-' || tmpMsg[first_pos] == '+')) {
		if (first_pos == 0) {
			opPos = tmpMsg.find_first_of(opStr, first_pos + 1);
		}
		else {
			opPos = first_pos;
		}
	}
	else {
		if (first_pos == 0)
			return false;
		opPos = first_pos;
	}

	std::string left = tmpMsg.substr(0, opPos);
	std::string right = tmpMsg.substr(opPos + 1);

	req->set_x(atoi(left.c_str()));
	req->set_y(atoi(right.c_str()));
	req->set_op(tmpMsg[opPos]);

	return true;
}
