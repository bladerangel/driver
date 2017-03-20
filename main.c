#include <stdio.h>
#include <process.h>

int channel[6] = {-1, -1, -1, -1, -1, -1};

void version1() {
    int status;
    int buffer = 5;
    sendAsync(&buffer, 0);
    receive(&status, 3);
    if (!status) {
        printf("voto errado version 1\n");
        return;
    } else {
        printf("version 1 em execucao\n");
        while (1) {}
    }
    return;
}

void version2() {
    int status;
    int buffer = 15;
    sendAsync(&buffer, 1);
    receive(&status, 4);
    if (!status) {
        printf("voto errado version 2\n");
        return;
    } else {
        printf("version 2 em execucao\n");
        while (1) {}
    }
    return;
}

void version3() {
    int status;
    int buffer = 15;
    sendAsync(&buffer, 2);
    receive(&status, 5);
    if (!status) {
        printf("voto errado version 3\n");
        return;
    } else {
        printf("version 3 em execucao\n");
        while (1) {}
    }
    return;
}

void driver() {
    start_thread();
    int votes[3];
    int status[3] = {0, 0, 0};

    for (int i = 0; i < 3; ++i) {
        receive(&votes[i], i);
    }

    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (votes[i] == votes[j]) {
                status[i] = 1;
                status[j] = 1;
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        if (status[i]) {
            printf("version %d possui o voto majoritario: %d\n", i + 1, votes[i]);
        } else {
            printf("version %d possui o voto minoritario: %d\n", i + 1, votes[i]);
        }
        sendAsync(&status[i], 3 + i);
    }
    return;
}

void sendAsync(int *buffer, int index) {
    channel[index] = *buffer;
    return;
}

void receive(int *buffer, int index) {
    while (channel[index] == -1) {}
    *buffer = channel[index];
    channel[index] = -1;
    return;
}


void start_thread() {
    int thread_id;

#if     defined(__WIN32__)
    if ((thread_id = _beginthread(version1, 4096, NULL)) == (unsigned long) -1)
#else
        if ((thread_id = _beginthread(version1, 4096, NULL)) == -1)
#endif
    {
        printf("Unable to create thread version 1.\n");
        return;
    }
    printf("Created thread version 1.\n");

#if     defined(__WIN32__)
    if ((thread_id = _beginthread(version2, 4096, NULL)) == (unsigned long) -1)
#else
        if ((thread_id = _beginthread(version2, 4096, NULL)) == -1)
#endif
    {
        printf("Unable to create thread version 2.\n");
        return;
    }
    printf("Created thread version 2.\n");

#if     defined(__WIN32__)
    if ((thread_id = _beginthread(version3, 4096, NULL)) == (unsigned long) -1)
#else
        if ((thread_id = _beginthread(version3, 4096, NULL)) == -1)
#endif
    {
        printf("Unable to create thread version 3.\n");
        return;
    }
    printf("Created thread version 3.\n");
}

int main() {
    driver();
    getchar();
    return 0;
}