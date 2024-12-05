#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

namespace Util {
	// 设置非阻塞函数
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
}  // namespace Util

#endif
