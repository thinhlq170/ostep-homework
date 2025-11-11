#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <limits.h>
#include "hash-type.h"

// #define THREADS_NUM 100
// #define OPS_PER_THREAD 50000
#define MAX_BUFFER 50

typedef struct {
    hash_t *table;
    int thread_id;
} thread_arg_t;


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

int main(int argc, char **argv) {

    char *thread_num_buff = malloc(sizeof(char) * MAX_BUFFER);
    char *ops_thread_buff = malloc(sizeof(char) * MAX_BUFFER);

    
    char *endPtr;

    int falseConvert = 0;
    printf("How many threads are used: ");
    if (fgets(thread_num_buff, MAX_BUFFER, stdin) != NULL) {
        THREADS_NUM = strtol(thread_num_buff, &endPtr, 10);
        falseConvert = endPtr == thread_num_buff 
                        || (*endPtr != '\0' && *endPtr != '\n')
                        || (errno == ERANGE && (THREADS_NUM == LONG_MAX || THREADS_NUM == LONG_MIN));
        
        free(thread_num_buff);
    }

    if (falseConvert) {
        fprintf(stderr, "Invalid input of number of threads are used!\n");
        return 1;
    }

    char *endOpsPtr;
    printf("How many operations shall be proceeded: ");
    if (fgets(ops_thread_buff, MAX_BUFFER, stdin) != NULL) {
        OPS_PER_THREAD = strtol(ops_thread_buff, &endOpsPtr, 10);
        falseConvert = endOpsPtr == ops_thread_buff 
                        || (*endOpsPtr != '\0' && *endOpsPtr != '\n')
                        || (errno == ERANGE && (OPS_PER_THREAD == LONG_MAX || OPS_PER_THREAD == LONG_MIN));
        
        free(ops_thread_buff);
    }
    if (falseConvert) {
        fprintf(stderr, "Invalid input of number of operations are proceeded!\n");
        return 1;
    }


    hash_t *table = malloc(sizeof(hash_t));
    Hash_Init(table);

    pthread_t threads[THREADS_NUM];
    thread_arg_t args[THREADS_NUM];
    // struct timespec start, end;
    struct timeval start, end;

    printf("Starting %ld threads, each doing %ld updates...\n", THREADS_NUM, OPS_PER_THREAD);
    gettimeofday(&start, NULL);

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

    gettimeofday(&end, NULL);


    double elapsed = get_time_diff(&start, &end);
    long total_ops = THREADS_NUM * OPS_PER_THREAD; // Operate Hash_Lookup and Hash_Insert


    printf("Completed %ld operations in %.6f seconds\n", total_ops, elapsed);
    printf("Throughput: %.2f ops/sec\n", total_ops / elapsed);

    free(table);

    return 0;
}