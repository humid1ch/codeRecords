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
#include <windows.h>		//�˳���������ͷ�ļ�
#include <string.h>			//ͨѶ¼��ʼ������ (memset) ����ͷ�ļ�	��  �����ַ���
#include <stdlib.h>			//��̬�ڴ溯��ͷ�ļ�
#include <errno.h>

// ��ϵ����Ϣ �ṹ��
typedef struct PeoInfo
{
	char name[Name_Max];
	int age;
	char sex[Sex_Max];
	char tele[Tele_Max];
	char addr[Addr_Max];
	char remark[Rem_Max];
}PeoInfo;
// ͨѶ¼�ṹ��
typedef struct Contact
{
	PeoInfo* Info;
	int peo_num;
	int Contact_capacity;
}Contact;

// �˵�����
void menu();	

// �˳�����
void exits();

// ͨѶ¼��ʼ������
void InitCon(Contact* con);

// �����ϵ�˺���
void AddPeople(Contact* con);

// ��ʾͨѶ¼����
void ShowContact(const Contact* con);

// ɾ����ϵ�˺���
void DelePeopel(Contact* con);

// ���Ҳ������ϵ�˺���
void SearchPeo(Contact* con);

// ����������ͨѶ¼����
void SortContact_Name(Contact* con);

// �޸���ϵ�˺���
void ModifyPeo(Contact* con);

// ���ͨѶ¼����
void ClearContact(Contact* con);

//����ͨѶ¼
void DestroyContact(Contact* con);