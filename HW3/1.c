#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>


#define MAX_student 25 // i defined up to 25 student you can increase it by your own as requirment..

void *student(void *num);

void *teacher(void *);

void randwait(int secs);

sem_t waitingRoom;

sem_t Chair;

sem_t teacherPillow;

sem_t seatBelt;

int allDone = 0;

int main(int argc, char *argv[]) {
    pthread_t btid;
    pthread_t tid[MAX_student];
    long RandSeed;
    int i, numStudent, numChairs = 3;
    int Number[MAX_student];

    printf("Enter the number of studnet : ");
    scanf("%d", &numStudent);

    if (numStudent > MAX_student) {
        printf("The maximum number of student is %d.\n", MAX_student);
        exit(-1);
    }

    for (i = 0; i < MAX_student; i++) {
        Number[i] = i;
    }

    sem_init(&waitingRoom, 0, numChairs);
    sem_init(&Chair, 0, 1);
    sem_init(&teacherPillow, 0, 0);
    sem_init(&seatBelt, 0, 0);

    pthread_create(&btid, NULL, teacher, NULL);

    for (i = 0; i < numStudent; i++) {
        pthread_create(&tid[i], NULL, student, (void *) &Number[i]);
        sleep(1);
    }

    for (i = 0; i < numStudent; i++) {
        pthread_join(tid[i], NULL);
        sleep(1);
    }

    allDone = 1;
    sem_post(&teacherPillow);
    pthread_join(btid, NULL);
}

void *student(void *number) {
    int num = *(int *) number;

    printf("student %d leaving for teachers office.\n", num + 1);
    randwait(2);
    printf("student %d arrived at teachers office.\n", num + 1);

    sem_wait(&waitingRoom);
    printf("student %d seat on waiting chair.\n", num + 1);

    sem_wait(&Chair);

    sem_post(&waitingRoom);

    printf("student %d waking the techer.\n", num + 1);
    sem_post(&teacherPillow);

    sem_wait(&seatBelt);

    sem_post(&Chair);
    printf("student %d leaving teachers office.\n", num + 1);
}

void *teacher(void *junk) {

    while (!allDone) {

        printf("The teacher is sleeping\n");
        sem_wait(&teacherPillow);

        if (!allDone) {

            printf("The teacher is helping other student\n");
            randwait(2);
            printf("The teacher is free.\n");

            sem_post(&seatBelt);
        } else {
            printf("all student is free.\n");
        }
    }
}

void randwait(int secs) {
    int len;

    len = (int) ((1 * secs) + 1);
    sleep(len);
}