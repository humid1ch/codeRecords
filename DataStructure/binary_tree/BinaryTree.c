#define _CRT_SECURE_NO_WARNINGS 1

#include "BinaryTree.h"

BTNode* buyBTNode(BTDataType x)
{
	BTNode* newNode = (BTNode*)malloc(sizeof(BTNode));
	if (newNode == NULL)
	{
		printf("malloc failed\n");
		exit(-1);
	}

	newNode->left = newNode->right = NULL;
	newNode->data = x;

	return newNode;
}

BTNode* creatBinaryTree()
{
	BTNode* node1 = buyBTNode(1);
	BTNode* node2 = buyBTNode(2);
	BTNode* node3 = buyBTNode(3);
	BTNode* node4 = buyBTNode(4);
	BTNode* node5 = buyBTNode(5);
	BTNode* node6 = buyBTNode(6);

	node1->left = node2;
	node1->right = node4;
	node2->left = node3;
	node4->left = node5;
	node4->right = node6;

	return node1;
}

void prevOrder(BTNode* root)
{// �ݹ� �������
	if (root == NULL)	// ����δ�գ���������������
	{
		printf("NULL ");
		return;
	}
	
	// �����������˳�򣺸������ӡ��Һ���
	printf("%d ", root->data);
	prevOrder(root->left);
	prevOrder(root->right);
}

void inOrder(BTNode* root)
{// �ݹ� �������
	if (root == NULL)
	{
		printf("NULL ");
		return;
	}

	// �����������˳�����ӡ������Һ���
	// ������������ӣ���һֱȥ�����ӽڵ�
	// ֱ������Ϊ�գ����ʿգ�Ȼ����� ��
	inOrder(root->left);
	printf("%d ", root->data);
	inOrder(root->right);
}

void postOrder(BTNode* root)
{
	if (root == NULL)
	{
		printf("NULL ");
		return;
	}

	postOrder(root->left);
	postOrder(root->right);
	printf("%d ", root->data);
}


// �������ڵ����� ��ȫ�ֱ��������ķ�����
//NodeCount = 0;
//void BTNodeSize(BTNode* root)
//{
//	if (root == NULL)
//		return 0;
//
//	NodeCount++;
//	BTNodeSize(root->left);
//	BTNodeSize(root->right);
//}


// �������ڵ�����������˼�룩
int BTNodeSize(BTNode* root)
{
	// ��ǰ�ڵ�Ϊ�վͷ��� 0����ʾ���� 1
	// ��ǰ�ڵ㲻Ϊ�գ��� + 1�����ҽ�����һ������
	return root == NULL ? 0 : BTNodeSize(root->left) + BTNodeSize(root->right) + 1;
}

// Ҷ�ӽڵ�����������˼�룩
int LeafSize(BTNode* root)
{
	if (root == NULL)
		return 0;

	// ���Һ��Ӷ�Ϊ�� ����1
	// ��Ϊ�ݹ���ݽ�
	// ���� ��󷵻ص��� �Ӹ����������µ�����Ҷ�ӽڵ������ + �Ӹ����Һ�����������Ҷ�ӽ�������
	return (root->left == NULL && root->right == NULL) ? 1 : LeafSize(root->left) + LeafSize(root->right);
}

int LevelSize(BTNode* root, int k)
{
	if (root == NULL || (root->left == NULL && root->right == NULL))
		return 0;

	return k == 1 ? 1 : LevelSize(root->left, k - 1) + LevelSize(root->right, k - 1);
}

int BinaryTreeDepth(BTNode* root)
{
	if (root == NULL)
		return 0;

	//return (root->left == NULL && root->right == NULL) ? 1 : (BinaryTreeDepth(root->left) > BinaryTreeDepth(root->right) ? (BinaryTreeDepth(root->left) + 1) : (BinaryTreeDepth(root->right) + 1));
	int leftDepth = BinaryTreeDepth(root->left);
	int rightDepth = BinaryTreeDepth(root->right);

	return leftDepth > rightDepth ? leftDepth + 1 : rightDepth + 1;
}

// �������
void levelOrder(BTNode* root)
{

}

// �ж϶������Ƿ�����ȫ������
bool BTComplete(BTNode* root)
{

}