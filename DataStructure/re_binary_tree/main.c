#include "binTree.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(0));

	int nums[20] = {0};
	for (int i = 0; i < 10; i++) {
		nums[i] = rand() % 50;
	}

	treenode* root = buyNode(nums[0]);
	for (int i = 1; i < 10; i++) {
		treeInsert(root, nums[i]);
	}

	midOrder(root);
	preOrder(root);
	postOrder(root);

	//     int leafCount = leafCnt(root);
	//     printf("叶子节点有 %d 个\n", leafCount);

	for (int i = 0; i < 4; i++) {
		int num = 0;
		printf("输入:> ");
		scanf("%d", &num);
		root = treeErase(root, num);
		midOrder(root);
		preOrder(root);
	}

	return 0;
}
