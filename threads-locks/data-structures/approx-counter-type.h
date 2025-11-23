#ifndef APRROXIMATE_COUTER_INCLUDED
#define APRROXIMATE_COUTER_INCLUDED

#include <pthread.h>

#define NUMCPUS 4

typedef struct __approx_counter_t {
    int                 global;                     // global count
    pthread_mutex_t     glock;                      // global lock
    int                 local[NUMCPUS];             // per-CPU count
    pthread_mutex_t     llock[NUMCPUS];             // ... per lock
    int                 threshold;                  // limit to start update
} approx_counter_t;

void Approx_Counter_Init(approx_counter_t *C, int threshold);
void Approx_Counter_Update(approx_counter_t *C, int threadId, int amt);
int Approx_Counter_Get(approx_counter_t *C);


#endif