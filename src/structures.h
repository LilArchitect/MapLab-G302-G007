#ifndef STRUCTURES_H
#define STRUCTURES_H
#define SIZE 256
#define HASHMAP_SIZE 1024



// House Struct
typedef struct House{

  char street[SIZE];
  int number;
  double latitude;
  double longitude;
  struct House *next;
} House;



// Place Struct
typedef struct Place {
  char id[SIZE];
  char name[SIZE];
  char type[SIZE];
  double latitude;
  double longitude;
  struct Place *next;
} Place;

// Types of addresses
typedef enum {
    TYPE_UNKNOWN = 0, // If it doesn't finds type of address it doesn't do anything
    TYPE_STREET,
    TYPE_AVENUE,
    TYPE_PASSAGE
} StreetType;

typedef struct Street {
    long long node1_id;
    double lat1, lon1;
    long long node2_id;
    double lat2, lon2;
    double speed;
    char name[SIZE];
    struct Street *next;
} Street;

typedef struct StreetNode {
    Street *street;
    struct StreetNode *next;
} StreetNode;

typedef struct HashEntry {
    long long node_id;
    StreetNode *streets;
    struct HashEntry *next;
} HashEntry;

typedef struct IntersectionMap {
    HashEntry *buckets[HASHMAP_SIZE];
} IntersectionMap;

typedef struct Path_node{
    struct Street *street;
    struct Path_node *next;
}Path_node;

typedef struct Queue{
    struct Path_node *path;
    struct Queue *next;
}Queue;

typedef struct Visited_node{
    struct Street *street;
    struct Visited_node *next;
}Visited_node;

typedef struct Visited_entry{
    long long node1_id;
    long long node2_id;
    struct Visited_entry *next;
}Visited_entry;

typedef struct Visited_hash{
    Visited_entry *buckets[HASHMAP_SIZE];
}Visited_hash;

#endif