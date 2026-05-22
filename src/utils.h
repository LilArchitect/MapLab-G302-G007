#ifndef UTILS_H
#define UTILS_H

#define EARTH_RADIUS 6371.0

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

#endif