#include "queue.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void queueInit(queue* queue) {
    assert(queue);

    int* tmp = (int*)malloc(sizeof(int) * 15);
    if (!tmp) {
        printf("malloc faild.\n");
        printf("queue init faild.\n");
        return;
    }
    
    queue->_size = 0;
    queue->_front = -1;
    queue->_capacity = 15;
}
