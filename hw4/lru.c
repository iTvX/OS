#include "lru.h"
#include <stdio.h>


//get elment index that its frequency is max
int max_frequency(int* frequency, int size)
{
    int index = 0;
    int max = frequency[0];
    for(int i = 0; i < size; i++) {
        if (frequency[i] >= max) {
            max = frequency[i];
            index = i;
        }
    }
    return index;
}
// add 1 for every element not -1
void  process_frequency(int* frequency, int size) {
    int i;
    for(i=0; i< size; i++){
        if(frequency[i] != -1)
            frequency[i]++;
    }
}

// lru page replacement algorithm
int lru(int* arr, int size) {
    int pages[max_length], frequency[max_length];
    int count = 0, index, found;
    int i, j;
    int a, b;
    // initial page array and frequency array
    for(i=0; i<max_length; i++) {
        pages[i]=-1;
        frequency[i]=-1;
    }
    for(i=0;i<size;i++)
    {
        a=0;
        b=0;
        for(j=0;j<max_length;j++)
        {
            // found element in memory
            if(pages[j]==arr[i])
            {
                a=1;
                b=1;
                process_frequency(frequency, max_length);
                frequency[j]--;
            }
        }
        if(a == 0)
        {
            for(j = 0;j < max_length; j++)
            {
                // still have element not replaced
                if(pages[j]==-1)
                {
                    pages[j]=arr[i];
                    b=1;
                    count++; // increment page faults;
                    process_frequency(frequency, max_length);
                    frequency[j] = 0; // set just accessed element frequency 0
                    break;
                }
            }
        } // when all pages have element ever
        if(b == 0)
        {
            int max= max_frequency(frequency, max_length);
            pages[max]=arr[i];
            process_frequency(frequency, max_length);
            frequency[max] = 0; // set just accessed element frequency 0
            count++;
        }
    }


    return count;
}
