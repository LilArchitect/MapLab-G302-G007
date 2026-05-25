#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

static int hash(long long node_id) {
    unsigned long long v = (unsigned long long)node_id;
    v = (v ^ (v >> 30)) * 0xbf58476d1ce4e5b9ULL;
    v = (v ^ (v >> 27)) * 0x94d049bb133111ebULL;
    v = v ^ (v >> 31);
    return (int)(v % HASHMAP_SIZE);
}

IntersectionMap *hashmap_create(void) {
    IntersectionMap *map = calloc(1, sizeof(IntersectionMap));
    return map;
}

void hashmap_insert(IntersectionMap *map, long long node_id, Street *street) {
    int idx = hash(node_id);

    HashEntry *entry = map->buckets[idx];
    while (entry != NULL) {
        if (entry->node_id == node_id) {

            StreetNode *sn = malloc(sizeof(StreetNode));
            sn->street = street;
            sn->next = entry->streets;
            entry->streets = sn;
            return;
        }
        entry = entry->next;
    }
    HashEntry *new_entry = malloc(sizeof(HashEntry));
    new_entry->node_id = node_id;
    new_entry->next = map->buckets[idx];

    StreetNode *sn = malloc(sizeof(StreetNode));
    sn->street = street;
    sn->next = NULL;
    new_entry->streets = sn;

    map->buckets[idx] = new_entry;
}

StreetNode *hashmap_get(IntersectionMap *map, long long node_id) {
    int idx = hash(node_id);
    HashEntry *entry = map->buckets[idx];
    while (entry != NULL) {
        if (entry->node_id == node_id)
            return entry->streets;
        entry = entry->next;
    }
    return NULL;
}

IntersectionMap *build_intersection_map(Street *head) {
    IntersectionMap *map = hashmap_create();
    Street *current = head;
    while (current != NULL) {
        hashmap_insert(map, current->node1_id, current);
        hashmap_insert(map, current->node2_id, current);
        current = current->next;
    }
    return map;
}

void find_connected_streets_fast(IntersectionMap *map, Street *street) {
    printf("From this street segment, you can go to:\n");
    printf("- %s\n", street->name);
    printf("    Which is connected to:\n");

    long long nodes[2] = {street->node1_id, street->node2_id};
    for (int i = 0; i < 2; i++) {
        StreetNode *sn = hashmap_get(map, nodes[i]);
        while (sn != NULL) {
            if (sn->street != street) 
                printf("     - %s\n", sn->street->name);
            sn = sn->next;
        }
    }
}

void hashmap_free(IntersectionMap *map) {
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        HashEntry *entry = map->buckets[i];
        while (entry != NULL) {
            StreetNode *sn = entry->streets;
            while (sn != NULL) {
                StreetNode *tmp = sn;
                sn = sn->next;
                free(tmp);
            }
            HashEntry *tmp = entry;
            entry = entry->next;
            free(tmp);
        }
    }
    free(map);
}