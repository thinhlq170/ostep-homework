#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <limits.h>

#include "counter-type.h"

#define OPS_PER_THREAD 1000000

typedef struct {
    counter_t *counter;
    int thread_id;
} counter_thread_arg_t;


// TODO: implement another metric such as start and end current time using gettimeofday()
// static inline double timespec_diff_sec(struct timespec start, struct timespec end) {
//     return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
// }

double get_time_diff(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec) + (end->tv_usec - start->tv_usec) / 1000000.0;
}


void *counter_worker(void *arg) {
    counter_thread_arg_t *c_thread_t = (counter_thread_arg_t *) arg;
    counter_t *counter = c_thread_t->counter;

    for (int i = 0; i < OPS_PER_THREAD; i++) {
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


    

    counter_t *counter = malloc(sizeof(counter_t));
    counter_thread_arg_t args[THREADS_NUM];
    Counter_Init(counter);

    

    pthread_t threads[THREADS_NUM];
    // thread_arg_t args[THREADS_NUM];
    
    // struct timespec start, end;
    struct timeval start, end;

    // printf("Starting %ld threads, each doing %d updates...\n", THREADS_NUM, OPS_PER_THREAD);
    gettimeofday(&start, NULL);

    // launch threads
    for (int i = 0; i < THREADS_NUM; i++) {
        
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


    // printf("Completed %ld operations in %.6f seconds\n", total_ops, elapsed);
    // printf("Throughput: %.2f ops/sec\n", total_ops / elapsed);
    printf("%ld,%.6f", THREADS_NUM, elapsed);
    free(counter);

    return 0;
}