#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_ITEMS 5

int buffer[BUFFER_SIZE], in = 0, out = 0;
sem_t empty, full, mutex;

void display_buffer() {
    printf("Buffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++)
        printf("%d ", buffer[i]);
    printf("\n");
}

void* producer(void* id) {
    int pid = *(int*)id;
    for (int i = 0; i < NUM_ITEMS; i++) {
        int item = rand() % 100;
        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = item;
        printf("Producer %d produced item %d at position %d\n", pid, item, in);
        in = (in + 1) % BUFFER_SIZE;
        display_buffer();

        sem_post(&mutex);
        sem_post(&full);
        usleep(rand() % 500000);
    }
    printf("Producer %d finished production\n", pid);
    return NULL;
}

void* consumer(void* id) {
    int cid = *(int*)id;
    for (int i = 0; i < (NUM_ITEMS * NUM_PRODUCERS) / NUM_CONSUMERS; i++) {
        sem_wait(&full);
        sem_wait(&mutex);

        int item = buffer[out];
        printf("Consumer %d consumed item %d from position %d\n", cid, item, out);
        buffer[out] = -1;
        out = (out + 1) % BUFFER_SIZE;
        display_buffer();

        sem_post(&mutex);
        sem_post(&empty);
        usleep(rand() % 500000);
    }
    printf("Consumer %d finished consumption\n", cid);
    return NULL;
}

int main() {
    for (int i = 0; i < BUFFER_SIZE; i++) buffer[i] = -1;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t prod[NUM_PRODUCERS], cons[NUM_CONSUMERS];
    int ids[NUM_PRODUCERS > NUM_CONSUMERS ? NUM_PRODUCERS : NUM_CONSUMERS];

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        ids[i] = i + 1;
        pthread_create(&prod[i], NULL, producer, &ids[i]);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        ids[i] = i + 1;
        pthread_create(&cons[i], NULL, consumer, &ids[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) pthread_join(prod[i], NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) pthread_join(cons[i], NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    printf("All threads completed successfully\n");
    return 0;
}
