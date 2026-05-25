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
    Street *current = head;

    printf("From this street segment, you can go to:\n");
    printf("- %s\n", closest->name);
    printf("    Which is connected to:\n");

    while (current != NULL) {
        // Comparten nodo si alguno de los 4 pares coincide
        if (current != closest &&
           (current->node1_id == closest->node1_id ||
            current->node1_id == closest->node2_id ||
            current->node2_id == closest->node1_id ||
            current->node2_id == closest->node2_id)) {

            printf("     - %s\n", current->name);
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