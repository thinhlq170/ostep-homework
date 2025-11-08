#include "hash-type.h"

void Hash_Init(hash_t *H) {
    for (int i = 0; i < BUCKETS; i++) {
        List_Init(&H->lists[i]);
    }
}

int Hash_Insert(hash_t *H, int key) {
    return List_Insert(H->lists[key % BUCKETS], key);
}

int Hash_Lookup(hash_t *H, int key) {
    return List_Lookup(H->lists[key % BUCKETS], key);
}