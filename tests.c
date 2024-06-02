#include <stdio.h>
#include <stdlib.h>
#include "prefix_handle.h"
#include "tests.h"

/* zbiór prefiksów do testowania */
static struct prefix_element test_list[] =
{
  {IP4TOU32(10, 20, 0, 0)  , 16},
  {IP4TOU32(10, 20, 10, 20),  8},
  {IP4TOU32(11, 20, 0, 0),   16},
  {IP4TOU32(128, 20, 0, 0) ,  0},
  {IP4TOU32(10, 20, 10, 20), 32},
  {IP4TOU32(0, 0, 0, 0),     16}
};

static void single_add_test(int nr, unsigned int base, char mask, int expected_result)
{
  int result = add(base, mask);
  
  printf("ADD TEST %d: %s\n", nr, result == expected_result ? "PASSED" : "FAILURE");
}

static void single_del_test(int nr, unsigned int base, char mask, int expected_result)
{
  int result = del(base, mask); 
  printf("DEL TEST %d: %s\n", nr, result == expected_result ? "PASSED" : "FAILURE");
}
// Testy dodawania 
void testAdd() 
{
  printf("Dodawanie prefiksów:\n");

  // dodanie pierwszego prefiksu
  // 10.20.0.0/16
  single_add_test(1, test_list[0].base, test_list[0].mask, SUCCESS);

  // dodawanie prefixu sieci z mniejszą maską
  // 10.20.10.20/8
  single_add_test(2, test_list[1].base, test_list[1].mask, SUCCESS);

  // powienien zwrócić błąd bo prefix jest już w zbiorze i posiada inny adres sieci
  // 11.20.0.0/16
  single_add_test(3, test_list[2].base, test_list[2].mask, FAILURE);

  // wartość brzegowa maski
  // 10.20.0.0/0
  single_add_test(4, test_list[3].base, test_list[3].mask, SUCCESS);

  // wartość brzegowa maski
  // 10.20.0.0/32
  single_add_test(5, test_list[4].base, test_list[4].mask, SUCCESS);

  // zakazany adres
  // 0.0.0.0/8
  single_add_test(6, test_list[5].base, test_list[5].mask, FAILURE);

  printf("\nPrefix struct size: %lu bytes\n\n", RAM_usage());
}

void testDelete() 
{
  printf("Usuwanie prefiksów:\n");

  // Usunięcie 10.20.0.0/16 (który został poprawnie dodany)
  single_del_test(1, test_list[0].base, test_list[0].mask, SUCCESS);

  // Usunięcie 0.0.0.0/16 (który nie został dodany)
  single_del_test(2, test_list[5].base, test_list[5].mask, FAILURE);

  printf("\nPrefix struct size: %lu bytes\n\n", RAM_usage());
}

void testCheck() 
{
  int mask;
  int tab_size = sizeof(test_list) / sizeof(struct prefix_element);

  printf("Sprawdzanie adresów IP:\n");
  
  for(int i=0;i<tab_size ;i++)
  {
    mask = check(test_list[i].base); 
    printf("CHECK TEST %d: MASK = %d for IP " IP4_FORMAT_U " \n", 
      i+1, mask, IP4_PARAMS(test_list[i].base) );
  }

  printf("\n");
}