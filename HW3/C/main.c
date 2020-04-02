#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_SLEEP_TIME  5
#define MAX_WAITING_STUDENTS 3
#define NUM_OF_SEATS 3
pthread_mutex_t mutex_lock;

sem_t *students_sem;
sem_t *teacher_sem;
int total_stu_number;
int waiting_students;
int student_number;
int *student_id;

pthread_t teacher;
pthread_t *students;


void help_student(int sleep_time)
{
    printf("Helping a student for %d seconds waiting students = %d\n",sleep_time, waiting_students);

    sleep(sleep_time);
}

void *teacher_loop(void *param)
{
    int sleep_time;

    /* seed random generator */
    srand((unsigned)time(NULL));

    while (1) {

        /* wait for a student to show up */
        if ( sem_wait(students_sem) != 0)
            printf("%s\n",strerror(errno));

        /* acquire the mutex lock */
        if (pthread_mutex_lock(&mutex_lock) != 0)
            printf("%s\n",strerror(errno));

        --waiting_students;

        /* indicate the teacher is ready to help a student */
        if (sem_post(teacher_sem) != 0)
            printf("%s\n",strerror(errno));

        /* release mutex lock */
        if (pthread_mutex_unlock(&mutex_lock) != 0)
            printf("%s\n",strerror(errno));

        sleep_time = (int)((random() % MAX_SLEEP_TIME) + 1);
        help_student(sleep_time);
    }
}

void hang_out(int i, int sleep_time)
{
    printf("Student %d hanging out for %d seconds\n",i,sleep_time);
    sleep(sleep_time);
}
void* student_loop(void *param)
{
    int *lnumber = (int *)param;
    int number = *lnumber;
    int sleep_time;
    int times_through_loop = 0;

    srand(time(NULL));

    while (times_through_loop < 5) {
        sleep_time = (int)((random() % MAX_SLEEP_TIME) + 1);
        hang_out(*lnumber,sleep_time);

        /* acquire the mutex lock */
        if ( pthread_mutex_lock(&mutex_lock) != 0)
            printf("Student %s\n",strerror(errno));

        if (waiting_students < NUM_OF_SEATS) {
            ++waiting_students;
            printf("Student %d takes a seat waiting = %d\n",*lnumber, waiting_students);

            if (sem_post(students_sem) != 0)
                printf("Student %s\n",strerror(errno));

            if (pthread_mutex_unlock(&mutex_lock) != 0)
                printf("Student %s\n",strerror(errno));

            if (sem_wait(teacher_sem) != 0)
                printf("Student %s\n",strerror(errno));

            printf("Student %d receiving help\n",*lnumber);


            ++times_through_loop;
        } else {
            printf("Student %d will try later\n",*lnumber);
            pthread_mutex_unlock(&mutex_lock);
        }
    }
    return NULL;
}

void initilize()
{
    int i;
    if ( pthread_mutex_init(&mutex_lock, NULL) != 0)
        fprintf(stderr, "%s\n",strerror(errno));

    if (sem_unlink("STUDENTS") == -1)
        fprintf(stderr, "%s\n",strerror(errno));

    if (sem_unlink("teacher") == -1)
        fprintf(stderr, "%s\n",strerror(errno));

    if ( (students_sem = sem_open("STUDENTS", O_CREAT, 0666, 0)) == SEM_FAILED)
        fprintf(stderr, "%s\n",strerror(errno));

    if ( (teacher_sem = sem_open("teacher", O_CREAT, 0666, 0)) == SEM_FAILED)
        fprintf(stderr,"%s\n",strerror(errno));

    waiting_students = 0;

    for (i = 0; i < total_stu_number; i++)
        student_id[i] = i;
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
    initilize();
    create_teacher_pthread();
    create_student_pthreads();
    for (i = 0; i < total_stu_number; i++)
        pthread_join(students[i], NULL);

    // delete teacher pthread
    if (pthread_cancel(teacher) != 0)
        printf("%s\n",strerror(errno));
    free(student_id);
    free(students);

    return 0;
}
