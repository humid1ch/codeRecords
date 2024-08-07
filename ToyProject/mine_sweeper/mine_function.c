#define _CRT_SECURE_NO_WARNINGS

#include "mine_function.h"

void InitiBoard(char mine[ROWS][COLS], int row, int col, char set)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			mine[i][j] = set;					//�������Ԫ�� ��ʼ��Ϊ set �е��ַ�
		}
	}
}

void DisplayBoard(char mine[ROWS][COLS], int row, int col)			//��ӡ����
{
	int i = 0;
	int j = 0;
	printf("\n-------------ɨ����Ϸ-------------\n\n");
	for (i = 0; i <= row; i++)
	{
		printf(" %d ", i);
	}
	printf("\n");
	for (i = 1; i <= row; i++)
	{
		printf(" %d ", i);
		for (j = 1; j <= col; j++)
		{
			printf(" %c ", mine[i][j]);
		}
		printf("\n");
	}
}

void SetMine(char mine[ROWS][COLS], int row, int col)			//��� �������� ������
{
	int count = MINE_NUM;
	while (count)
	{
		int x = rand() % row + 1;
		int y = rand() % col + 1;
		if (mine[x][y] == '0')
		{
			mine[x][y] = '1';
			count--;
		}
	}
}

//static ���κ��� ���ı亯�����÷�Χ  ʹ���������ڱ� .c�ļ�ʹ��  �Ҳ�Ӱ�������ļ���ͬ�������ĺ���
static int MineNum(char mine[ROWS][COLS], int x, int y)
{
	return	mine[x - 1][y - 1] +
		mine[x - 1][y] +
		mine[x - 1][y + 1] +
		mine[x][y - 1] +
		mine[x][y + 1] +
		mine[x + 1][y - 1] +
		mine[x + 1][y] +
		mine[x + 1][y + 1] - 8 * '0';
	//�ж� mine[x][y] ��Χ�Ƿ�����   ������ �ַ�1 Ϊ�� �ַ�0 Ϊ��	�� - 8���ַ�0 ��Ϊ�׸���
}

void FindMine(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col)
{
	int x = 0;
	int y = 0;
	int count = 0;
	while (1)
	{
		printf("\n����������:>");
		scanf("%d%d", &x, &y);		//x��y �� �Ϸ���Χ �� 1 - col/row
		//�ж�����ĺϷ���
		if (x >= 1 && x <= row && y >= 1 && y <= col)
		{
			if (mine[x][y] == '1')
			{
				printf("\n�㱻ը���ˣ�͸͸�ģ�\n\n");
				DisplayBoard(show, ROW, COL);
				break;
			}
			else
			{
				int count_mine = MineNum(mine, x, y);
				show[x][y] = count_mine + '0';
				DisplayBoard(show, ROW, COL);
				count++;								//���״����ܺ�
				if (count == row * col - MINE_NUM)			//��һ��ֻ���ų�һ�� �����״��� ���ʱ ����
				{
					printf("\n��ϲ���أ�\n\n");
					break;
				}
			}
		}
		else
		{
			printf("\n����Ƿ�������������\n\n");
		}
	}
}
