#include "queue.h"

// initial quue
void queue_init() {
    queue_length = 0;
    head = tail = 0;
    for (int i = 0; i < MAX_LENGTH; i++)
        queue[i] = -1;
}

// check queue is full
int queue_is_full(void) {
    if (queue_length == MAX_LENGTH)
        return 1;
    else
        return 0;
}

// add n to queue
int en_queue(int n) {
    if (queue_is_full()) {
        return -1;
    }

    queue[tail] = n;
    tail = (tail+1) % MAX_LENGTH;

    queue_length++;

    return n;
}
// remove n from queue
int de_queue() {
    queue_length--;
    int i;
    i = queue[head];
    head = (head+1) % MAX_LENGTH;
    return i;
}

// check elment n is in queue
int is_in_queue(int n) {
    if (queue_length == 0)
        return 0;
    int i;
    for (i = 0; i < MAX_LENGTH; i++)
        if (queue[i] == n)
            return 1;

    return 0;
}
