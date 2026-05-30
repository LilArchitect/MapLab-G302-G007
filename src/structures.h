#ifndef STRUCTURES_H
#define STRUCTURES_H

/** @brief Maximum buffer size for strings. */
#define SIZE 256

/** @brief Number of buckets in the intersection hashmap. */
#define HASHMAP_SIZE 1024

/**
 * @brief Represents a house address with its geolocation.
 */
typedef struct House
{

    char street[SIZE];
    int number;
    double latitude;
    double longitude;
    struct House *next;
} House;

/**
 * @brief Represents a point of interest (place) on the map.
 */
typedef struct Place
{
    char id[SIZE];
    char name[SIZE];
    char type[SIZE];
    double latitude;
    double longitude;
    struct Place *next;
} Place;

/**
 * @brief Street type prefix detected during name normalization.
 */
typedef enum
{
    TYPE_UNKNOWN = 0,
    TYPE_STREET,     
    TYPE_AVENUE,     
    TYPE_PASSAGE      
} StreetType;

/**
 * @brief Represents a directed street segment between two OSM nodes.
 */
typedef struct Street
{
    long long node1_id;
    double lat1, lon1;
    long long node2_id;
    double lat2, lon2;
    double speed;
    char name[SIZE];
    struct Street *next;
} Street;

/**
 * @brief Node in a list of streets associated with an intersection.
 */
typedef struct StreetNode
{
    Street *street;
    struct StreetNode *next;
} StreetNode;

/**
 * @brief A single bucket entry in the IntersectionMap hashmap.
 */
typedef struct HashEntry
{
    long long node_id;
    StreetNode *streets;
    struct HashEntry *next;
} HashEntry;

/**
 * @brief Hashmap of intersections indexed by node ID.
 */
typedef struct IntersectionMap
{
    HashEntry *buckets[HASHMAP_SIZE];
} IntersectionMap;

/**
 * @brief A node in the BFS path linked list.
 */
typedef struct Path_node
{
    struct Street *street;
    struct Path_node *next;
} Path_node;

/**
 * @brief A node in the BFS frontier queue.
 */
typedef struct Queue
{
    struct Path_node *path;
    struct Queue *next;
} Queue;

/**
 * @brief A node in the old unused visited list.
 */
typedef struct Visited_node
{
    struct Street *street;
    struct Visited_node *next;
} Visited_node;

/**
 * @brief A single entry in the Visited_hash hashmap.
 */
typedef struct Visited_entry
{
    long long node1_id;
    long long node2_id;
    struct Visited_entry *next;
} Visited_entry;

/**
 * @brief Hashmap of visited street segments for BFS traversal.
 */
typedef struct Visited_hash
{
    Visited_entry *buckets[HASHMAP_SIZE];
} Visited_hash;

#endif