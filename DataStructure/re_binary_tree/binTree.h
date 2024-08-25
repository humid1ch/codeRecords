#pragma once

typedef struct treenode {
	int _data;
	struct treenode* _lchild; // 左孩子节点
	struct treenode* _rchild; // 右孩子节点
} treenode;

treenode* buyNode(int num);

treenode* treeInsert(treenode* root, int num);

void freeNode(treenode* cur);

treenode* treeInsert(treenode* root, int num);

void preOrder(const treenode* root);

void midOrder(const treenode* root);

void postOrder(const treenode* root);

treenode* treeFind(treenode* root, treenode** parent, int num);

int leafCnt(const treenode* root);

void freeTree(treenode* root);

treenode* treeErase(treenode* root, int num);
