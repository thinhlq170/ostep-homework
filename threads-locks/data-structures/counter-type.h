#ifndef COUNTER_INCLUDED
#define COUNTER_INCLUDED

#include <pthread.h>

typedef struct __counter_t {
    int value;
    pthread_mutex_t lock;
} counter_t;

extern void Counter_Init(counter_t *C);
extern void Counter_Increase(counter_t *C);
extern void Counter_Decrease(counter_t *C);
extern int Counter_Get(counter_t *C);




#endif