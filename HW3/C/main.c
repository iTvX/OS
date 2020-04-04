#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>


sem_t walTeacher;
sem_t requestHelp;
sem_t occupy;
sem_t getChance;
int stuNum;
int *stuID;
int finished = 0;

pthread_t teacher;
pthread_t *students;


void *artTeacher(void *param)
{
    srand((unsigned)time(NULL));
    while (!finished) {
        printf("teacher takes a nap\n");
        sem_wait(&walTeacher);
        if (!finished) {
            printf("teacher helps student\n");
            sleep(rand()%2+1);
            printf("teacher now is free\n");
            sem_post(&requestHelp);
        }
        else {
            printf("teacher has finished helping all students\n");
        }
    }
}

void* stu(void *param)
{
    int *lnumber = (int *)param;
    int times_through_loop = 0;

    srand(time(NULL));

    while (times_through_loop < 1) {
        sem_wait(&occupy);
        printf("Student %d takes a seat\n",*lnumber);

        sem_wait(&getChance);
        sem_post(&occupy);
        printf("Student %d waking to the teacher.\n",*lnumber);
        sem_post(&walTeacher);
        printf("Student %d receiving help\n",*lnumber);
        sem_wait(&requestHelp);
        sem_post(&getChance);

        ++times_through_loop;
    }
    return NULL;
}

void init()
{
    int i;
    for (i = 0; i < stuNum; i++)
        stuID[i] = i;
    sem_init(&walTeacher, 0, 0);
    sem_init(&requestHelp, 0, 0);
    sem_init(&occupy, 0, 3);
    sem_init(&getChance, 0, 1);

}

void creStuThreads()
{
    int i;
    for (i = 0; i < stuNum; i++) {
        pthread_create(&students[i], 0, stu, (void *) &stuID[i]);
    }
}

void creTeachThreads()
{
    pthread_create(&teacher, 0, artTeacher, 0);
}

int main(void)
{
    int i;
    printf("please input numbers of students that need help: ");
    scanf("%d", &stuNum);
    stuID = (int *)malloc(sizeof(int) * stuNum);
    students = (pthread_t *)malloc(sizeof(pthread_t) * stuNum);
    init();
    creTeachThreads();
    creStuThreads();
    for (i = 0; i < stuNum; i++)
        pthread_join(students[i], NULL);
    finished = 1;
    printf("finished\n");
    // delete teacher pthread
    if (pthread_cancel(teacher) != 0)
        printf("error 2 %s\n",strerror(errno));
    free(stuID);
    free(students);

    return 0;
}
