#ifndef _MAN_H
#define _MAN_H 1

typedef struct Man {
	int         _id;        // 客户id
	char        _name[20];  // 姓名
	long long   _phone;     // 电话, 个人觉得电话更合适一些
	float       _integ;     // 积分
	char        _birth[20]; // 生日
} Man;

void mansInit();

void regMan();

void modifyManInfo(Man* man);

Man* queryManByName(const char* name);

Man* queryManByPhone(long long phone);

Man* queryManByID(int id);

void showMans();

void printManInfo(const Man* man);

void modifyMan();

void queryMan();

#endif
