/* Name: Zijie Yu
 *
 * Please read README.pdf before you run.
 * This code is the modify version of my part 1 code.
 *
 * I used readline, if you want to run this program, you must install readline first:
 * sudo apt-get install libreadline-dev
 *
 * how to run?
 * gcc part1.c -lreadline -o part1; ./part1
 *
 * reference:
 * https://indradhanush.github.io/blog/writing-a-unix-shell-part-2/
 * Since this is my first time to program a linux shell, I got no idea how to do it, I choose use readline to program my shell.
 * I got an idea from above link and use partial algorithm: how to handle user input and takes a string as the input.
 * */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>

//After I checked the given table, each row for the table is 8 and and each column is 2.
#define tableRow 8
#define tableColumn 2

//store vocabulary
char *esTable[tableRow][tableColumn] = {
        {"cat", "gato"},
        {"dog", "perro"},
        {"bird", "pajaro"},
        {"mouse", "raton"},
        {"cow", "vaca"},
        {"tiger", "tigre"},
        {"horse", "caballo"},
        {"monkey", "mono"}};

char *efTable[tableRow][tableColumn] = {
        {"cat", "chat"},
        {"dog", "chian"},
        {"bird", "oiseau"},
        {"mouse", "souris"},
        {"cow", "vache"},
        {"tiger", "tigre"},
        {"horse", "cheval"},
        {"monkey", "singe"}};

//Crate Translation function.
void trans_ES(char userInput[], char *ES[][tableColumn]){

    int i = 0;
    while (i < tableRow){

        //check the first column if can find user's input
        if (strcmp(ES[i][0], userInput) == 0){

            printf("%s\n", ES[i][1]);
            return;
        }
        i++;
    }
    printf("%s is an English word that has no Spanish translation present in dictionary\n", userInput);
}

//same, but check the second column.
void trans_SE(char userInput[], char *ES[][tableColumn]){

    int i = 0;
    while (i < tableRow){

        if (strcmp(ES[i][1], userInput) == 0){

            printf("%s\n", ES[i][0]);
            return;
        }
        i++;
    }
    printf("%s is an Spanish word that has no English translation present in dictionary\n", userInput);
}

void trans_EF(char userInput[], char *EF[][tableColumn]){

    int i = 0;
    while (i < tableRow){

        if (strcmp(EF[i][0], userInput) == 0){

            printf("%s\n", EF[i][1]);
            return;
        }
        i++;
    }
    printf("%s is an English word that has no French translation present in dictionary\n", userInput);
}

void trans_FE(char userInput[], char *EF[][tableColumn]){

    int i = 0;
    while (i < tableRow){

        if (strcmp(EF[i][1], userInput) == 0){

            printf("%s\n", EF[i][0]);
            return;
        }
        i++;
    }
    printf("%s is an French word that has no English translation present in dictionary\n", userInput);
}


char **userInput(char *input){

    char **command = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL){
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;
}

//main function begin
int main() {
    //create user input char
    char *input;
    char **command;

    pid_t p1;
    p1 = fork();

    if(p1 > 0) {
        wait(NULL);
        printf("Thanks for using! \n");
    } else if (p1 < 0) {
        printf("Something went wrong");
    }else {
        while (1) {
            input = readline("Translator> ");
            command = userInput(input);
            if (strcmp(command[0], "trans_ES") == 0) {
                trans_ES(command[1], esTable);
            }
            else if (strcmp(command[0], "trans_SE") == 0) {
                trans_SE(command[1], esTable);
            }
            else if (strcmp(command[0], "trans_EF") == 0) {
                trans_EF(command[1], efTable);
            }
            else if (strcmp(command[0], "trans_FE") == 0) {
                trans_FE(command[1], efTable);
            }
            else if (strcmp(command[0], "exit") == 0) {
                exit(0);
            }
            else {
                printf("We can't find this command.\n");
            }
        }
    }
    // parent process wait.
    wait(NULL);
    return 0;
}
