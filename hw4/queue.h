#ifndef PAGEREPLACEMENT_QUEUE_H
#define PAGEREPLACEMENT_QUEUE_H

#include <stdio.h>
#define MAX_LENGTH 2

int queue[MAX_LENGTH];
int head, tail;
int queue_length;

void queue_init();
int queue_is_full();
int en_queue(int n);
int de_queue();
void print_queue();
int is_in_queue(int n);

#endif //PAGEREPLACEMENT_QUEUE_H
