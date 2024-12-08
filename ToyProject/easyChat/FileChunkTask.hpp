#pragma once

#include <json/json.h>
#include <json/value.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "logMessage.hpp"
#include "util.hpp"

class FileChunkTask {
public:
	// 块大小, 设置成1MB吧
	FileChunkTask(const std::string account, const std::string& filePath = "", size_t startOffset = 0,
				  size_t chunkSize = 512 * 1024, int partNum = 0)
		: _account(account)
		, _filePath(filePath)
		, _startOffset(startOffset)
		, _chunkSize(chunkSize)
		, _partNum(partNum) {

		LOG(DEBUG, "account: %s, filePath: %s, chunkSize: %d, chunkNum: %d", _account.c_str(), _filePath.c_str(), _chunkSize,
			_partNum);
	}

	void run() {
		// 打开文件
		int fd = open(_filePath.c_str(), O_RDWR);
		if (fd == -1) {
			LOG(FATAL, "open the file %s failed! errno: %d, desc: %s", _filePath.c_str(), errno, strerror(errno));
			return;
		}

		// 移动到偏移量位置
		if (lseek(fd, _startOffset, SEEK_SET) == -1) {
			LOG(FATAL, "set file %s offset failed! errno: %d, desc: %s", _filePath.c_str(), errno, strerror(errno));
			close(fd);
			return;
		}

		// 读取指定大小的数据块
		char* fileData = new char[_chunkSize];
		ssize_t res = read(fd, fileData, _chunkSize);
		LOG(DEBUG, "NO SER fileData size: %d", res);

		std::string fileDataBase64 = BoostBase64::encodeBase64C(fileData, res);
		if (res > 0) {
			// 序列化数据
			Json::Value fileAsk;
			fileAsk["filePath"] = _filePath;
			fileAsk["startOffset"] = _startOffset;
			fileAsk["data"] = fileDataBase64; // CString

			LOG(DEBUG, "Send filePath: %s, startOffset: %u, fileDataStr: %s", fileAsk["filePath"].asString().c_str(),
				fileAsk["startOffset"].asUInt(), fileAsk["data"].asCString());

			std::string fileAskStr;
			jsonUtil::serialize(fileAsk, &fileAskStr);

			std::string fileAskSizeStr;
			Json::Value fileAskSize;
			fileAskSize["size"] = fileAskStr.size();
			jsonUtil::serialize(fileAskSize, &fileAskSizeStr);
			std::cout << "fileAskSize: " << fileAskStr.size() << std::endl;

			// 发送大小
			int res = send(_socketFd, fileAskSizeStr.c_str(), fileAskSizeStr.size(), 0);

			// 发送数据
			int sendSize = 0;
			while (sendSize < fileAskStr.size()) {
				res = send(_socketFd, (char*)fileAskStr.c_str() + sendSize, _chunkSize, 0);

				std::cout << "sock: " << _socketFd << ", send data addr" << static_cast<const void*>((char*)fileAskStr.c_str() + sendSize) << ", send data size: " << res << std::endl;
				sendSize += res;
			}

			delete[] fileData;
		}
		if (res == -1) {
			LOG(FATAL, "read file %s failed! errno: %d, desc: %s", _filePath.c_str(), errno, strerror(errno));
			close(fd);
			delete[] fileData;

			return;
		}
	}

	void setSocketFd(int socketFd) {
		_socketFd = socketFd;
	}

private:
	std::string _account;  // 目标用户
	std::string _filePath; // 文件路径
	size_t _startOffset;   // 文件起始偏移量
	size_t _chunkSize;	   // 文件的块大小, 单位字节
	int _partNum;		   // 第几块
	int _socketFd;		   // 用于接收线程维护的连接
};
