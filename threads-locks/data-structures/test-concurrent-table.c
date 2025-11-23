#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <limits.h>

#include "approx-counter-type.h"

// #define THREADS_NUM 100
#define OPS_PER_THREAD 1000000
#define THRESHOLD 5000000

typedef struct {
    approx_counter_t *approx_counter;
    int thread_id;
} approx_counter_arg_t;


// long OPS_PER_THREAD;

// TODO: implement another metric such as start and end current time using gettimeofday()
// static inline double timespec_diff_sec(struct timespec start, struct timespec end) {
//     return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
// }

double get_time_diff(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec) + (end->tv_usec - start->tv_usec) / 1000000.0;
}

// void *worker(void *arg) {
//     thread_arg_t *arg_t = (thread_arg_t *) arg;
//     hash_t *table = arg_t->table;
//     int tid = arg_t->thread_id;

//     for (int i = 0; i < OPS_PER_THREAD; i++) {
//         int key = (tid * OPS_PER_THREAD) + i;
//         // if (Hash_Lookup(table, key) == -1) {
//         //     Hash_Insert(table, key);
//         // }
//         Hash_Insert(table, key);
//     }

//     return NULL;
// }

void *approx_counter_worker(void *arg) {
    approx_counter_arg_t *arg_t = (approx_counter_arg_t *) arg;
    int thread_id = arg_t->thread_id;
    approx_counter_t *counter = arg_t->approx_counter;

    for (int i = 0; i < OPS_PER_THREAD; i++) {
        Approx_Counter_Update(counter, thread_id, 1);
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
    long THREADS_NUM = strtol(argv[1], &endPtr, 10);
    falseConvert = endPtr == argv[1]
                    || (*endPtr != '\0' && *endPtr != '\n')
                    || (errno == ERANGE && (THREADS_NUM == LONG_MAX || THREADS_NUM == LONG_MIN));

    if (falseConvert) {
        fprintf(stderr, "Invalid input of number of threads are used!\n");
        return 1;
    }

    // char *endOpsPtr;
    // OPS_PER_THREAD = strtol(argv[2], &endOpsPtr, 10);
    // falseConvert = endOpsPtr == argv[2]
    //                 || (*endOpsPtr != '\0' && *endOpsPtr != '\n')
    //                 || (errno == ERANGE && (OPS_PER_THREAD == LONG_MAX || OPS_PER_THREAD == LONG_MIN));
    
    if (falseConvert) {
        fprintf(stderr, "Invalid input of number of operations are proceeded!\n");
        return 1;
    }

    approx_counter_t *approx_counter = malloc(sizeof(approx_counter_t));
    approx_counter_arg_t args[THREADS_NUM];
    Approx_Counter_Init(approx_counter, THRESHOLD);

    pthread_t threads[THREADS_NUM];
    // thread_arg_t args[THREADS_NUM];
    
    // struct timespec start, end;
    struct timeval start, end;

    // printf("Starting %ld threads, each doing %d updates...\n", THREADS_NUM, OPS_PER_THREAD);
    gettimeofday(&start, NULL);

    // launch threads
    for (int i = 0; i < THREADS_NUM; i++) {
        
        args[i].approx_counter = approx_counter;
        args[i].thread_id = i;
        pthread_create(&threads[i], NULL, approx_counter_worker, &args[i]);
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
    free(approx_counter);

    return 0;
}