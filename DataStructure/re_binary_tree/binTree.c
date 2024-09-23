#include "binTree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

treenode* buyNode(int num) {
	treenode* newNode = (treenode*)malloc(sizeof(treenode));
	if (!newNode) {
		printf("malloc faild.\n");
		printf("buy node faild.\n");

		return NULL;
	}

	newNode->_data = num;
	newNode->_lchild = NULL;
	newNode->_rchild = NULL;

	return newNode;
}

void freeNode(treenode* cur) {
	assert(cur);

	cur->_data = 0;
	cur->_lchild = NULL;
	cur->_rchild = NULL;

	free(cur);
	cur = NULL;
}

treenode* treeInsert(treenode* root, int num) {
	treenode* newNode = buyNode(num);
	if (!root) {
		root = newNode;
		return root;
	}

	treenode* cur = root;
	treenode* parent = root;
	while (cur) {
		parent = cur;

		if (num < cur->_data)
			cur = cur->_lchild;
		else
			cur = cur->_rchild;
	}

	if (num < parent->_data)
		parent->_lchild = newNode;
	else
		parent->_rchild = newNode;

	return root;
}

void _preOrder(const treenode* root) {
	if (!root) {
		return;
	}

	printf("%d ", root->_data);
	_preOrder(root->_lchild);
	_preOrder(root->_rchild);
}

void preOrder(const treenode* root) {
	printf("前序遍历: ");
	_preOrder(root);
	printf("\n");
}

void _midOrder(const treenode* root) {
	if (!root) {
		return;
	}

	_midOrder(root->_lchild);
	printf("%d ", root->_data);
	_midOrder(root->_rchild);
}

void midOrder(const treenode* root) {
	printf("中序遍历: ");
	_midOrder(root);
	printf("\n");
}

void _postOrder(const treenode* root) {
	if (!root) {
		return;
	}

	_postOrder(root->_lchild);
	printf("%d ", root->_data);
	_postOrder(root->_rchild);
}

void postOrder(const treenode* root) {
	printf("后序遍历: ");
	_postOrder(root);
	printf("\n");
}

void _leafCnt(const treenode* root, int* cnt) {
	if (!root) {
		return;
	}

	_leafCnt(root->_lchild, cnt);
	_leafCnt(root->_rchild, cnt);

	if (!root->_lchild && !root->_rchild) {
		printf("%d ", root->_data);
		(*cnt)++;
	}
}

int leafCnt(const treenode* root) {
	int count = 0;
	_leafCnt(root, &count);

	return count;
}

void _freeTree(treenode* root) {
	if (!root) {
		return;
	}

	_freeTree(root->_lchild);
	_freeTree(root->_rchild);
	freeNode(root);
}

void freeTree(treenode* root) {
	_freeTree(root);
}

treenode* treeFind(treenode* root, treenode** parent, int num) {
	treenode* cur = root;
	while (cur) {
		if (num < cur->_data) {
			*parent = cur;
			cur = cur->_lchild;
		}
		else if (num > cur->_data) {
			*parent = cur;
			cur = cur->_rchild;
		}
		else
			return cur;
	}

	return cur;
}

treenode* treeMinNode(treenode* root, treenode** parent) {
	if (!root) {
		printf("minNode: tree empty.\n");
		return NULL;
	}

	treenode* cur = root;
	// 不停向左子树找, 直到没有左子树, 就是最小节点
	while (cur->_lchild) {
		*parent = cur;
		cur = cur->_lchild;
	}

	return cur;
}

// 因为存在 删除传入树的根节点的情况, 所以可能会生成新的根,
// 所以返回一个结点指针, 表示新树
treenode* treeErase(treenode* root, int num) {
	if (!root) {
		printf("tree empty.\n");
		return root;
	}

	treenode* parent = NULL;
	treenode* cur = treeFind(root, &parent, num);
	if (!cur) {
		printf("cur is not existed.\n");
		return root;
	}

	// 如果删除整棵树的根, 单独处理
	if (cur == root) {
		if (!root->_rchild)
			return root->_lchild;
		if (!root->_lchild)
			return root->_rchild;

		treenode* minParent = cur;
		treenode* minNode = treeMinNode(root->_rchild, &minParent);

 		if (minParent != cur) {
 			minParent->_lchild = minNode->_rchild;
 			minNode->_rchild = minParent;
 		}
 		minNode->_lchild = cur->_lchild;

		freeNode(cur);

		return minNode;
	}

	// 其他情况, 正常处理

	// 记录 目标节点是其父节点的左孩子还是右孩子, 为子树连接做准备
	int isLchild = 0;
	if (parent->_lchild == cur)
		isLchild = 1;

	if (!cur->_lchild && !cur->_rchild) {
		// 目标节点是叶子节点, 父节点对应的子节点直接置空
		if (isLchild)
			parent->_lchild = NULL;
		else
			parent->_rchild = NULL;
	}
	else if (!cur->_lchild) {
		// 目标节点没有左子树, 父节点对应的子树 更新为目标节点的右子树
		if (isLchild)
			parent->_lchild = cur->_rchild;
		else
			parent->_rchild = cur->_rchild;
	}
	else if (!cur->_rchild) {
		// 目标节点没有右子树, 父节点对应的子树 更新为目标节点的左子树
		if (isLchild)
			parent->_lchild = cur->_lchild;
		else
			parent->_rchild = cur->_lchild;
	}
	else {
		// 目标节点拥有两个子树
		// 需要找目标节点右子树的最小节点, 作为其父节点的新子节点, 这样可以保证
		// 树依然保持 左小 右大的规则
 		treenode* minParent = cur;
 		treenode* minNode = treeMinNode(cur->_rchild, &minParent);
 		// 找到的minNode一定没有左孩子
 		// minNode一定是minParent的左孩子
 
 		if (minParent != cur) {
 			/* cur右子树中的最小节点, 不是cur的右孩子节点
 			 * 即 至少有 cur
 			 *            \
              *             n
 			 *            /
 			 *           min
 			 *             \ 
              *              n
              */
 
 			// 更新minParent的左孩子
 			minParent->_lchild = minNode->_rchild;
 			// 更新minNode 位置
 			minNode->_rchild = cur->_rchild;
 			minNode->_lchild = cur->_lchild;
 		}
 		else {
 			/* cur右子树中的最小节点, 就是cur的右孩子节点
 			 * 即 存在     parent
 			 *              /\
              *             cur
 			 *               \
              *               min
 			 *                \
              *                  n
              */
 			minNode->_lchild = minParent->_lchild;
 		}
 
 		if (isLchild)
 			parent->_lchild = minNode;
 		else
 			parent->_rchild = minNode;
	}

	freeNode(cur);
	cur = NULL;

	return root;
}
