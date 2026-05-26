#include "utils.h"
#include "test_houses.h"
#include "test_houses.c"
#include "test_places.h"
#include "test_places.c"
#include <stdio.h>

int main() {
  test_houses();
  test_places();
  allsuccess();
  return 0;
}