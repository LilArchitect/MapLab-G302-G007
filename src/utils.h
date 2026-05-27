#ifndef UTILS_H
#define UTILS_H

#include "structures.h" 
#define EARTH_RADIUS 6371.0

void ltrim(char *s);
StreetType get_type_and_strip_prefix(char *s);

char* get_string(int size, const char *msg);
void bubble_sort(int arr[], int n);
void normalize(char *str);
int levenshtein(const char *a, const char *b);

typedef struct position {
    double lat;
    double lon;
} Position;

double toRadians(double degree);
double toDegrees(double radians);
double haversine(Position posA, Position posB);
Position midpoint(Position a, Position b);
void replace_em_dash(char *s);
Street *find_closest_street(Street *head, double user_lat, double user_lon);
Street *coordinates(Street *street_head, double lat, double lon);
Queue *create_queue();
Queue *enqueue(Queue *queue, Path_node *path);
Path_node *dequeue(Queue **queue);
int queue_is_empty(Queue *queue);
void free_queue(Queue *queue);
Visited_node *add_visited(Visited_node *visited, Street *street);
int is_visited(Visited_node *visited, Street *street);
void free_visited(Visited_node *visited);
Path_node *add_to_path(Path_node *path, Street *street);
Path_node *BFS(IntersectionMap *map, Street *origin_head, Street *dest_head);
#endif