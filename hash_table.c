#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct hash_entry {
    int key;
    int is_deleted;
} HashEntry;

typedef struct hash_table {
    HashEntry *table;
    int size;
    int count;
} HashTable;

int hash1(int key, int size) {
    return key % size;              // we find the index
}

int hash2(int key, int size) {
    return (key % (size - 1)) + 1;      // here we find the step to cope with collisions
}

void insert(HashTable **hash_table, int key);

HashTable* create_hashtable(int size) {
    HashTable* hash_table = (HashTable*) malloc(sizeof(HashTable));
    HashEntry* entries = (HashEntry*) malloc(size * sizeof(HashEntry));
    hash_table->size = size;
    hash_table->count = 0;
    for (int i = 0; i < size; i++) {
        entries[i].key = 0;
        entries[i].is_deleted = 1;
    }
    hash_table->table = entries;
    return hash_table;
}

HashTable* resize(HashTable *previous) {                // if our hash table has a lot of elements it will become bigger to
    int prev_size = previous->size;                     // prevent  many colisions
    int new_size = prev_size * 2;

    HashTable* new_table = create_hashtable(new_size);
    new_table->size = new_size;

    for (int i = 0; i < prev_size; i++) {               // writing back elements which were in initial hash table
        if (previous->table[i].is_deleted == 0) {
            insert(&new_table, previous->table[i].key);
        }
    }

    free(previous->table);
    return new_table;
}

void insert(HashTable **hash_table, int key) {
    if ((float)(*hash_table)->count / (*hash_table)->size > 0.7) {          // if our hash table is 70% taken than we increase the size
        *hash_table = resize(*hash_table);
    }

    int index = hash1(key, (*hash_table)->size);
    int step = hash2(key, (*hash_table)->size);

    while ((*hash_table)->table[index].is_deleted == 0) {           // while the place is taken we will go further
        if ((*hash_table)->table[index].key == key) {               // if there was an exact key than we return
            return;
        }
        index = (index + step) % (*hash_table)->size;               // finding new index
    }

    (*hash_table)->table[index].key = key;
    (*hash_table)->table[index].is_deleted = 0;
    (*hash_table)->count++;
}

void delete(HashTable *hash_table, int key) {
    int index = hash1(key, hash_table->size);
    int step = hash2(key, hash_table->size);

    while (hash_table->table[index].is_deleted == 0) {
        if (hash_table->table[index].key == key) {          // if we found the element we will mark it as deleted
            hash_table->table[index].is_deleted = 1;
            hash_table->count--;
            return;
        }

        index = (index + step) % hash_table->size;
    }
}

int search(HashTable* ht, int key) {
    int index = hash1(key, ht->size); 
    int step = hash2(key, ht->size);   

    while (ht->table[index].is_deleted == 0) {  
        if (ht->table[index].key == key) {
            return 1;               // was found
        }
        index = (index + step) % ht->size;  
    }

    return 0;  // wasn't found
}

void clear(HashTable *hash_table) {
    for (int i = 0; i < hash_table->size; i++) {
        hash_table->table[i].key = 0;
        hash_table->table[i].is_deleted = 1;
    }
    hash_table->count = 0;
}

