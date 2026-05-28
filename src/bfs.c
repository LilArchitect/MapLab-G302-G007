#include <stdio.h>
#include <stdlib.h>
#include "bfs.h"
#include "hashmap.h"
#include <string.h>
#include "utils.h"


Queue *create_queue()
{
    return NULL;
}

Queue *enqueue(Queue *queue, Path_node *path)
{
    Queue *temp = malloc(sizeof(Queue));

    if (temp == NULL)
        return queue;

    temp->path = path;
    temp->next = NULL;

    if (queue == NULL)
        return temp;
    Queue *aux = queue;

    while (aux->next != NULL)
    {
        aux = aux->next;
    }

    aux->next = temp;
    return queue;
}

Path_node *dequeue(Queue **queue)
{
    if (queue == NULL)
        return NULL;

    Queue *front = *queue;
    Path_node *path = front->path;
    *queue = front->next;
    free(front);

    return path;
}

int queue_is_empty(Queue *queue)
{
    return queue == NULL;
}

void free_queue(Queue *queue)
{
    while (!queue_is_empty(queue))
    {
        Path_node *path = dequeue(&queue);
        free(path);
    }

    free(queue);
}

Visited_node *add_visited(Visited_node *visited, Street *street)
{
    Visited_node *node = malloc(sizeof(Visited_node));
    node->street = street;
    node->next = visited;
    return node;
}

int is_visited(Visited_node *visited, Street *street)
{
    while (visited != NULL)
    {
        if (visited->street->node1_id == street->node1_id &&
            visited->street->node2_id == street->node2_id)
            return 1;
        visited = visited->next;
    }
    return 0;
}

void free_visited(Visited_node *visited)
{
    while (visited != NULL)
    {
        Visited_node *temp = visited;
        visited = visited->next;
        free(temp);
    }
}

Path_node *add_to_path(Path_node *path, Street *street)
{
    Path_node *new_path = NULL;
    Path_node *tail = NULL;
    Path_node *current = path;

    while (current != NULL)
    {
        Path_node *node = malloc(sizeof(Path_node));
        node->street = current->street;
        node->next = NULL;
        if (new_path == NULL)
        {
            new_path = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
        current = current->next;
    }

    Path_node *node = malloc(sizeof(Path_node));
    node->street = street;
    node->next = NULL;
    if (new_path == NULL)
        new_path = node;
    else
        tail->next = node;
    return new_path;
}

void free_path(Path_node *path) {
    while (path != NULL) {
        Path_node *temp = path;
        path = path->next;
        free(temp);
    }
}

Path_node *BFS(IntersectionMap *map, Street *origin_head, Street *dest_head)
{

    Queue *Q = create_queue();
    Path_node *initial_path = malloc(sizeof(Path_node));

    if (initial_path == NULL)
        return NULL;

    initial_path->street = origin_head;
    initial_path->next = NULL;
    Q = enqueue(Q, initial_path);

    Visited_hash *visited = visited_hashmap_create();

    while (!queue_is_empty(Q))
    {
        Path_node *path = dequeue(&Q);
        Path_node *temp = path;

        while (temp->next != NULL)
            temp = temp->next;

        Street *current_street = temp->street;

        if (current_street->node1_id == dest_head->node1_id ||
            current_street->node2_id == dest_head->node1_id ||
            current_street->node1_id == dest_head->node2_id ||
            current_street->node2_id == dest_head->node2_id)
        {
            free_visited_hash(visited);
            free_queue(Q);
            return path;
        }

        if (!is_visited_hash(visited, current_street))
        {
            visited_hashmap_insert(visited, current_street);
            StreetNode *sn = hashmap_get(map, current_street->node2_id);
            while (sn != NULL)
            {
                if (!is_visited_hash(visited, sn->street))
                {
                    Path_node *new_path = add_to_path(path, sn->street);
                    Q = enqueue(Q, new_path);
                }
                sn = sn->next;
            }
        }
        free_path(path);
    }
    return NULL;
}

int turn_direction(Street *ab, Street *bc) {
    double ax, ay, bx, by, cx, cy;
    double lat_ref = ab->lat1;
    double lon_ref = ab->lon1;

    latlon_to_xy(lat_ref, lon_ref, ab->lat1, ab->lon1, &ax, &ay);
    latlon_to_xy(lat_ref, lon_ref, ab->lat2, ab->lon2, &bx, &by);
    latlon_to_xy(lat_ref, lon_ref, bc->lat2, bc->lon2, &cx, &cy);

    double cross = (bx - ax) * (cy - by) - (by - ay) * (cx - bx);

    if (cross > 0)  return 1;
    if (cross < 0)  return -1;
    return 0;
}

void print_path(Path_node *node) {
    printf("\n--- ROUTE ---\n");

    if (node == NULL) {
        printf("  No route found.\n");
        return;
    }

    printf("  Start at %s\n", node->street->name);

    Path_node *prev = node;
    Path_node *curr = node->next;

    while (curr != NULL) {
        int meters = (int)curr->street->speed;

        // Acumular segmentos consecutivos de la misma calle
        while (curr->next != NULL &&
               strcmp(curr->next->street->name, curr->street->name) == 0) {
            curr = curr->next;
            meters += (int)curr->street->speed;
        }

        if (curr->next == NULL) {
            printf("    You have arrived to %s\n", curr->street->name);
        } else {
            int dir = turn_direction(prev->street, curr->street);
            //printf("DEBUG before print direction: dir = %d", dir);
            if(dir == 1) printf("  Turn left to %s and continue for %dm\n", curr->street->name, meters);
            if(dir == 0) printf("  Continue straight to %s and continue for %dm\n", curr->street->name, meters);
            if(dir == -1) printf("  Turn right to %s and continue for %dm\n", curr->street->name, meters);
            prev = curr;
        }

        curr = curr->next;
    }
}

