#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "hash-type.h"

#define THREADS_NUM 4
#define OPS_PER_THREAD 50000

typedef struct {
    hash_t *table;
    int thread_id;
} thread_arg_t;


// TODO: implement another metric such as start and end current time using gettimeofday()
static inline double timespec_diff_sec(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void *worker(void *arg) {
    thread_arg_t *arg_t = (thread_arg_t *) arg;
    hash_t *table = arg_t->table;
    int tid = arg_t->thread_id;

    for (int i = 0; i < OPS_PER_THREAD; i++) {
        int key = (tid * OPS_PER_THREAD) + i;
        Hash_Insert(table, key);
    }

    return NULL;
}

int main(int argc, char **argv) {

    hash_t *table = malloc(sizeof(hash_t));
    Hash_Init(table);

    pthread_t threads[THREADS_NUM];
    thread_arg_t args[THREADS_NUM];
    struct timespec start, end;

    printf("Starting %d threads, each doing %d updates...\n", THREADS_NUM, OPS_PER_THREAD);
    clock_gettime(CLOCK_MONOTONIC, &start);

    // launch threads
    for (int i = 0; i < THREADS_NUM; i++) {
        args[i].table = table;
        args[i].thread_id = i;
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }


    // Join threads
    for (int i = 0; i < THREADS_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);


    double elapsed = timespec_diff_sec(start, end);
    long total_ops = THREADS_NUM * OPS_PER_THREAD;


    printf("Completed %ld operations in %.6f seconds\n", total_ops, elapsed);
    printf("Throughput: %.2f ops/sec\n", total_ops / elapsed);

    free(table);

    return 0;
}