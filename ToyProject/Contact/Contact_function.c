#define _CRT_SECURE_NO_WARNINGS 1

#include "Contact.h"

// �˵�����
void menu()
{
	printf("*******************************************\n");
	printf("*                 ͨѶ¼                  *\n");
	printf("*******************************************\n");
	printf("*****  1.�����ϵ��	2.ɾ����ϵ��  *****\n");
	printf("*****  3.������ϵ��	4.�޸���ϵ��  *****\n");
	printf("*****  5.����������	6.��ʾͨѶ¼  *****\n");
	printf("*****  7.���ͨѶ¼	0.�˳�ͨѶ¼  *****\n");
	printf("*******************************************\n");
}

// �˳�����
void exits()
{
	printf("�����˳�ͨѶ¼\n");
	printf(". ");
	Sleep(1000);
	printf(". ");
	Sleep(1000);
	printf(". \n");
	Sleep(500);
	printf("�˳��ɹ�\n");
	Sleep(1000);
}

int cmp_name(const void* elem1, const void* elem2)
{
	if (strcmp(((PeoInfo*)elem1)->name, ((PeoInfo*)elem2)->name ) > 0)
		return 1;
	else
		return 0;
}

// ��ʼ��ͨѶ¼����
void InitCon(Contact* con)
{
	assert(con);
	PeoInfo* tmp = (PeoInfo*)malloc(Con_Capacity * sizeof(PeoInfo));
	if (tmp != NULL)
	{
		con->Info = tmp;
	}
	else
	{
		printf("ERROR_InitCon::%s\n", strerror(errno));
		return;
	}
	con->peo_num = 0;
	con->Contact_capacity = Con_Capacity;
}

//ͨѶ¼���ݺ���
void Check_Capacity(Contact* con)
{
	assert(con);

	if (con->peo_num == con->Contact_capacity)
	{
		PeoInfo* tmp = (PeoInfo*)realloc(con->Info, (con->Contact_capacity + 2) * sizeof(PeoInfo));	//ͨѶ¼���ݣ�+2��
		if (tmp != NULL)
		{
			con->Info = tmp;
			con->Contact_capacity += 2;		//���ݳɹ� ������ �� 2 
			printf("���ݳɹ�\n");
		}
		else
		{
			printf("ERROR_Check_Capacity()::%s", strerror(errno));
		}
	}
}

// �����ϵ�˺���
void AddPeople(Contact* con)
{
	assert(con);
	Check_Capacity(con);

	printf("��������ϵ������:>");
	scanf("%s", con->Info[con->peo_num].name);
	printf("��������ϵ�˱�ע:>");
	scanf("%s", con->Info[con->peo_num].remark);
	printf("��������ϵ������:>");
	scanf("%d", &(con->Info[con->peo_num].age));
	printf("��������ϵ���Ա�:>");
	scanf("%s", con->Info[con->peo_num].sex);
	printf("��������ϵ�˵绰:>");
	scanf("%s", con->Info[con->peo_num].tele);
	printf("��������ϵ��סַ:>");
	scanf("%s", con->Info[con->peo_num].addr);

	con->peo_num++;
	printf("�����ϵ�˳ɹ�\n\n");
}

// ��ʾͨѶ¼����
void ShowContact(const Contact* con)
{
	assert(con);
	printf("%-4s\t%-4s\t%-4s\t%-15s\t%s\t\n", "����(��ע)", "�Ա�", "����", "�绰", "��ͥסַ");
	for (int i = 0; i < con->peo_num; i++)
	{
		printf("%-4s(%s) \t%-4s\t%-4d\t%-15s\t%s\t\n",
			con->Info[i].name ,con->Info[i].remark, con->Info[i].sex, con->Info[i].age, con->Info[i].tele, con->Info[i].addr);
	}
	printf("\n");
}

// �򵥵Ĳ�����ϵ�˺���
int SearchPeoByName(Contact* con, char* searchname)
{
	for (int i = 0; i < con->peo_num; i++)
	{
		if (strcmp(con->Info[i].name, searchname) == 0)
			return i;
	}

	return -1;
}

// ɾ����ϵ�˺���
void DelePeopel(Contact* con)
{
	char delename[Name_Max] = { 0 };
	if (con->peo_num == 0)
	{
		printf("ͨѶ¼Ϊ�գ��޷�ɾ��\n\n");
		return;
	}
	printf("������Ҫɾ������ϵ��:>");
	scanf("%s", delename);
	int peo_n = SearchPeoByName(con, delename);
	if (peo_n == -1)
		printf("���޴��ˣ��޷�ɾ��\n\n");
	else
	{
		for (int i = peo_n; i < con->peo_num - 1; i++)
		{
			con->Info[i] = con->Info[i + 1];	//ͬ���ͽṹ�� ����ֱ�Ӹ�ֵ
		}
		con->peo_num--;
		printf("ɾ��ָ����ϵ�˳ɹ�\n\n");
	}
}

// ������ϵ�˲��������
void SearchPeo(Contact* con)
{
	char SearchPeo_name[Name_Max];
	if (con->peo_num == 0)
	{
		printf("ͨѶ¼Ϊ�գ��޷�����\n\n");
		return;
	}
	printf("������Ҫ���ҵ���ϵ��:>");
	scanf("%s", SearchPeo_name);
	int peo_n = SearchPeoByName(con, SearchPeo_name);
	if (peo_n == -1)
		printf("��Ǹ�����޴���\n\n");
	else
	{
		printf("%-4s\t%-4s\t%-4s\t%-15s\t%s\t\n", "����(��ע)", "�Ա�", "����", "�绰", "��ͥסַ");
		printf("%-4s(%s) \t%-4s\t%-4d\t%-15s\t%s\t\n\n",
			con->Info[peo_n].name, con->Info[peo_n].remark,
			con->Info[peo_n].sex, con->Info[peo_n].age, 
			con->Info[peo_n].tele, con->Info[peo_n].addr);
	}
}
// �޸���ϵ��
void ModifyPeo(Contact* con)
{
	char SearchPeo_name[Name_Max];
	if (con->peo_num == 0)
	{
		printf("ͨѶ¼Ϊ�գ��޷��޸�\n\n");
		return;
	}
	printf("������Ҫ�޸ĵ���ϵ��:>");
	scanf("%s", SearchPeo_name);
	int peo_n = SearchPeoByName(con, SearchPeo_name);
	if (peo_n == -1)
		printf("��Ǹ�����޴���\n\n");
	else
	{
		char flag = 0;
		printf("%-4s\t%-4s\t%-4s\t%-15s\t%s\t\n", "����(��ע)", "�Ա�", "����", "�绰", "��ͥסַ");
		printf("%-4s(%s) \t%-4s\t%-4d\t%-15s\t%s\t\n\n",
			con->Info[peo_n].name, con->Info[peo_n].remark,
			con->Info[peo_n].sex, con->Info[peo_n].age,
			con->Info[peo_n].tele, con->Info[peo_n].addr);
		printf("�Ƿ��޸ģ�(y/n)\n");
		getchar();
		flag = getchar();
		if (flag == 'y')
		{
			printf("��������ϵ������:>");
			scanf("%s", con->Info[peo_n].name);
			printf("��������ϵ�˱�ע:>");
			scanf("%s", con->Info[peo_n].remark);
			printf("��������ϵ������:>");
			scanf("%d", &(con->Info[peo_n].age));
			printf("��������ϵ���Ա�:>");
			scanf("%s", con->Info[peo_n].sex);
			printf("��������ϵ�˵绰:>");
			scanf("%s", con->Info[peo_n].tele);
			printf("��������ϵ��סַ:>");
			scanf("%s", con->Info[peo_n].addr);
		}
		else
			return;
		printf("�޸ĳɹ�\n\n");
	}
}
// ͨѶ¼������������
void SortContact_Name(Contact* con)
{
	qsort(con->Info, (con->peo_num), sizeof(PeoInfo), cmp_name);
	printf("�Ѱ�����������ĸ����\n\n");
}
// ���ͨѶ¼����
void ClearContact(Contact* con)
{
	InitCon(con);
	printf("ͨѶ¼�����\n\n");
}

void DestroyContact(Contact* con)
{
	assert(con);

	free(con->Info);
	con->Contact_capacity = 0;
	con->peo_num = 0;
}
