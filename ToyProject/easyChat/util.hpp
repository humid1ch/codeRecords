#pragma once

#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include "logMessage.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>

using namespace boost::archive::iterators;

class BoostBase64 {
public:
	// 对 char 数组进行 Base64 编码
	static std::string encodeBase64C(const char* data, std::size_t length) {
		typedef base64_from_binary<transform_width<const char*, 6, 8>> Base64EncodeIterator;

		std::string encoded(Base64EncodeIterator(data), Base64EncodeIterator(data + length));

		return encoded.append((3 - length % 3) % 3, '=');
	}

	static std::string decodeBase64(const std::string& input) {
		typedef transform_width<binary_from_base64<std::string::const_iterator>, 8, 6> Base64DecodeIterator;

		// 处理输入字符串，去除可能存在的填充字符 '='
		std::string cleanedInput = input;
		boost::algorithm::trim_right_if(cleanedInput, boost::is_any_of("="));
		std::cout << "Cleaned Input: " << cleanedInput << std::endl;
		std::cout << "Cleaned Input Length: " << cleanedInput.length() << std::endl;
		try {
			return std::string(Base64DecodeIterator(cleanedInput.begin()), Base64DecodeIterator(cleanedInput.end()));
		}
		catch (...) {
			throw std::runtime_error("Failed to decode Base64 string");
		}
	}

	static std::string decodeBase64C(const char* data) {
		typedef transform_width<binary_from_base64<std::string::const_iterator>, 8, 6> Base64DecodeIterator;

		// 处理输入字符串，去除可能存在的填充字符 '='
		std::string cleanedInput(data);
		boost::algorithm::trim_right_if(cleanedInput, boost::is_any_of("="));
		try {
			return std::string(Base64DecodeIterator(cleanedInput.begin()), Base64DecodeIterator(cleanedInput.end()));
		}
		catch (...) {
			throw std::runtime_error("Failed to decode Base64 string");
		}
	}
};

class jsonUtil {
	// 需要实现 数据的序列化与反序列化
public:
	// 序列化, 将Json::Value 内容, 序列化为字符串并设置到 body中
	static bool serialize(const Json::Value& root, std::string* body) {
		Json::StreamWriterBuilder swb;
		std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());

		std::stringstream ss;
		int ret = sw->write(root, &ss); // 写入字符串流中
		// 源码中 StreamWriter::write() 返回值, 写入成功返回0
		if (ret != 0) {
			LOG(WARNING, "jsonUtil::serialize():: StreamWriter::write() failed!");
			return false;
		}

		*body = ss.str();

		return true;
	}

	static bool deserialize(const std::string& body, Json::Value* root) {
		Json::CharReaderBuilder crb;
		std::unique_ptr<Json::CharReader> cr(crb.newCharReader());

		std::string err;
		bool ret = cr->parse(body.c_str(), body.c_str() + body.size(), root, &err);
		// 源码中, CharReader::parse() 的接口返回值是 bool 类型的.
		if (!ret) {
			// LOG(WARNING, "jsonUtil::deserialize():: CharReader::parse() failed!");
			return false;
		}

		return true;
	}
};

bool setNoBlock(int fd) {
	// 获取文件描述符的 原有属性
	int flag = fcntl(fd, F_GETFL);
	if (flag == -1) {
		return false;
	}

	// 设置文件描述符属性
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);

	return true;
}
