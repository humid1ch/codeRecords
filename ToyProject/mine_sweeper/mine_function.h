#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define ROW 9	
#define COL 9					//��Ϸ�к�������������

#define ROWS ROW+2				
#define COLS COL+2				//��Ϸ��Ϊ�����ı����ڷ������Χ ����  ���������չһ�С���

#define MINE_NUM 10				//��Ϸ��  �׵ĸ���

//��ʼ������ ����
void InitiBoard(char mine[ROWS][COLS], int row, int col, char set);			
//��ӡ���� ����
void DisplayBoard(char mine[ROWS][COLS], int row, int col);
//������ ����
void SetMine(char mine[ROWS][COLS], int row, int col);
//�ų��� ����
void FindMine(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col);