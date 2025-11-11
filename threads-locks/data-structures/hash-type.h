#ifndef HASH_INCLUDED
#define HASH_INCLUDED

#include "list-type.h"

#define BUCKETS (101)

typedef struct __hash_t {
    list_t lists[BUCKETS];
} hash_t;

extern void Hash_Init(hash_t *H);
extern int Hash_Insert(hash_t *H, int key);
extern int Hash_Lookup(hash_t *H, int key);


#endif