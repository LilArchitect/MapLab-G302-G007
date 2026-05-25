#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "structures.h"
#include "utils.h"

// Asks the user for a string
char *get_string(int size, const char *msg)
{
    char *str = malloc(size * sizeof(char));
    if (str == NULL)
        return NULL;

    char fmt[20];
    snprintf(fmt, sizeof(fmt), "%%%ds", size - 1);

    printf("%s", msg);

    if (scanf(fmt, str) != 1)
    {
        free(str);
        return NULL;
    }

    return str;
}

void bubble_sort(int arr[], int size)
{
    int i, j, temp;

    for (i = 0; i < size - 1; i++)
    {

        for (j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void normalize(char *str)
{

    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int levenshtein(const char *a, const char *b)
{
    int m = strlen(a);
    int n = strlen(b);
    int D[m + 1][n + 1];

    for (int i = 0; i <= m; i++)
        D[i][0] = i;
    for (int j = 0; j <= n; j++)
        D[0][j] = j;

    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            int del = D[i - 1][j] + 1;
            int ins = D[i][j - 1] + 1;
            int sub = D[i - 1][j - 1] + cost;
            D[i][j] = del < ins ? (del < sub ? del : sub)
                                : (ins < sub ? ins : sub);
        }
    }
    return D[m][n];
}

double toRadians(double degree)
{
    return degree * (M_PI / 180.0);
}

double toDegrees(double radians)
{
    return radians * (180.0 / M_PI);
}

double haversine(Position posA, Position posB)
{
    double lat1 = toRadians(posA.lat);
    double lon1 = toRadians(posA.lon);
    double lat2 = toRadians(posB.lat);
    double lon2 = toRadians(posB.lon);
    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;
    double a = pow(sin(dLat / 2), 2) +
               cos(lat1) * cos(lat2) * pow(sin(dLon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}

Position midpoint(Position a, Position b)
{
    double lat1 = toRadians(a.lat);
    double lon1 = toRadians(a.lon);
    double lat2 = toRadians(b.lat);
    double lon2 = toRadians(b.lon);
    double x1 = cos(lat1) * cos(lon1), y1 = cos(lat1) * sin(lon1), z1 = sin(lat1);
    double x2 = cos(lat2) * cos(lon2), y2 = cos(lat2) * sin(lon2), z2 = sin(lat2);
    double x = (x1 + x2) / 2.0, y = (y1 + y2) / 2.0, z = (z1 + z2) / 2.0;
    double lon = atan2(y, x);
    double hyp = sqrt(x * x + y * y);
    double lat = atan2(z, hyp);
    Position mid;
    mid.lat = toDegrees(lat);
    mid.lon = toDegrees(lon);
    return mid;
}

Street *find_closest_street(Street *head, double user_lat, double user_lon)
{
    Street *current = head;
    Street *closest = NULL;
    double min_dist = -1.0;

    Position user = {user_lat, user_lon};

    while (current != NULL)
    {
        Position a = {current->lat1, current->lon1};
        Position b = {current->lat2, current->lon2};


        Position mid = midpoint(a, b);
        double dist = haversine(user, mid);


        if (min_dist < 0 || dist < min_dist)
        {
            min_dist = dist;
            closest = current;
        }

        current = current->next;
    }

    return closest;
}

// Function to remove " "
void ltrim(char *s)
{
    while (*s == ' ')
    {
        memmove(s, s + 1, strlen(s));
    }
}

StreetType get_type_and_strip_prefix(char *s)
{
    ltrim(s);
    // Street
    if (strncmp(s, "carrer ", 7) == 0 ||
        strncmp(s, "calle ", 6) == 0 ||
        strncmp(s, "c/ ", 3) == 0 ||
        strncmp(s, "c. ", 3) == 0)
    {
        if (strncmp(s, "carrer ", 7) == 0)
        {
            memmove(s, s + 7, strlen(s + 7) + 1);
        }
        else if (strncmp(s, "calle ", 6) == 0)
        {
            memmove(s, s + 6, strlen(s + 6) + 1);
        }
        else
        {
            memmove(s, s + 3, strlen(s + 3) + 1);
        }
        ltrim(s);
        return TYPE_STREET;
    }
    // Avenue
    if (strncmp(s, "avinguda ", 9) == 0 ||
        strncmp(s, "avenida ", 8) == 0 ||
        strncmp(s, "av. ", 4) == 0 ||
        strncmp(s, "av ", 3) == 0)
    {
        if (strncmp(s, "avinguda ", 9) == 0)
        {
            memmove(s, s + 9, strlen(s + 9) + 1);
        }
        else if (strncmp(s, "avenida ", 8) == 0)
        {
            memmove(s, s + 8, strlen(s + 8) + 1);
        }
        else if (strncmp(s, "av. ", 4) == 0)
        {
            memmove(s, s + 4, strlen(s + 4) + 1);
        }
        else
        {
            memmove(s, s + 3, strlen(s + 3) + 1);
        }
        ltrim(s);
        return TYPE_AVENUE;
    }
    // Passage
    if (strncmp(s, "passatge ", 9) == 0 ||
        strncmp(s, "pasaje ", 7) == 0 ||
        strncmp(s, "psg. ", 5) == 0)
    {
        if (strncmp(s, "passatge ", 9) == 0)
        {
            memmove(s, s + 9, strlen(s + 9) + 1);
        }
        else if (strncmp(s, "pasaje ", 7) == 0)
        {
            memmove(s, s + 7, strlen(s + 7) + 1);
        }
        else
        {
            memmove(s, s + 5, strlen(s + 5) + 1);
        }
        ltrim(s);
        return TYPE_PASSAGE;
    }
    // Other cases
    return TYPE_UNKNOWN;
}

void replace_em_dash(char *s)
{
    char result[SIZE];
    int j = 0;
    for (int i = 0; s[i] != '\0';)
    {
        // Guion largo UTF-8: E2 80 93
        if ((unsigned char)s[i] == 0xE2 &&
            (unsigned char)s[i + 1] == 0x80 &&
            (unsigned char)s[i + 2] == 0x93)
        {
            result[j++] = '-';
            i += 3;
        }
        else
        {
            result[j++] = s[i++];
        }
    }
    result[j] = '\0';
    strcpy(s, result);
}

Street *coordinates(Street *street_head, double lat, double lon)
{
    if (street_head == NULL)
        printf("Warning: no streets loaded\n");
    Street *street;
    street = find_closest_street(street_head, lat, lon);
    return street;
}
