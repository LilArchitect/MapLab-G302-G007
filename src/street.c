#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "utils.h"
#include "street.h"

#define SIZE 128

Street* load_streets(char* mapName) {
    char filename[SIZE];
    sprintf(filename, "maps/%s/streets.txt", mapName);

    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    Street *head = NULL;
    Street temp;
    int skipped = 0;

    char line [512];

    while (fgets(line, sizeof(line), file)) {
        if(fscanf(file, "%lld,%lf,%lf,%lld,%lf,%lf,%lf,%127[^\n]",
                  &temp.node1_id, &temp.lat1, &temp.lon1,
                  &temp.node2_id, &temp.lat2, &temp.lon2,
                  &temp.speed, temp.name) == 8) {

            Street *new = malloc(sizeof(Street));
            *new = temp;
            new->next = head;
            head = new;
        } else {
            skipped++;
        }
    }
    printf("DEBUG streets: skipped %d malformed lines\n", skipped);
    fclose(file);
    return head;
}

void free_streets(Street *head) {
    //printf("DEBUG: Inside free_streets\n");

    Street *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void find_connected_streets(Street *head, Street *closest) {
    printf("    From this street segment, you can go to:\n");
    printf("    - %s\n", closest->name);
    printf("        Which is connected to:\n");

    char seen[64][SIZE];
    int seen_count = 0;

    // Primero recogemos todos los nodos de los segmentos de la misma calle
    // que toquen con el segmento más cercano
    long long nodes[128];
    int node_count = 0;
    nodes[node_count++] = closest->node1_id;
    nodes[node_count++] = closest->node2_id;

    // Expandir: añadir nodos de segmentos contiguos de la misma calle
    Street *seg = head;
    while (seg != NULL) {
        if (strcmp(seg->name, closest->name) == 0) {
            for (int i = 0; i < node_count; i++) {
                if (seg->node1_id == nodes[i] || seg->node2_id == nodes[i]) {
                    // Añadir sus nodos si no están ya
                    int found1 = 0, found2 = 0;
                    for (int j = 0; j < node_count; j++) {
                        if (nodes[j] == seg->node1_id) found1 = 1;
                        if (nodes[j] == seg->node2_id) found2 = 1;
                    }
                    if (!found1 && node_count < 128) nodes[node_count++] = seg->node1_id;
                    if (!found2 && node_count < 128) nodes[node_count++] = seg->node2_id;
                    break;
                }
            }
        }
        seg = seg->next;
    }

    // Buscar calles conectadas a cualquiera de esos nodos
    Street *current = head;
    while (current != NULL) {
        if (strcmp(current->name, closest->name) != 0) {
            for (int i = 0; i < node_count; i++) {
                if (current->node1_id == nodes[i] || current->node2_id == nodes[i]) {
                    int already = 0;
                    for (int j = 0; j < seen_count; j++)
                        if (strcmp(seen[j], current->name) == 0) { already = 1; break; }
                    if (!already && seen_count < 64) {
                        printf("         - %s\n", current->name);
                        strcpy(seen[seen_count++], current->name);
                    }
                    break;
                }
            }
        }
        current = current->next;
    }
}

void street_navigation(char *mapName) {
    Street *streets = load_streets(mapName);
    if (streets == NULL) {
        printf("Error loading streets.\n");
        return;
    }

    double lat, lon;
    printf("Enter your coordinates:\n");
    printf("Latitude: ");
    scanf("%lf", &lat);
    printf("Longitude: ");
    scanf("%lf", &lon);

    Street *closest = find_closest_street(streets, lat, lon);
    if (closest == NULL) {
        printf("No street found.\n");
        free_streets(streets);
        return;
    }

    printf("Closest street: %s\n", closest->name);
    printf("Between %lld (%.6f, %.6f) and %lld (%.6f, %.6f)\n",
           closest->node1_id, closest->lat1, closest->lon1,
           closest->node2_id, closest->lat2, closest->lon2);

    find_connected_streets(streets, closest);

    free_streets(streets);
}