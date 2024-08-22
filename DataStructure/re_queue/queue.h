#pragma once

typedef struct quque {
    int* _data;
    int _front;
    int _size;
    int _capacity;
} queue;

void queueInit(queue* queue);
