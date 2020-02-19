#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();

    if (pid > 0) {
        wait(NULL);
        execl("./part1", "output", NULL);

    } else {
        printf("Welcome to use my shell \n");
    }

    return 0;
}

