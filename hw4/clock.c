#include "clock.h"
#include <stdio.h>

// page replacement clock algorithm
// return the page faults counts
int clock(int arr[], int size) {
    // memory page array
    int pages[max_length], used[max_length];
    int count, index, found;
    int i, j;
    // initial assignment
    for(i=0; i<max_length; i++) {
        pages[i]=-1;
        used[i]=0;
    }
    count=0;
    index=0;
    for(i=0; i<size; i++) {
        found=0;
        // check number is in memory
        for(j=0; j<max_length; j++) {
            if(arr[i]==pages[j]) {
                found=1;
                used[j]=1;
            }
        }
        // number not in memory
        if(found == 0) {
            while (found != 1) {
                if(used[index]==0) {
                    pages[index] = arr[i]; // put arr[i] to memory
                    used[index] = 1;  // set used flag
                    found=1;
                    count++;  // page fault count
                }
                else {
                    used[index]=0; // resume to 0
                }
                index++; // to next elment
                if(index==max_length)
                {
                    index=0;   // reset to the start
                }
            }
        }
    }
    return count;
}