#include <stdio.h>
#include <stdlib.h>
#include "prefix_handle.h"
#include "tests.h"

int main() {

  testAdd();
  testCheck();
  testDelete();
  testCheck();

  free_prefix_list();
  printf("Prefix struct size: %lu bytes\n", RAM_usage());
  
  return 0;
}