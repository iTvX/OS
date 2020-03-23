#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define num_threads 9


int valid[num_threads] = {0};


typedef struct {
    int row;
    int column;
} parameters;

int sudoku[9][9] = {
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



void *validator(void* param) {

    parameters *params = (parameters*) param;
    int sum = 0;
    int product = 1;
    int row = params->row;
    int col = params->column;

    int i, j;
    for (i = row; i < row + 3; i++) {
        for (j = col; j < col + 3; j++) {
            int num = sudoku[i][j];
            sum += num;
            product *= num;
        }
    }
    if (sum == 45 && product == 362880) {
        valid[row + col/3] = 1;
        pthread_exit(NULL);
    }
    return NULL;
}




int main() {
    pthread_t threads[num_threads];

    int threadIndex = 0;
    int i,j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (i%3 == 0 && j%3 == 0) {
                parameters *data = (parameters *) malloc(sizeof(parameters));
                data->row = i;
                data->column = j;
                pthread_create(&threads[threadIndex++], NULL, validator, data); // 3x3 subsection threads
            }
        }
    }

    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }


    for (i = 0; i < num_threads; i++) {
        if (valid[i] == 0) {
            printf("Invalid!\n");
            return EXIT_SUCCESS;
        }
    }
    printf("Valid!\n");
    return EXIT_SUCCESS;
}