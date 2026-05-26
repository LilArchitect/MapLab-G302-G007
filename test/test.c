#include "utils.h"
#include "test_houses.h"
#include "test_houses.c"
#include "test_places.h"
#include "test_places.c"
#include "test_streets.h"
#include "test_streets.c"
#include <stdio.h>

int main()
{
  test_houses();
  test_places();
  test_streets();
  allsuccess();
  return 0;
}