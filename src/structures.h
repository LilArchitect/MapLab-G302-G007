#ifndef STRUCTURES_H
#define STRUCTURES_H
#define SIZE 128


// House Struct
typedef struct House{

  char street[SIZE];
  int number;
  double latitude;
  double longitude;
  struct House *next;
} House;



// Place Struct
typedef struct Place {
  char id[SIZE];
  char name[SIZE];
  char type[SIZE];
  double latitude;
  double longitude;
  struct Place *next;
} Place;

// Types of addresses
typedef enum {
    TYPE_UNKNOWN = 0, // If it doesn't finds type of address it doesn't do anything
    TYPE_STREET,
    TYPE_AVENUE,
    TYPE_PASSAGE
} StreetType;

typedef struct Street {
    long long node1_id;
    double lat1, lon1;
    long long node2_id;
    double lat2, lon2;
    int speed;
    char name[SIZE];
    struct Street *next;
} Street;

#endif