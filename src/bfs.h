#ifndef BFS_H
#define BFS_H
#include "structures.h"

/**
 * @brief Creates an empty queue.
 * @return NULL (empty queue sentinel).
 */
Queue *create_queue();

/**
 * @brief Appends a path to the back of the queue.
 *
 * @param queue Head of the queue.
 * @param path  Path node to enqueue.
 * @return Updated head of the queue.
 */
Queue *enqueue(Queue *queue, Path_node *path);

/**
 * @brief Removes and returns the path at the front of the queue.
 *
 * @param queue Pointer to the head of the queue.
 * @return The dequeued Path_node, or NULL if the queue is empty.
 */
Path_node *dequeue(Queue **queue);

/**
 * @brief Checks whether the queue is empty.
 *
 * @param queue Head of the queue.
 * @return 1 if empty, 0 otherwise.
 */
int queue_is_empty(Queue *queue);

/**
 * @brief Frees all queue nodes and their associated paths.
 * @param queue Head of the queue to free.
 */
void free_queue(Queue *queue);

/**
 * @brief Adds a street to the visited linked list.
 *
 * @deprecated Use visited_hashmap_insert()
 * @param visited Head of the visited list.
 * @param street  Street to mark as visited.
 * @return New head of the visited list.
 * @see is_visited()
 */
Visited_node *add_visited(Visited_node *visited, Street *street);

/**
 * @brief Checks whether a street has been visited.
 *
 * Compares node1_id and node2_id to identify a segment uniquely.
 *
 * @deprecated Use is_visited_hash().
 * @param visited Head of the visited list.
 * @param street  Street to look up.
 * @return 1 if visited, 0 otherwise.
 * @see add_visited()
 */
int is_visited(Visited_node *visited, Street *street);

/**
 * @brief Frees all nodes in a visited linked list.
 * @deprecated Use free_visited_hash() for the hashmap version.
 * @param visited Head of the visited list to free.
 */
void free_visited(Visited_node *visited);

/**
 * @brief Creates a new path by copying an existing one and appending a street.
 *
 * Allocates a full deep copy of path and appends a new node for street.
 * The original path is left unmodified.
 *
 * @param path   Existing path to copy.
 * @param street Street segment to append.
 * @return Head of the new path, or NULL on allocation failure.
 */
Path_node *add_to_path(Path_node *path, Street *street);

/**
 * @brief Frees all nodes in a path linked list.
 * @param path Head of the path to free.
 */
void free_path(Path_node *path);

/**
 * @brief Finds the shortest path between two streets using BFS and a hashmap.
 *
 * Uses the IntersectionMap neighbor lookup and Visited_hash
 * for visited checks. Expands neighbors via node2_id of each segment.
 *
 * @param map         Pre-built intersection hashmap.
 * @param origin_head Origin street segment.
 * @param dest_head   Destination street segment.
 * @return Head of the shortest Path_node list, or NULL if no route exists.
 * @see BFS_linear()
 */
Path_node *BFS(IntersectionMap *map, Street *origin_head, Street *dest_head);

/**
 * @brief Computes the turn direction from street AB to street BC.
 *
 * @param ab Incoming street segment (A → B).
 * @param bc Outgoing street segment (B → C).
 * @return  1 for left turn, -1 for right turn, 0 for straight.
 */
int turn_direction(Street *ab, Street *bc);

/**
 * @brief Prints a readable route from a BFS path.
 *
 * Merges consecutive segments of the same street into one instruction,
 * summing their distances. Indicates turn direction at each junction.
 *
 * @param node Head of the Path_node list returned by BFS().
 * @see turn_direction()
 */
void print_path(Path_node *node);

/**
 * @brief Finds the path between two streets using BFS and a linear visited list.
 *
 * @param streets Full Street linked list.
 * @param origin  Origin street segment.
 * @param dest    Destination street segment.
 * @return Head of the shortest Path_node list, or NULL if no route exists.
 * @note The caller is responsible for freeing the result with free_path().
 * @see BFS()
 */
Path_node *BFS_linear(Street *streets, Street *origin, Street *dest);
#endif