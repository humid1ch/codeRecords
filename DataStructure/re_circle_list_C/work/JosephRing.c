#include "JosephRing.h"
#include "list.h"

#include <assert.h>
#include <stdio.h>

list* josephRing(list* list) {
	assert(list);

	// 链表中有效节点 只有 0  1  2 个时, 直接返回
	if (list->_next == list || list->_next->_next == list ||
		list->_next->_next->_next == list) {
		return list;
	}

	int cnt = 1;
	node* cur = list;
	while (cur->_next->_next->_next != cur) {
		if (cur == list) {
			cur = cur->_next;
			continue;
		}

		printf("%d:%d  ", cur->_data, cnt);

		if (cnt == 3) {
			cnt = 1;
			printf("杀%d  ", cur->_data);
			node* next = cur->_next;
			listEraseNode(list, cur); // 删除该删节点
			cur = next;
			printList(list);
			printf("\n");
			printf("%d:%d  ", cur->_data, cnt);
		}
		cur = cur->_next;
		cnt++;
	}
	printf("\n");

	return list;
}
