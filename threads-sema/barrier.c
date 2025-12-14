#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common_threads.h"

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

// You likely need two semaphores to do this correctly, and some
// other integers to track things.

typedef struct __barrier_t
{
    // add semaphores and other information here

    int count;
    sem_t incompleted;
    sem_t completed;
} barrier_t;

// the single barrier we are using for this program
barrier_t b;

void barrier_init(barrier_t *b, int num_threads)
{
    // initialization code goes here
    b->count = num_threads;
    Sem_init(&b->incompleted, 1);
    Sem_init(&b->completed, 0);
}

void barrier(barrier_t *b)
{
    /* My barrier */
    // barrier code goes here
    /* Lock to update the count */
    Sem_wait(&b->incompleted);
    b->count--;
    /* If the count > 0 it means other threads do not arrive the barrier yet
        Therefore, wake others to arrive to the barrier AND this thread shall sleep (wait) for the others complete their arrivals
    */
    while (b->count > 0)
    {
        Sem_post(&b->incompleted);
        /* If the last thread arrives the barrier, it shall wake here */
        Sem_wait(&b->completed);
    }
    /* When the last thread arrives the barrier, it shall wake others */
    Sem_post(&b->completed);

    /* LittleBookOfSemaphores' barrier */
    /* Lock to update the count */
    // Sem_wait(&b->incompleted);
    // b->count--;
    // Sem_post(&b->incompleted);

    // /* If all threads arrive the barrier then wake others */
    // if (b->count == 0)
    //     Sem_post(&b->completed);

    // Sem_wait(&b->completed);
    // /* After waiting for the last thread arrive to the barrier, each thread shall sequentially wake others */
    // Sem_post(&b->completed);
}

//
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t
{
    int thread_id;
} tinfo_t;

void *child(void *arg)
{
    tinfo_t *t = (tinfo_t *)arg;
    printf("\033[0;31m"); // Set text color to red
    printf("child %d: before\n", t->thread_id);
    printf("\033[0m"); // Reset text color to default
    barrier(&b);
    printf("child %d: after\n", t->thread_id);
    return NULL;
}

// run with a single argument indicating the number of
// threads you wish to create (1 or more)
int main(int argc, char *argv[])
{
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);

    int i;
    for (i = 0; i < num_threads; i++)
    {
        t[i].thread_id = i;
        Pthread_create(&p[i], NULL, child, &t[i]);
    }

    for (i = 0; i < num_threads; i++)
        Pthread_join(p[i], NULL);

    printf("parent: end\n");
    return 0;
}
