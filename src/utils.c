#include <stdio.h>
#include <stdlib.h>


// Asks the user for a string
char* getString(int size, const char *msg) {
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