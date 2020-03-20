#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define num_threads 9

/* 
	Initialize the array which worker threads can update to 1 if the 
	corresponding region of the sudoku puzzle they were responsible 
 	for is valid.
*/
int valid[num_threads] = {0};

// Struct that stores the data to be passed to threads
typedef struct {
    int row;
    int column;
} parameters;

// Sudoku puzzle to be solved
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


// Method that determines if numbers 1-9 only appear once in a 3x3 subsection
void *is3x3Valid(void* param) {
    // Confirm that parameters indicate a valid 3x3 subsection
    parameters *params = (parameters*) param;
    int sum = 0;
    int product = 1;
    int row = params->row;
    int col = params->column;
    if (row > 6 || row % 3 != 0 || col > 6 || col % 3 != 0) {
        fprintf(stderr, "Invalid row or column for subsection! row=%d, col=%d\n", row, col);
        pthread_exit(NULL);
    }

    int i, j;
    for (i = row; i < row + 3; i++) {
        for (j = col; j < col + 3; j++) {
            int num = sudoku[i][j];
            sum += num;
            product *= num;
        }
    }
    if (sum == 45 && product == 362880) {
        valid[row + col/3] = 1; // Maps the subsection to an index in the first 8 indices of the valid array
        pthread_exit(NULL);
    }
    return NULL;
}




int main() {
    pthread_t threads[num_threads];

    int threadIndex = 0;
    int i,j;
    // Create 9 threads for 9 3x3 subsections, 9 threads for 9 columns and 9 threads for 9 rows.
    // This will end up with a total of 27 threads.
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (i%3 == 0 && j%3 == 0) {
                parameters *data = (parameters *) malloc(sizeof(parameters));
                data->row = i;
                data->column = j;
                pthread_create(&threads[threadIndex++], NULL, is3x3Valid, data); // 3x3 subsection threads
            }
        }
    }

    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);			// Wait for all threads to finish
    }

    // If any of the entries in the valid array are 0, then the sudoku solution is invalid
    for (i = 0; i < num_threads; i++) {
        if (valid[i] == 0) {
            printf("Sudoku solution is invalid!\n");
            return EXIT_SUCCESS;
        }
    }
    printf("Sudoku solution is valid!\n");
    return EXIT_SUCCESS;
}