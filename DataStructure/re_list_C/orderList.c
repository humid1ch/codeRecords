#include "list.h"

#include <assert.h>
#include <stdio.h>

void orderInsert(list* list, int num) {
    assert(list);

    listnode* cur = listFind(list, num);
    if (cur) {
        // 相同值的节点存在不再插入
        return;
    }

    // listFindPos() 找第一个比num大的节点
    // 找不到返回空, 找到返回节点
    listnode* pos = listFindPos(list, num);
    if (!pos) {
        listPushBack(list, num);
    }
    else {
        listInsertBefore(list, pos->_data, num);
    }
}
