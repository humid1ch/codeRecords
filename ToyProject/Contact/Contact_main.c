#define _CRT_SECURE_NO_WARNINGS 1

#include "Contact.h"

int main()
{
	int input = 0;
	Contact Con;
	InitCon(&Con);		//��ʼ��ͨѶ¼
	do 
	{
		menu();
		printf("��ѡ��:>");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			AddPeople(&Con);	//�����ϵ��
			break;
		case 2:
			DelePeopel(&Con);	//ɾ����ϵ��
			break;
		case 3:
			SearchPeo(&Con);	//������ϵ��
			break;
		case 4:
			ModifyPeo(&Con);	//�޸���ϵ��
			break;
		case 5:
			SortContact_Name(&Con);	//ͨѶ¼����
			break;
		case 6:
			ShowContact(&Con);	//��ʾͨѶ¼
			break;
		case 7:
			ClearContact(&Con);	//���ͨѶ¼
			break;
		case 0:
			DestroyContact(&Con);
			exits();			//�˳�ͨѶ¼
			break;
		default:
			printf("ѡ�����,������ѡ��\n");
			break;
		}

	} while (input);

	return 0;
}