#pragma once

typedef struct Stack {
    int* _data;
    int _top;
    int _capacity;
} Stack;

Stack* stackInit(Stack* st);

// 空返回1
int stackEmpty(const Stack* st);

int stackTop(const Stack* st);

void stackPop(Stack* st);

void stackPush(Stack* st, int num);

void showStack(const Stack* st);

typedef struct DataNode {
    int _data;
    struct DataNode* _next;
} DataNode, DataList;

// 尾插需要两个节点
// 头插只需要记录栈顶节点, 表示链表首节点就可以
// typedef struct StackList {
//     DataList* _list;
//     DataNode* _top;
// } StackList;
typedef struct StackList {
    DataNode* _top;
    int _count;         // 节点个数
} StackList;

DataNode* buyDataNode(int num);

StackList* stackListInit(StackList* st);

// 空返回1
int stackListEmpty(const StackList* st);

int stackListTop(const StackList* st);

void stackListPop(StackList* st);

void stackListPush(StackList* st, int num);

void showStackList(const StackList* st);
