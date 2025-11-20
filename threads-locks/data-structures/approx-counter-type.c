#include "approx-counter-type.h"


void init(approx_counter_t *C, int threshold) {
    C->threshold = threshold;
    C->global = 0;
    pthread_mutex_init(&C->glock, NULL);
    int i;
    for (i = 0; i < NUMCPUS; i++) {
        C->local[i] = 0;
        pthread_mutex_init(&C->llock[i], NULL);
    }
}

/* 
    update: usually, just grab local lock and update local amount;
    once it has risen 'threshold', grab global lock and transfer
    local values to it
*/
void update(approx_counter_t *C, int threadId, int amt) {
    int cpu = threadId % NUMCPUS;
    pthread_mutex_lock(&C->llock[cpu]);
    C->local[cpu] += amt;
    if (C->local[cpu] >= C->threshold) {
        // lock glabal value
        // send local value to global value
        // unlock global value
        pthread_mutex_lock(&C->glock);
        C->global += C->local[cpu];
        pthread_mutex_unlock(&C->glock);
        C->local[cpu] = 0; // after sending the value to global, reset the local
    }
    pthread_mutex_unlock(&C->llock[cpu]);
}

/* get: just return the global amount (approximate) */
int get(approx_counter_t *C) {
    pthread_mutex_lock(&C->glock);
    int val = C->global;
    pthread_mutex_unlock(&C->glock);
    return val;
}