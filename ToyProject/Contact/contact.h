#pragma once

#define Peo_Max 200
#define Name_Max 20
#define Sex_Max 8
#define Tele_Max 15
#define Addr_Max 30
#define Rem_Max 10
#define Con_Capacity 2

#include <stdio.h>
#include <assert.h>
#include <windows.h>		//退出函数所需头文件
#include <string.h>			//通讯录初始化函数 (memset) 所需头文件	、  错误字符串
#include <stdlib.h>			//动态内存函数头文件
#include <errno.h>

// 联系人信息 结构体
typedef struct PeoInfo
{
	char name[Name_Max];
	int age;
	char sex[Sex_Max];
	char tele[Tele_Max];
	char addr[Addr_Max];
	char remark[Rem_Max];
}PeoInfo;
// 通讯录结构体
typedef struct Contact
{
	PeoInfo* Info;
	int peo_num;
	int Contact_capacity;
}Contact;

// 菜单函数
void menu();	

// 退出函数
void exits();

// 通讯录初始化函数
void InitCon(Contact* con);

// 添加联系人函数
void AddPeople(Contact* con);

// 显示通讯录函数
void ShowContact(const Contact* con);

// 删除联系人函数
void DelePeopel(Contact* con);

// 查找并输出联系人函数
void SearchPeo(Contact* con);

// 按名字排序通讯录函数
void SortContact_Name(Contact* con);

// 修改联系人函数
void ModifyPeo(Contact* con);

// 清空通讯录函数
void ClearContact(Contact* con);

//销毁通讯录
void DestroyContact(Contact* con);