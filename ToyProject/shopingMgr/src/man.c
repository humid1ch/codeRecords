#include "../inc/man.h"
#include "../inc/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MAN 100

Man* mans;

int manNum = 5;

Man defaultMans[5] = {{8042, "韩金轮", 12333333333, 823.2, "1997-8-8"},
					  {8043, "红皮鸭", 12244444444, 83.2, "1998-9-9"},
					  {8044, "维塔斯", 15566666666, 102.3, "1999-1-1"},
					  {8045, "钢琴家", 15111111111, 452.5, "2000-2-2"},
					  {8046, "阿克曼", 10086666666, 219.2, "2001-3-3"}};

void mansInit() {
	mans = (Man*)malloc(sizeof(Man) * MAX_MAN);
	memset(mans, 0, sizeof(Man) * MAX_MAN);

	for (int i = 0; i < manNum; i++) {
		mans[i]._id = defaultMans[i]._id;
		mans[i]._integ = defaultMans[i]._integ;
		mans[i]._phone = defaultMans[i]._phone;
		strcpy(mans[i]._birth, defaultMans[i]._birth);
		strcpy(mans[i]._name, defaultMans[i]._name);
	}
}

void regMan() {
	Man man;
	printf("请输入客户姓名:> ");
	scanf("%s", man._name);
	printf("请输入客户电话:> ");
	scanf("%lld", &man._phone);
	clearInputBuffer();
	printf("请输入客户出生日期:> ");
	scanf("%s", man._birth);
	man._id = mans[manNum - 1]._id + 1; // _id设置为上一个客户id+1
	man._integ = 0.0;					// 积分初始值为0

	mans[manNum]._id = man._id;
	mans[manNum]._integ = man._integ;
	mans[manNum]._phone = man._phone;
	strcpy(mans[manNum]._name, man._name);
	strcpy(mans[manNum]._birth, man._birth);
	manNum++;
}

void printManInfo(const Man* man) {
	printf("%d\t%s\t\t%lld\t%.1f\t%s\n", man->_id, man->_name, man->_phone,
		   man->_integ, man->_birth);
}

void modifyManInfo(Man* man) {
    printf("查询到用户信息\n");
	printf("ID\t用户名\t\t电话\t\t积分\t生日\n");
	printManInfo(man);

	printf("修改客户姓名:> ");
	scanf("%s", man->_name);
	printf("修改客户电话:> ");
	scanf("%lld", &man->_phone);
	clearInputBuffer();
	printf("修改客户积分:> ");
	scanf("%f", &man->_integ);
	printf("修改客户生日(年-月-日):> ");
	scanf("%s", man->_birth);
}

Man* queryManByName(const char* name) {
	for (int i = 0; i < manNum; i++) {
		if (strcmp(mans[i]._name, name) == 0) {
			return mans + i;
		}
	}

	return NULL;
}

Man* queryManByPhone(long long phone) {
	for (int i = 0; i < manNum; i++) {
		if (mans[i]._phone == phone) {
			return mans + i;
		}
	}

	return NULL;
}

Man* queryManByID(int id) {
	for (int i = 0; i < manNum; i++) {
		if (mans[i]._id == id) {
			return mans + i;
		}
	}

	return NULL;
}

void showMans() {
	printf("\n==购物管理系统 > 主菜单 > 客户信息管理 > 所有客户信息==\n");
	printf("ID\t用户名\t\t电话\t\t积分\t生日\n");
	for (int i = 0; i < manNum; i++) {
		printManInfo(mans + i);
	}
}

void modifyMan() {
	while (1) {
		printf("\n==购物管理系统 > 主菜单 > 客户信息管理 > 修改客户信息==\n");
		printf("|              1. 根据姓名查询并修改信息              |\n");
		printf("|              2. 根据电话查询并修改信息              |\n");
		printf("|                   0. 返回上一级                     |\n");
		printf("=======================================================\n");

		char name[20] = {0};
		long long phone = 0;
		Man* tmpMan = NULL;

		int select = -1;
		printf("请选择选项:> ");
		scanf("%d", &select);
		clearInputBuffer();

		switch (select) {
		case 1:
			printf("请输入客户姓名:> ");
			scanf("%s", name);

			tmpMan = queryManByName(name);
			if (tmpMan != NULL) {
				modifyManInfo(tmpMan);
			}
			else {
				printf("未找到 %s 的相关信息\n", name);
			}
			break;
		case 2:
			printf("请输入客户电话:> ");
			scanf("%lld", &phone);
			clearInputBuffer(); // 输入数据类型之后最好都清空一下输入缓冲区
                                
			tmpMan = queryManByPhone(phone);
			if (tmpMan != NULL) {
				modifyManInfo(tmpMan);
			}
			else {
				printf("未找到电话为 %lld 客户的相关信息\n", phone);
			}
			break;
		case 0:
			return;
		default:
			printf("选项非法, 请重新选择\n");
			break;
		}
	}
}

void queryMan() {
	while (1) {
		printf("\n==购物管理系统 > 主菜单 > 客户信息管理 > 查询客户信息==\n");
		printf("|                  1. 根据姓名查询                    |\n");
		printf("|                  2. 根据电话查询                    |\n");
		printf("|                  0. 返回上一级                      |\n");
		printf("=======================================================\n");

		int select = -1;
		printf("请选择选项:> ");
		scanf("%d", &select);
		clearInputBuffer();

		char name[20];
		long long phone = 0;
		Man* tmpMan = NULL;
		switch (select) {
		case 1:
			printf("请输入姓名:> ");
			scanf("%s", name);

			tmpMan = queryManByName(name);
			if (tmpMan != NULL) {
				printf("ID\t用户名\t\t电话\t\t积分\t生日\n");
				printManInfo(tmpMan);
			}
			else {
				printf("未找到 %s 的相关信息\n", name);
			}
			break;
		case 2:
			printf("请输入电话:> ");
			scanf("%lld", &phone);
			clearInputBuffer();

			tmpMan = queryManByPhone(phone);
			if (tmpMan != NULL) {
				printf("ID\t用户名\t\t电话\t\t积分\t生日\n");
				printManInfo(tmpMan);
			}
			else {
				printf("未找到电话为 %lld 客户的相关信息\n", phone);
			}
			break;
		case 0:
			return; // 返回
		default:
			printf("选项非法, 请重新选择\n");
			break;
		}
	}
}
