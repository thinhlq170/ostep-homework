#include "counter-type.h"


void Counter_Init(counter_t *C) {
    C->value = 0;
    pthread_mutex_init(&C->lock, NULL);
}

void Counter_Increment(counter_t *C) {
    pthread_mutex_lock(&C->lock);
    C->value++;
    pthread_mutex_unlock(&C->lock);
}

void Counter_Decrement(counter_t *C) {
    pthread_mutex_lock(&C->lock);
    C->value--;
    pthread_mutex_unlock(&C->lock);
}

int Counter_Get(counter_t *C) {
    pthread_mutex_lock(&C->lock);
    int val = C->value;
    pthread_mutex_unlock(&C->lock);
    return val;
}