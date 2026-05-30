#ifndef UTILS_H
#define UTILS_H

#include "structures.h"

/** @brief Earth's radius in kilometers, used by haversine(). */
#define EARTH_RADIUS 6371.0

/**
 * @brief Strips leading spaces from a string in-place.
 * @param s Null-terminated string to trim.
 */
void ltrim(char *s);

/**
 * @brief Detects and removes a street type prefix from a normalized name.
 *
 * Recognizes prefixes and modifies the string by removing the detected prefix.
 *
 * @param s Null-terminated, already-lowercased street name.
 * @return The detected StreetType, or TYPE_UNKNOWN if no prefix matched.
 * @see normalize()
 */
StreetType get_type_and_strip_prefix(char *s);

/**
 * @brief Prompts the user for a string input with a size limit.
 *
 * Allocates a buffer of the given size, prints the message, and reads
 * from stdin.
 *
 * @param size Maximum number of characters to read including null terminator.
 * @param msg  Prompt message displayed to the user.
 * @return Heap-allocated string, or NULL on allocation or read failure.
 */
char *get_string(int size, const char *msg);

/**
 * @brief Sorts an integer array using bubble sort.
 *
 * @param arr  Array to sort.
 * @param size Number of elements in the array.
 */
void bubble_sort(int arr[], int n);

/**
 * @brief Converts a string to lowercase in-place.
 * @param str Null-terminated string to normalize.
 */
void normalize(char *str);

/**
 * @brief Computes the Levenshtein distance between two strings.
 *
 * @param a First null-terminated string.
 * @param b Second null-terminated string.
 * @return Minimum number of single-character edits to transform @p a into @p b.
 */
int levenshtein(const char *a, const char *b);

/**
 * @brief Represents a geographic coordinate (latitude/longitude pair).
 */
typedef struct position
{
    double lat;
    double lon;
} Position;

/**
 * @brief Converts decimal degrees to radians.
 * @param degree Angle in decimal degrees.
 * @return Equivalent angle in radians.
 */
double toRadians(double degree);

/**
 * @brief Converts radians to decimal degrees.
 * @param radians Angle in radians.
 * @return Equivalent angle in decimal degrees.
 */
double toDegrees(double radians);

/**
 * @brief Computes the great-circle distance between two positions.
 *
 * Uses the Haversine formula to calculate the shortest distance
 * over the Earth's surface.
 *
 * @param posA First geographic position.
 * @param posB Second geographic position.
 * @return Distance in kilometers.
 */
double haversine(Position posA, Position posB);

/**
 * @brief Computes the geographic midpoint between two positions.
 *
 * Converts to 3D Cartesian coordinates, averages them, and converts back
 * to latitude/longitude.
 *
 * @param a First geographic position.
 * @param b Second geographic position.
 * @return Position representing the midpoint.
 */
Position midpoint(Position a, Position b);

/**
 * @brief Projects geographic coordinates in meters.
 *
 * Uses an equirectangular projection centered on the reference point.
 *
 * @param lat_ref Reference latitude in decimal degrees (origin of the plane).
 * @param lon_ref Reference longitude in decimal degrees (origin of the plane).
 * @param lat     Target latitude in decimal degrees.
 * @param lon     Target longitude in decimal degrees.
 * @param x       Output: east-west displacement in meters.
 * @param y       Output: north-south displacement in meters.
 */
void latlon_to_xy(double lat_ref, double lon_ref,
                  double lat, double lon,
                  double *x, double *y);

/**
 * @brief Replaces UTF-8 en-dashes (U+2013) with ASCII hyphens in-place.
 *
 * Place names sometimes contain en-dashes (0xE2 0x80 0x93).
 * This function normalizes them to '-' for consistent string comparison.
 *
 * @param s Null-terminated UTF-8 string to process.
 */
void replace_em_dash(char *s);

/**
 * @brief Finds the street segment closest to the given coordinates.
 *
 * Computes the haversine distance from the user's position to the midpoint
 * of each street segment and returns the nearest one.
 *
 * @param head     Head of the Street linked list.
 * @param user_lat User's latitude in decimal degrees.
 * @param user_lon User's longitude in decimal degrees.
 * @return Pointer to the closest Street segment, or NULL if the list is empty.
 * 
 */
Street *find_closest_street(Street *head, double user_lat, double user_lon);

/**
 * @brief Wrapper around find_closest_street() for coordinate-based lookup.
 *
 * @param street_head Head of the Street linked list.
 * @param lat         Latitude in decimal degrees.
 * @param lon         Longitude in decimal degrees.
 * @return Pointer to the closest Street segment, or NULL if the list is empty.
 * @see find_closest_street()
 */
Street *coordinates(Street *street_head, double lat, double lon);

#endif