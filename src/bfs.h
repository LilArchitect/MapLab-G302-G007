#ifndef BFS_H
#define BFS_H
#include "structures.h" 

Queue *create_queue();
Queue *enqueue(Queue *queue, Path_node *path);
Path_node *dequeue(Queue **queue);
int queue_is_empty(Queue *queue);
void free_queue(Queue *queue);
Visited_node *add_visited(Visited_node *visited, Street *street);
int is_visited(Visited_node *visited, Street *street);
void free_visited(Visited_node *visited);
Path_node *add_to_path(Path_node *path, Street *street);
void free_path(Path_node *path);
Path_node *BFS(IntersectionMap *map, Street *origin_head, Street *dest_head);
int turn_direction(Street *ab, Street *bc);
void print_path(Path_node *node);
#endif