#include "stack.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Stack* stackInit(Stack* st) {
    assert(st);

    int* tmp = (int*)malloc(sizeof(int) * 15);
    if (!tmp) {
        printf("stack init faild.\n");
        return st;
    }

    st->_data = tmp;
    st->_top = 0;
    st->_capacity = 15;

    return st;
}

int stackEmpty(const Stack* st) {
    assert(st);

    return st->_top == 0;
}

int stackTop(const Stack* st) {
    assert(st);

    return st->_data[st->_top];
}

void stackPop(Stack* st) {
    assert(st);

    if (stackEmpty(st)) {
        printf("stack have no elem.\n");
        return;
    }

    st->_top--;
}

void stackPush(Stack* st, int num) {
    assert(st);

    if (st->_top == st->_capacity) {
        int newCapacity = st->_capacity * 1.5;
        int* tmp = (int*)realloc(st->_data, sizeof(int) * newCapacity);
        if (!tmp) {
            printf("realloc faild.\n");
            printf("push faild: capacity not enough.\n");
            return;
        }

        st->_data = tmp;
        st->_capacity = newCapacity;
    }

    st->_data[st->_top++] = num;
}

void showStack(const Stack* st) {
    assert(st);
    if (stackEmpty(st)) {
        return;
    }

    for (int i = 0; i < st->_top; i++) {
        printf("%d ", st->_data[i]);
    }
    printf("\n");
}

DataNode* buyDataNode(int num) {
    DataNode* newNode = (DataNode*)malloc(sizeof(DataNode));
    if (!newNode) {
        printf("malloc faild.\n");
        printf("buy node faild.\n");
        return NULL;
    }

    newNode->_next = NULL;
    newNode->_data = num;

    return newNode;
}

StackList* stackListInit(StackList* st) {
    assert(st);

    st->_top = NULL;
    st->_count = 0;

    return st;
}

int stackListEmpty(const StackList* st) {
    assert(st);

    return st->_count == 0;
}

int stackListTop(const StackList* st) {
    assert(st);

    if (stackListEmpty(st)) {
        printf("stack is empty.\n");
        return -1;
    }

    return st->_top->_data;
}

void stackListPop(StackList* st) {
    assert(st);

    if (stackListEmpty(st)) {
        printf("stack is empty.\n");
        return;
    }

    DataNode* next = st->_top->_next;
    free(st->_top);
    st->_top = next;
    st->_count--;
}

void stackListPush(StackList* st, int num) {
    assert(st);

    DataNode* newNode = buyDataNode(num);

    // 头插连接新节点
    newNode->_next = st->_top;  // 链表为不为空无所谓, 因为没有解引用头插前的st->top
    // 更新top
    st->_top = newNode;
    st->_count++;
}

void showStackList(const StackList* st) {
    assert(st);
    
    DataNode* cur = st->_top;
    while (cur) {
        printf("%d-->", cur->_data);
        cur = cur->_next;
    }
    printf("NULL\n");
}

// 下面是链表尾作为top, push是尾插
// StackList* stackListInit(StackList* st) {
//     assert(st);
// 
//     st->_list = NULL;
//     st->_top = NULL;
// 
//     return st;
// }
// 
// int stackListEmpty(const StackList* st) {
//     assert(st);
// 
//     return st->_list == NULL;
// }
// 
// int stackListTop(const StackList* st) {
//     assert(st);
// 
//     if (stackListEmpty(st)) {
//         printf("stack is empty.\n");
//         return -1;
//     }
// 
//     return stackListEmpty(st) ? 0 : st->_top->_data;
// }
// 
// void stackListPop(StackList* st) {
//     assert(st);
// 
//     if (stackListEmpty(st)) {
//         printf("stack is empty.\n");
//         return;
//     }
//     
//     DataNode* cur = st->_list;
//     DataNode* prev = cur;
//     while (cur != st->_top) {
//         prev = cur;
//         cur = cur->_next;
//     }
// 
//     prev->_next = NULL;
//     free(cur);
//     st->_top = prev;
// }
// 
// void stackListPush(StackList* st, int num) {
//     assert(st);
// 
//     DataNode* newNode = buyDataNode(num);
// 
//     // 链表为空
//     if (!st->_list) {
//         st->_list = newNode;
//         st->_top = newNode;
//         return;
//     }
// 
//     // 连接新节点
//     st->_top->_next = newNode;
//     // 更新top
//     st->_top = st->_top->_next;
// }
// 
// void showStackList(const StackList* st) {
//     assert(st);
//     
//     DataNode* cur = st->_list;
//     while (cur) {
//         printf("%d-->", cur->_data);
//         cur = cur->_next;
//     }
// }
