#include <stdio.h>
#include "queue.h"
#include "clock.h"
#include "lru.h"

int main()
{
    int arr1[] = {2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2};
    int arr2[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    // get arr1 and arr2 length
    int n1 = sizeof(arr1)/ sizeof(int);
    int n2 = sizeof(arr2)/ sizeof(int);
    printf("n1 = %d, n2 = %d\n", n1, n2);
    //printf("FIFO page replacement algorithm:\n");
    int i;
    int count = 0;
    // initialize fifo
    queue_init();
    for (i = 0; i < n1; i++) {
        // check elment is in queue
        if (is_in_queue(arr1[i])) {
            //printf("%d in queue\n", arr1[i]);
            continue;
        }
        else { // elment is not in queue
            count++;
            if (queue_is_full()) {
                int n = de_queue();
                //printf("%d dequeue\n", n);
            }
            en_queue(arr1[i]);
            //printf("%d enqueque\n", arr1[i]);
        }
    }
    //printf("arr1 fifo page replacement need %d page fault\n", count);
    //printf("arr1 clock page replacement need %d page fault\n", clock(arr1, n1));
    //printf("arr1 lru page replacement need %d page fault\n", lru(arr1, n1));

    // get arr2 fifo page replacement fault count
    count = 0;
    queue_init();
    for (i = 0; i < n2; i++) {
        //printf("arr[%d] = %d\n", i, arr2[i]);
        if (is_in_queue(arr2[i])) {
            //printf("%d in queue\n", arr2[i]);
            continue;
        }
        else {
            count++;
            if (queue_is_full()) {
                int n = de_queue();
                //printf("%d dequeue\n", n);
            }
            en_queue(arr2[i]);
            //printf("%d enqueque\n", arr2[i]);
        }
    }
    //printf("arr2 fifo page replacement need %d page fault\n", count);
    //printf("arr2 clock page replacement need %d page fault\n", clock(arr2, n2));
    printf("arr2 lru page replacement need %d page fault\n", lru(arr2, n2));




}