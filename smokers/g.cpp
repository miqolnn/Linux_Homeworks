#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define SEM_KEY 1234
#define NUM_SMOKERS 3

void sem_wait(int sem_id) {
    struct sembuf sb = {0, -1, 0};
    semop(sem_id, &sb, 1);
}

void sem_signal(int sem_id) {
    struct sembuf sb = {0, 1, 0};
    semop(sem_id, &sb, 1);
}

void handle_sigterm(int sig) {
    printf("Smoker process received SIGTERM. Exiting...\n");
    exit(0);
}

int main() {
    int sem_id[NUM_SMOKERS];
    pid_t pids[NUM_SMOKERS];

    for (int i = 0; i < NUM_SMOKERS; i++) {
        sem_id[i] = semget(SEM_KEY + i, 1, IPC_CREAT | 0666);
        if (sem_id[i] == -1) {
            perror("semget");
            exit(1);
        }
        semctl(sem_id[i], 0, SETVAL, 0);
    }

    for (int i = 0; i < NUM_SMOKERS; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            signal(SIGTERM, handle_sigterm);
            while (1) {
                sem_wait(sem_id[i]);
                if (i == 0) {
                    printf("T\n");
                } else if (i == 1) {
                    printf("P\n");
                } else if (i == 2) {
                    printf("M\n");
                }
                pause();
            }
        }
    }

    printf("Bartender is ready to assist smokers.\n");

    char input[100];
    printf("Enter a sequence of items (t, p, m): ");
    scanf("%s", input);

    for (int i = 0; i < strlen(input); i++) {
        char item = input[i];
        if (item == 't') {
            sem_signal(sem_id[0]);
        } else if (item == 'p') {
            sem_signal(sem_id[1]);
        } else if (item == 'm') {
            sem_signal(sem_id[2]);
        }
    }

    sleep(3);
    for (int i = 0; i < NUM_SMOKERS; i++) {
        kill(pids[i], SIGTERM);
    }

    for (int i = 0; i < NUM_SMOKERS; i++) {
        semctl(sem_id[i], 0, IPC_RMID);
    }

    printf("All processes finished.\n");

    return 0;
}

