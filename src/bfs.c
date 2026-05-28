#include <stdio.h>
#include <stdlib.h>
#include "bfs.h"
#include "hashmap.h"
#include <string.h>


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
        Path_node *tmp = path;
        path = path->next;
        free(tmp);
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

    Visited_node *visited = NULL;

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
            free_visited(visited);
            free_queue(Q);
            return path;
        }

        if (!is_visited(visited, current_street))
        {
            visited = add_visited(visited, current_street);

            long long nodes[2] = {current_street->node1_id, current_street->node2_id};
            for (int i = 0; i < 2; i++)
            {
                StreetNode *sn = hashmap_get(map, nodes[i]);
                while (sn != NULL)
                {
                    if (!is_visited(visited, sn->street))
                    {
                        Path_node *new_path = add_to_path(path, sn->street);
                        Q = enqueue(Q, new_path);
                    }
                    sn = sn->next;
                }
            }
        }
        free_path(path);
    }
    return NULL;
}