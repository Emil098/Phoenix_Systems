#include "stdint.h"

/* makra do przekształcania adresu IP z oktetów na liczbę uint i na odwrót + format pod printf */
#define IP4TOU32(a, b, c, d)                                                   \
  (((uint32_t)((a)&0xFF) << 24) | ((uint32_t)((b)&0xFF) << 16) |               \
   ((uint32_t)((c)&0xFF) << 8) | (uint32_t)((d)&0xFF))

#define IP4_FORMAT_U "%u.%u.%u.%u"
#define IP4_PARAMS(x)                                                          \
  ((x) >> 24) & 0xFF, ((x) >> 16) & 0xFF, ((x) >> 8) & 0xFF, (x)&0xFF

/* przeksztalcanie bazowego adresu IP na adres sieci */
#define NET_MASK(BASE, MASK) (BASE & (0xFFFFFFFF << (32 - MASK)))

/* definicja wartości jakie  zwracają funkcje add del i check */
enum ret_msg { SUCCESS = 0, FAILURE = -1 };

/* arbitralna struktura przechowująca prefiksy */
struct PrefixNode {
  unsigned int base;
  char mask;
  struct PrefixNode *next;
};

/* struktura do sprawdzania prefiksów */
struct prefix_element{
  unsigned int base;
  char mask;
};


int add(unsigned int base, char mask);
int del(unsigned int base, char mask);
int check(unsigned int ip);
void free_prefix_list();
long unsigned int RAM_usage();
