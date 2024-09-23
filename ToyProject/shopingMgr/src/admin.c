#include "../inc/admin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ADMIN 50

Admin* admins = NULL;
int adminNum = 0;

int isLogin = 0;

void adminInit() {
	if (admins == NULL) {
		// 初始化一下管理员列表
		admins = (Admin*)malloc(sizeof(Admin) * MAX_ADMIN);
		strcpy(admins[0]._account, "admin");
		strcpy(admins[0]._passwd, "admin");
		adminNum++;
	}
}

void regAdmin() {
	Admin admin;
	printf("请输入用户名(6~18位):> ");
	scanf("%s", admin._account);
	printf("请输入密码(6~18位):> ");
	scanf("%s", admin._passwd);

	strcpy(admins[adminNum]._passwd, admin._passwd);
	strcpy(admins[adminNum]._account, admin._account);
	adminNum++;

	printf("注册成功\n");
}

// 验证成功 返回0, 否则返回-1
int verifyPasswd(const char* account, const char* passwd) {
	int index = -1;
	for (int i = 0; i < adminNum; i++) {
		if (strcmp(admins[i]._account, account) == 0) {
			index = i;
			break;
		}
	}

	if (index > -1) {
		if (strcmp(admins[index]._passwd, passwd) == 0) {
			return 0;
		}
	}

	return -1;
}

int modifyAmdinPasswd(const char* account) {
	int index = -1;
	for (int i = 0; i < adminNum; i++) {
		if (strcmp(admins[i]._account, account) == 0) {
			index = i;
			break;
		}
	}

	if (index > -1) {
		printf("请输入新密码(6~18位):> ");
		char passwd[20] = {0};
		scanf("%s", passwd);

		strcpy(admins[index]._passwd, passwd);

		return 0;
	}

	return -1;
}

int loginAdmin() {
	char account[20];
	char passwd[20];
	printf("请输入用户名:> ");
	scanf("%s", account);
	printf("请输入密码:> ");
	scanf("%s", passwd);

	if (verifyPasswd(account, passwd) == 0) {
        isLogin = 1;            // 更新登录状态

		return 0;
	}

	return -1;
}

void modifyAmdin() {
    if (!isLogin) {
        printf("请先登录任意管理员账号\n");
        return;
    }
	char adminAcc[20];
	printf("请输入管理员用户名:> ");
	scanf("%s", adminAcc);
	if (modifyAmdinPasswd(adminAcc) == 0) {
		printf("修改成功\n");
	}
	else {
		printf("查找不到该管理员账户\n");
	}
}
