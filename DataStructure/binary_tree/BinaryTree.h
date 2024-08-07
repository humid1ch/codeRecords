#pragma once 

//int NodeCount = 0;

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>


typedef int BTDataType;

// ��ʽ������
// ��ʽ������ ��Ҫ���һ����Ҫ��˼�룺
// ÿһ�����������Է�Ϊ �������ӡ��Һ���
// �� ����һ�ö�������		 1
//					  2     3
//			        4   5 6   5
// ����Կ���   1 Ϊ���� 2 Ϊ���ӣ� 3 Ϊ�Һ���
// ͬ�����Կ��� 2 Ϊ���� 4 Ϊ���ӣ� 5 Ϊ�Һ���

// ���û������ ���� û���Һ��� ��ô���� = �Һ��� = NULL
// ��         4 Ϊ���� 4������Ϊ NULL�� 4���Һ���Ϊ NULL
typedef struct BinaryTreeNode
{
	struct BinaryTreeNode* left;
	struct BinaryTreeNode* right;
	BTDataType data;
}BTNode;

// �����½ڵ�
BTNode* buyBTNode(BTDataType x);
// �����µĶ������ĺ������̶��ģ�
BTNode* creatBinaryTree();

// �������
void prevOrder(BTNode* pbt);
// �������
void inOrder(BTNode* pbt);
// �������
void postOrder(BTNode* root);
// �������ڵ�ĸ���
int BTNodeSize(BTNode* root);
// ������Ҷ�ӽڵ�����
int LeafSize(BTNode* root);
// ��K�� �ڵ�����
int LevelSize(BTNode* root, int k);
// ���������
int BinaryTreeDepth(BTNode* root);
// �������
void BinaryTreeLevelOrder(BTNode* root);
// �ж϶������Ƿ�����ȫ������
int BinaryTreeComplete(BTNode* root);
