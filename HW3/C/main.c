#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_SLEEP_TIME  5


int finished = 0;
sem_t waking_teacher;
sem_t request_teacher_help;
sem_t occupy_seat;
sem_t get_chance;
int total_stu_number;
int *student_id;

pthread_t teacher;
pthread_t *students;


void *teacher_loop(void *param)
{
    int sleep_time;

    /* seed random generator */
    srand((unsigned)time(NULL));

    while (!finished) {
        printf("teacher is sleeping\n");
        /* wake up the teacher*/
        sem_wait(&waking_teacher);
        if (!finished) {
            printf("teacher help student\n");
            sleep(rand()%2+1);
            printf("teacher now is free\n");
            sem_post(&request_teacher_help);
        }
        else {
            printf("finish helping all students\n");
        }
    }
}

void* student_loop(void *param)
{
    int *lnumber = (int *)param;
    int times_through_loop = 0;

    srand(time(NULL));

    while (times_through_loop < 1) {
        sem_wait(&occupy_seat);
        printf("Student %d takes a seat\n",*lnumber);
        sem_wait(&get_chance);
        sem_post(&occupy_seat);
        printf("Student %d waking the teacher.\n",*lnumber);
        sem_post(&waking_teacher);
        printf("Student %d receiving help\n",*lnumber);
        sem_wait(&request_teacher_help);
        sem_post(&get_chance);

        ++times_through_loop;
    }
    return NULL;
}

void init()
{
    int i;
    for (i = 0; i < total_stu_number; i++)
        student_id[i] = i;
    sem_init(&waking_teacher, 0, 0);
    sem_init(&request_teacher_help, 0, 0);
    sem_init(&occupy_seat, 0, 3);
    sem_init(&get_chance, 0, 1);

}

void create_student_pthreads()
{
    int i;
    for (i = 0; i < total_stu_number; i++) {
        pthread_create(&students[i], 0, student_loop, (void *)&student_id[i]);
    }
}

void create_teacher_pthread()
{
    pthread_create(&teacher, 0, teacher_loop, 0);
}

int main(void)
{
    int i;
    printf("please input numbers of students: ");
    scanf("%d", &total_stu_number);
    student_id = (int *)malloc(sizeof(int)*total_stu_number);
    students = (pthread_t *)malloc(sizeof(pthread_t)*total_stu_number);
    init();
    create_teacher_pthread();
    create_student_pthreads();
    for (i = 0; i < total_stu_number; i++)
        pthread_join(students[i], NULL);
    finished = 1;
    printf("finished\n");
    // delete teacher pthread
    if (pthread_cancel(teacher) != 0)
        printf("error 2 %s\n",strerror(errno));
    free(student_id);
    free(students);

    return 0;
}
