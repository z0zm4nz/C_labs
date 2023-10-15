#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 512
#define M 512
#define NUM_THREADS 4

int A[N][M];
int max_value = INT_MIN;
pthread_mutex_t lock;

void* find_max(void* arg) {
    int thread_id = *(int*)arg;
    int local_max = INT_MIN;

    for (int i = thread_id; i < N; i += NUM_THREADS) {
        for (int j = 0; j < M; j++) {
            if (A[i][j] > local_max) {
                local_max = A[i][j];
            }
        }
    }

    pthread_mutex_lock(&lock);
    if (local_max > max_value) {
        max_value = local_max;
    }
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    pthread_mutex_init(&lock, NULL);

    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            A[i][j] = rand() % 1000;
        }
    }

    clock_t start_time = clock();

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, find_max, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Max value: %d\n", max_value);
    printf("Running time: %.4lf seconds\n", execution_time);

    pthread_mutex_destroy(&lock);

    return 0;
}
