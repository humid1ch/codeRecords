#include "stack.h"

int main() {
	Stack st;
    StackList stl;

	stackInit(&st);
    stackListInit(&stl);

	for (int i = 0; i < 8; i++) {
		stackPush(&st, i);
        stackListPush(&stl, i);
		showStack(&st);
		showStackList(&stl);
	}

	for (int i = 0; i < 10; i++) {
		stackPop(&st);
        stackListPop(&stl);
		showStack(&st);
		showStackList(&stl);
	}

	return 0;
}
