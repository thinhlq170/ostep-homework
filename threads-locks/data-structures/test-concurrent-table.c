#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <limits.h>
#include "hash-type.h"
#include "counter-type.h"

// #define THREADS_NUM 100
// #define OPS_PER_THREAD 50000make
#define MAX_BUFFER 50
#define THRESHOLD 500000000

typedef struct {
    hash_t *table;
    int thread_id;
} thread_arg_t;

typedef struct {
    counter_t *counter;
    int thread_id;
} counter_thread_arg_t;


long THREADS_NUM = 0;
long OPS_PER_THREAD = 0;

// TODO: implement another metric such as start and end current time using gettimeofday()
// static inline double timespec_diff_sec(struct timespec start, struct timespec end) {
//     return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
// }

double get_time_diff(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec) + (end->tv_usec - start->tv_usec) / 1000000.0;
}

void *worker(void *arg) {
    thread_arg_t *arg_t = (thread_arg_t *) arg;
    hash_t *table = arg_t->table;
    int tid = arg_t->thread_id;

    for (int i = 0; i < OPS_PER_THREAD; i++) {
        int key = (tid * OPS_PER_THREAD) + i;
        // if (Hash_Lookup(table, key) == -1) {
        //     Hash_Insert(table, key);
        // }
        Hash_Insert(table, key);
    }

    return NULL;
}

void *counter_worker(void *arg) {
    counter_thread_arg_t *c_thread_t = (counter_thread_arg_t *) arg;
    counter_t *counter = c_thread_t->counter;
    // int tid = c_thread_t->thread_id;

    for (int i = 0; i < OPS_PER_THREAD; i++) {
        // int key = (tid * OPS_PER_THREAD) + i;
        if (counter->value == THRESHOLD) {
            break;
        }
        Counter_Increment(counter);
    }

    return NULL;
}

int main(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "%s: <number of threads> <number of ops per each thread>\n", argv[0]);
        return 1;
    }

    char *endPtr;
    int falseConvert = 0;
    THREADS_NUM = strtol(argv[1], &endPtr, 10);
    falseConvert = endPtr == argv[1]
                    || (*endPtr != '\0' && *endPtr != '\n')
                    || (errno == ERANGE && (THREADS_NUM == LONG_MAX || THREADS_NUM == LONG_MIN));

    if (falseConvert) {
        fprintf(stderr, "Invalid input of number of threads are used!\n");
        return 1;
    }

    char *endOpsPtr;
    OPS_PER_THREAD = strtol(argv[2], &endOpsPtr, 10);
    falseConvert = endOpsPtr == argv[2]
                    || (*endOpsPtr != '\0' && *endOpsPtr != '\n')
                    || (errno == ERANGE && (OPS_PER_THREAD == LONG_MAX || OPS_PER_THREAD == LONG_MIN));
    
    if (falseConvert) {
        fprintf(stderr, "Invalid input of number of operations are proceeded!\n");
        return 1;
    }


    hash_t *table = malloc(sizeof(hash_t));
    Hash_Init(table);

    counter_t *counter = malloc(sizeof(counter_t));
    Counter_Init(counter);

    pthread_t threads[THREADS_NUM];
    // thread_arg_t args[THREADS_NUM];
    counter_thread_arg_t args[THREADS_NUM];
    // struct timespec start, end;
    struct timeval start, end;

    printf("Starting %ld threads, each doing %ld updates...\n", THREADS_NUM, OPS_PER_THREAD);
    gettimeofday(&start, NULL);

    // launch threads
    for (int i = 0; i < THREADS_NUM; i++) {
        if (counter->value == THRESHOLD) {
            break;
        }
        args[i].counter = counter;
        args[i].thread_id = i;
        pthread_create(&threads[i], NULL, counter_worker, &args[i]);
    }


    // Join threads
    for (int i = 0; i < THREADS_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);


    double elapsed = get_time_diff(&start, &end);
    // long total_ops = THREADS_NUM * OPS_PER_THREAD; // Operate Hash_Lookup and Hash_Insert


    // printf("Completed %ld operations in %.6f seconds\n", total_ops, elapsed);
    // printf("Throughput: %.2f ops/sec\n", total_ops / elapsed);
    printf("%ld,%.6f", THREADS_NUM, elapsed);

    free(table);
    free(counter);

    return 0;
}