#ifndef STRUCTURES_H
#define STRUCTURES_H
#define SIZE 128


// House Struct
typedef struct House{

  char street[SIZE];
  int number;
  float latitude;
  float longitude;
  struct House *next;

} House;

// Place Struct
typedef struct Place {
  char id[SIZE];
  char name[SIZE];
  char type[SIZE];
  float latitude;
  float longitude;
  struct Place *next;
} Place;

// Types of addresses
typedef enum {
    TYPE_UNKNOWN = 0, // If it doesn't finds type of address it doesn't do anything
    TYPE_STREET,
    TYPE_AVENUE,
    TYPE_PASSAGE
} StreetType;

#endif