#pragma once

typedef struct Node {
	int _data;
    struct Node* _next;
} Node;

Node* createHead();

Node* buyNode(int num);

void pushBack(Node* list, Node* node);

void pushFront(Node* list, Node* node);

void printList(Node* list);

Node* findNode(Node* list, int num);
