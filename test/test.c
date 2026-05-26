#include "utils.h"
#include "test_houses.h"
#include "test_houses.c"
#include "test_places.h"
#include "test_places.c"
#include "test_streets.h"
#include "test_streets.c"
#include "test_hashmap.h"
#include "test_hashmap.c"
#include <stdio.h>

int main()
{
  test_houses();
  test_places();
  test_streets();
  hashmap_test();
  allsuccess();
  return 0;
}