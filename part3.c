//  Please read README.pdf before you run.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>

//After I checked the given table, each row for the table is 8 and and each column is 2.
#define tableRow 8
#define tableColumn 2

//store vocabulary
char *esTable[tableRow][tableColumn] = {
        {"cat",    "gato"},
        {"dog",    "perro"},
        {"bird",   "pajaro"},
        {"mouse",  "raton"},
        {"cow",    "vaca"},
        {"tiger",  "tigre"},
        {"horse",  "caballo"},
        {"monkey", "mono"}};

char *efTable[tableRow][tableColumn] = {
        {"cat",    "chat"},
        {"dog",    "chian"},
        {"bird",   "oiseau"},
        {"mouse",  "souris"},
        {"cow",    "vache"},
        {"tiger",  "tigre"},
        {"horse",  "cheval"},
        {"monkey", "singe"}};

//Crate Translation function.
char *trans_ES(char userInput[], char *ES[][tableColumn]) {
    int i = 0;
    while (i < tableRow) {
        //check the first column if can find user's input
        if (strcmp(ES[i][0], userInput) == 0) {
            return ES[i][1];
        }
        i++;
    }
    printf("%s is an English word that has no Spanish translation present in dictionary\n", userInput);
}

//same, but check the second column.
char *trans_SE(char userInput[], char *ES[][tableColumn]) {
    int i = 0;
    while (i < tableRow) {
        if (strcmp(ES[i][1], userInput) == 0) {
            return ES[i][0];
        }
        i++;
    }
    printf("%s is an Spanish word that has no English translation present in dictionary\n", userInput);
}

char *trans_EF(char userInput[], char *EF[][tableColumn]) {
    int i = 0;
    while (i < tableRow) {
        if (strcmp(EF[i][0], userInput) == 0) {
            return EF[i][1];
        }
        i++;
    }
    printf("%s is an English word that has no French translation present in dictionary\n", userInput);
}

char *trans_FE(char userInput[], char *EF[][tableColumn]) {
    int i = 0;
    while (i < tableRow) {
        if (strcmp(EF[i][1], userInput) == 0) {
            return EF[i][0];
        }
        i++;
    }
    printf("%s is an French word that has no English translation present in dictionary\n", userInput);
}

// read the command from the shell.
char **take_input(char *input) {
    char **command = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL) {
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
    char *toEng;
    
    int fd1[2]; // Pipe to send data from parent to child
    int fd2[2]; // pipe to send data from child to parent

    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 

    pid_t p1;

    p1 = fork();
    while (1) {
        if (p1 > 0) {
            input = readline("Translator> ");
            command = take_input(input);
            char *toTrans;

            if (strcmp(command[0], "trans_SF") == 0) {
                toEng = trans_SE(command[1], esTable);
                toTrans = trans_EF(toEng, efTable);
            } else if (strcmp(command[0], "trans_FS") == 0) {
                toEng = trans_FE(command[1], efTable);
                toTrans = trans_EF(toEng, esTable);
            } else if (strcmp(command[0], "trans_ES") == 0) {
                toTrans = trans_ES(command[1], esTable);
            } else if (strcmp(command[0], "trans_SE") == 0) {
                toTrans = trans_SE(command[1], esTable);
            } else if (strcmp(command[0], "trans_EF") == 0) {
                toTrans = trans_EF(command[1], efTable);
            } else if (strcmp(command[0], "trans_FE") == 0) {
                toTrans = trans_FE(command[1], efTable);
            } else if (strcmp(command[0], "exit") == 0) {
                exit(0);
            } else {
                printf("Commmand not recognised.\n");
            }

            close(fd1[0]); // close reading end of the first pipe
            write(fd1[1], toEng, strlen(toEng) + 1);
            close(fd1[1]);

            // wait for child to pass the engish word
            wait(NULL);

            close(fd2[1]); // close writing end of the second pipe

            read(fd2[0], toTrans, 100);
            if (toTrans[0] != '\0') {
                printf("%s\n", toTrans);
            }

            close(fd2[0]);
        }
            // child process
        else {
            close(fd1[1]);
            char *toTrans;
            read(fd1[0], toTrans, 100);

            if (strcmp(command[0], "trans_FS")) {
                toTrans = trans_ES(toEng, esTable);
            }
            if (strcmp(command[0], "trans_SF")) {
                toTrans = trans_EF(toEng, efTable);
            }

            close(fd1[0]);
            close(fd2[0]);

            write(fd2[1], toTrans, strlen(toTrans) + 1);
            close(fd2[1]);

            exit(0);
        }
    }
    wait(NULL);
    wait(NULL);
    return 0;
}
