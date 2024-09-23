#ifndef _ADMIN_H
#define _ADMIN_H 1

typedef struct Admin {
	char _account[20];
	char _passwd[20];
} Admin;

void adminInit();

void regAdmin();

int verifyPasswd(const char* account, const char* passwd);

int modifyAmdinPasswd(const char* account);

int loginAdmin();

void modifyAmdin();

#endif
