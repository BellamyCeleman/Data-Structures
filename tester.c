#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "WBT.c"
#include "red-black.c"
#include "hash_table.c"

#define DATA_SIZE 1000000

int main() {
    srand(time(NULL));
    clock_t start, end;
    double time_taken;

    int *randomData = malloc(sizeof(int) * DATA_SIZE);
    for (int i = 0; i < DATA_SIZE; i++) {
        randomData[i] = rand();
    }

    // ====== Weight Balanced Tree Test ======
    printf("===== Weight Balanced Tree Test =====\n");
    NodeWBT *rootWBT = NULL;

    // Insert into WBT
    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        insertWBT(&rootWBT, createNodeWBT(i));
    }
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("WBT insert time: %.6f seconds\n", time_taken);

    // Search in WBT
    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        searchWBT(rootWBT, i);
    }
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("WBT search time: %.6f seconds\n", time_taken);

    // Delete from WBT
    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        deleteWBT(&rootWBT, i);
    }
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("WBT delete time: %.6f seconds\n", time_taken);

    printf("WBT cleared\n");

    // === WBT with random values ===
    rootWBT = NULL;
    printf("--- WBT Random Test ---\n");

    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        insertWBT(&rootWBT, createNodeWBT(randomData[i]));
    }
    end = clock();
    printf("WBT random insert: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        searchWBT(rootWBT, randomData[i]);
    }
    end = clock();
    printf("WBT random search: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        deleteWBT(&rootWBT, randomData[i]);
    }
    end = clock();
    printf("WBT random delete: %.6f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);

    freeTreeWBT(&rootWBT);

    // ====== Red-Black Tree Test ======
    printf("===== Red-Black Tree Test =====\n");
    NodeRBT *rootRBT = NULL;

    // Insert into RBT
    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        insertRBT(&rootRBT, new_NodeRBT(i));
    }
    end = clock();
    printf("RBT insert time: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Search in RBT
    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        searchRBT(rootRBT, i);
    }
    end = clock();
    printf("RBT search time: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Delete from RBT
    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        deleteRBT(&rootRBT, new_NodeRBT(i));
    }
    end = clock();
    printf("RBT delete time: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    printf("RBT cleared\n");

    // === RBT with random values ===
    rootRBT = NULL;
    printf("--- RBT Random Test ---\n");

    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        insertRBT(&rootRBT, new_NodeRBT(randomData[i]));
    }
    end = clock();
    printf("RBT random insert: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        searchRBT(rootRBT, randomData[i]);
    }
    end = clock();
    printf("RBT random search: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        deleteRBT(&rootRBT, new_NodeRBT(randomData[i]));
    }
    end = clock();
    printf("RBT random delete: %.6f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);

    clearRBTree(&rootRBT);

    // ====== Hash Table Test ======
    printf("===== Hash Table Test =====\n");
    HashTable *ht = create_hashtable(DATA_SIZE / 10);

    // Insert into HT
    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        insert(&ht, i);
    }
    end = clock();
    printf("HT insert time: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Search in HT
    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        search(ht, i);
    }
    end = clock();
    printf("HT search time: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Delete from HT
    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        delete(ht, i);
    }
    end = clock();
    printf("HT delete time: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    printf("HT cleared\n");

    // === HT with random values ===
    ht = create_hashtable(2 * DATA_SIZE);
    printf("--- HT Random Test ---\n");

    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        insert(&ht, randomData[i]);
    }
    end = clock();
    printf("HT random insert: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        search(ht, randomData[i]);
    }
    end = clock();
    printf("HT random search: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        delete(ht, randomData[i]);
    }
    end = clock();
    printf("HT random delete: %.6f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    clear(ht);
    free(randomData);

    return 0;
}
