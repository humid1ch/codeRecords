#include "../inc/util.h"
#include "../inc/admin.h"
#include "../inc/man.h"
#include "../inc/shop.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 登录状态
extern int isLogin;

// 建议每次输入完 非字符串类型, 都清空一下输入缓冲区
void clearInputBuffer() {
	char c;
	while ((c = getchar()) != EOF && c != '\n')
		;
}

// 客户信息管理
void manInfoMgr() {
	while (1) {
		printf("\n=========购物管理系统 > 主菜单 > 客户信息管理==========\n");
		printf("|                  1. 显示所有客户信息                |\n");
		printf("|                  2. 添加客户                        |\n");
		printf("|                  3. 修改客户信息                    |\n");
		printf("|                  4. 查询客户信息                    |\n");
		printf("|                  0. 返回上一级                      |\n");
		printf("=======================================================\n");

		int select = -1;
		printf("请选择选项:> ");
		scanf("%d", &select);
		clearInputBuffer();

		switch (select) {
		case 0:
			return; // 返回
		case 1:
			showMans();
			break;
		case 2:
			regMan();
			break;
		case 3:
			modifyMan();
			break;
		case 4:
			queryMan();
			break;
		default:
			printf("选项非法, 请重新选择\n");
			break;
		}
	}
}

void sysMenu() {
	while (1) {
		printf("\n================购物管理系统 > 主菜单=================\n");
		printf("|                   1. 客户信息管理                  |\n");
		printf("|                   2. 购物结算                      |\n");
		printf("|                   3. 真情回馈                      |\n");
		printf("|                   4. 注销                          |\n");
		printf("|                   0. 返回上一级                    |\n");
		printf("======================================================\n");

		int select = -1;
		printf("请选择选项:> ");
		scanf("%d", &select);
		clearInputBuffer();

		switch (select) {
		case 0:
			return;
		case 4:
			printf("注销成功\n");
			isLogin = 0; // 取消登录状态, 此时无法修改管理员信息
			return;
		case 1:
			manInfoMgr();
			break;
		case 2:
			shoppingCheckout();
			break;
		case 3:
			welfare();
			break;
		default:
			printf("选项非法, 请重新选择\n");
			break;
		}
	}
}

void loginSys() {
	while (1) {
		printf("\n===============购物管理系统 > 登录系统================\n");
		printf("|                    1. 登录                         |\n");
		printf("|                    2. 注册                         |\n");
		printf("|                    0. 返回上一级                   |\n");
		printf("======================================================\n");

		int select = -1;
		printf("请选择选项:> ");
		scanf("%d", &select);
		clearInputBuffer();

		switch (select) {
		case 0:
			return;
		case 1:
			if (loginAdmin() == 0) {
				printf("登录成功\n");
				sysMenu();
			}
			else {
				printf("登陆失败\n");
				printf("请检查用户名或密码\n");
			}
			break;
		case 2:
			regAdmin();
			break;
		default:
			printf("选项非法, 请重新选择\n");
			break;
		}
	}
}

void mainMenu() {
	while (1) {
		printf("\n=================欢迎进入购物管理系统=================\n");
		printf("|                   1. 登录系统                      |\n");
		printf("|                   2. 更改管理员密码                |\n");
		printf("|                   0. 退出系统                      |\n");
		printf("======================================================\n");

		int select = -1;
		printf("请选择选项:> ");
		scanf("%d", &select);
		clearInputBuffer();

		switch (select) {
		case 0:
			printf("退出成功..\n");
			sleep(1);
			exit(0);
			break;
		case 1:
			loginSys();
			break;
		case 2:
			modifyAmdin();
			break;
		default:
			printf("选项非法, 请重新选择\n");
			break;
		}
	}
}
