/* Since I found C doesn't have "this" argument, and I have tried a lot of way to pass multiple argument
 * to threaded function from pthread_create but all of them didn't work. So I Google it how to do that
 * and found a way to solve that: use struct and set multiple parameter in that struct.
 *
 * Reference:
 * Passing multiple arguments to threaded function from pthread_create
 * https://stackoverflow.com/questions/16230542/passing-multiple-arguments-to-threaded-function-from-pthread-create
 *
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define num_colOrRow 9
#define numOfThreads 9
#define sub_interval 3
// to check each of thread is valid.
int isValid[numOfThreads];
// import the source.
int source[num_colOrRow][num_colOrRow] = {
        {6, 2, 4, 5, 3, 9, 1, 8, 7},
        {5, 1, 9, 7, 2, 8, 6, 3, 4},
        {8, 3, 7, 6, 1, 4, 2, 9, 5},
        {1, 4, 3, 8, 6, 5, 7, 2, 9},
        {9, 5, 8, 2, 4, 7, 3, 6, 1},
        {7, 6, 2, 3, 9, 1, 4, 5, 8},
        {3, 7, 1, 9, 5, 6, 8, 4, 2},
        {4, 9, 6, 1, 8, 2, 5, 7, 3},
        {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

// Set the struct so that pthread_create can call multiple argument
struct myParam {
    int row;
    int column;
};

void *validator(void *arg) {
    //take arg to be pointer to myParm.
    struct myParam *recv_para;
    recv_para = (struct myParam *) arg;
    int row = recv_para->row;
    int col = recv_para->column;
    //Set the sum of product and Add.
    int sumOfAdd = 0;
    int sumOfProduct = 1;


    int i = row;
    while (i < row + sub_interval) {
        int j = col;
        while (j < col + sub_interval) {
            int num = source[i][j];
            sumOfAdd += num;
            sumOfProduct *= num;
            j++;
        }
        i++;
    }

    //45 and 362880 is given by the instruction (A2.pdf)
    if (sumOfAdd == 45 && sumOfProduct == 362880) {
        isValid[row + col / 3] = 1;
        pthread_exit(NULL);
    }
    return NULL;
}


int main() {
    pthread_t threads[numOfThreads];
    int index = 0;
    int col = 0;
    while (col < num_colOrRow) {
        int row = 0;
        while (row < num_colOrRow) {
            if (col % sub_interval == 0 && row % sub_interval == 0) {
                struct myParam *recv_para;
                recv_para = (struct myParam *) malloc(sizeof(struct myParam));
                recv_para->row = row;
                recv_para->column = col;
                pthread_create(&threads[index++], NULL, validator, recv_para);
            }
            row++;
        }
        col++;
    }

    col = 0;
    while (col < numOfThreads) {
        pthread_join(threads[col], NULL);
        col++;
    }


    col = 0;
    while (col < numOfThreads) {
        if (isValid[col] == 0) {
            printf("Invalid!\n");
            return -1;
        }
        col++;
    }
    printf("valid!\n");
}
