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
void trans_ES(char userInput[], char *ES[][tableColumn])
{
    int i = 0;
    while (i < tableRow)
    {
        //check the first column if can find user's input
        if (strcmp(ES[i][0], userInput) == 0)
        {
            printf("%s\n", ES[i][1]);
            return;
        }
        i++;
    }
    printf("%s is an English word that has no Spanish translation present in dictionary\n", userInput);
}

//same, but check the second column.
void trans_SE(char userInput[], char *ES[][tableColumn])
{
    int i = 0;
    while (i < tableRow)
    {
        if (strcmp(ES[i][1], userInput) == 0)
        {
            printf("%s\n", ES[i][0]);
            return;
        }
        i++;
    }
    printf("%s is an Spanish word that has no English translation present in dictionary\n", userInput);
}

void trans_EF(char userInput[], char *EF[][tableColumn])
{
    int i = 0;
    while (i < tableRow)
    {
        if (strcmp(EF[i][0], userInput) == 0)
        {
            printf("%s\n", EF[i][1]);
            return;
        }
        i++;
    }
    printf("%s is an English word that has no French translation present in dictionary\n", userInput);
}

void trans_FE(char userInput[], char *EF[][tableColumn])
{
    int i = 0;
    while (i < tableRow)
    {
        if (strcmp(EF[i][1], userInput) == 0)
        {
            printf("%s\n", EF[i][0]);
            return;
        }
        i++;
    }
    printf("%s is an French word that has no English translation present in dictionary\n", userInput);
}

char **take_input(char *input)
{
    char **command = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL)
    {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;
}

//main function begin
int main()
{
    //create user input char
    char *input;
    char **command;

    pid_t p1;
    //Since I found C language doesn't have "true", so I use "1".
    p1 = fork();

    if (p1 == 0)
    {
        while (1)
        {
            input = readline("Translator> ");
            command = take_input(input);

            if (strcmp(command[0], "trans_ES") == 0)
            {
                trans_ES(command[1], esTable);
            }
            else if (strcmp(command[0], "trans_SE") == 0)
            {
                trans_SE(command[1], esTable);
            }
            else if (strcmp(command[0], "trans_EF") == 0)
            {
                trans_EF(command[1], efTable);
            }
            else if (strcmp(command[0], "trans_FE") == 0)
            {
                trans_FE(command[1], efTable);
            }
            else if (strcmp(command[0], "exit") == 0)
            {
                exit(0);
            }
            else
            {
                printf("Command issued not recognised.\n");
            }
        }
    }
    wait(NULL);
    return 0;
}
