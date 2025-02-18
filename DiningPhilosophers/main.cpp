#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>

#define PHILOSOPHERS 5
#define ITERATIONS 3

sem_t *sticks[PHILOSOPHERS];

void think(int id) {
    printf("Philosopher %d is thinking.\n", id);
    usleep(rand() % 1000000);
}

void eat(int id) {
    printf("Philosopher %d is eating.\n", id);
    usleep(rand() % 1000000);
}

void philosopher(int id) {
    srand(time(NULL) ^ getpid());
    int left = id;
    int right = (id + 1) % PHILOSOPHERS;

    if (id == 0) { 
        int temp = left;
        left = right;
        right = temp;
    }

    for (int i = 0; i < ITERATIONS; i++) {
        think(id);
        sem_wait(sticks[left]);
        sem_wait(sticks[right]);
        eat(id);
        sem_post(sticks[right]);
        sem_post(sticks[left]);
    }
    exit(0);
}

int main() {
    pid_t pids[PHILOSOPHERS];
    char sem_name[16];

    
    for (int i = 0; i < PHILOSOPHERS; i++) {
        snprintf(sem_name, sizeof(sem_name), "/stick_%d", i);
        sem_unlink(sem_name);
        sticks[i] = sem_open(sem_name, O_CREAT | O_EXCL, 0666, 1);
        if (sticks[i] == SEM_FAILED) {
            perror("sem_open");
            exit(1);
        }
    }

    
    for (int i = 0; i < PHILOSOPHERS; i++) {
        if ((pids[i] = fork()) == 0) {
            philosopher(i);
        }
    }

    
    for (int i = 0; i < PHILOSOPHERS; i++) {
        waitpid(pids[i], NULL, 0);
    }

    
    for (int i = 0; i < PHILOSOPHERS; i++) {
        snprintf(sem_name, sizeof(sem_name), "/stick_%d", i);
        sem_close(sticks[i]);
        sem_unlink(sem_name);
    }
    
    return 0;
}

