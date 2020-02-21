// Please read README.pdf before you run.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid > 0) {
        printf("Welcome to use my shell \n");
        wait(NULL);

    } else {
        execl("./part1", "output", NULL);
    }

    return 0;
}

