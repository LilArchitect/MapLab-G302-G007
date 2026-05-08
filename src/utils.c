#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


// Asks the user for a string
char* get_string(int size, const char *msg) {
    char *str = malloc(size * sizeof(char));
    if (str == NULL) return NULL;

    char fmt[20];
    snprintf(fmt, sizeof(fmt), "%%%ds", size - 1);

    printf("%s", msg);

    if (scanf(fmt, str) != 1) {
        free(str);
        return NULL;
    }

    return str;
}

void bubble_sort(int arr[], int size) {
    int i, j, temp;

    for (i = 0; i < size - 1; i++) {

        for (j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void normalize(char *str) {

    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}