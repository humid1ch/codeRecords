#define _CRT_SECURE_NO_WARNINGS

#include "mine_function.h"

void menu()
{
	printf("\n****************************\n");
	printf("******     1. play     ******\n");
	printf("******     0. exit     ******\n");
	printf("*****************************\n");
}

void minegame_main()
{
	char mine[ROWS][COLS] = { 0 };				//�����׵�����
	char show[ROWS][COLS] = { 0 };				//չʾ�׵�����
	//��ʼ������													''�ڵ����� Ϊ��ʼ������
	InitiBoard(mine, ROWS, COLS,'0');
	InitiBoard(show, ROWS, COLS,'*');
	//��ӡ����
	DisplayBoard(show, ROW, COL);
	//������
	SetMine(mine, ROW, COL);
			//DisplayBoard(mine, ROW, COL);				//����� ��������֤��Ϸ�߼��Ƿ���ȷ
	//�ų���
	FindMine(mine,show,ROW,COL);
}

int main()
{
	int input = 0;
	srand((unsigned int)time(NULL));
	do {
		menu();
		printf("\n��ѡ��:>");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			minegame_main();
			break;
		case 0:
			printf("\n��Ϸ�˳��ɹ���\n");
			break;
		default:
			printf("\nѡ�����������ѡ��\n\n");
			break;
		}

	} while (input);

	return 0;
}

