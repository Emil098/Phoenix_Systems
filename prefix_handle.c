#include "prefix_handle.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// struktura przechowująca prefiksy
static struct PrefixNode *prefix_list = NULL;

static struct prefix_element forbiddens[] = {
    {IP4TOU32(10, 0, 0, 0),     8},     
    {IP4TOU32(172, 16, 0, 0),  12},
    {IP4TOU32(192, 168, 0, 0), 16},   
    {IP4TOU32(0, 0, 0, 0),      8},
    {IP4TOU32(127, 0, 0, 0),    8},
};

// funkcja do tworzenia nowego węzła
static struct PrefixNode *createNode(unsigned int base, char mask) 
{
  // alokowanie pamięci
  struct PrefixNode *newNode =
      (struct PrefixNode *)malloc(sizeof(struct PrefixNode));

  // przypisanie wartości
  newNode->base = base;
  newNode->mask = mask;
  newNode->next = NULL;
  return newNode;
}


static bool isAddrForbidden(unsigned int base, char mask) 
{
  int tab_size = sizeof(forbiddens) / sizeof(struct prefix_element);
  
  for (int i = 0; i < tab_size; i++) {
    if (base == forbiddens[i].base && mask == forbiddens[i].mask)
      return true;
  }
  return false;
}

// Dodawanie prefiksu do zbioru
int add(unsigned int base, char mask) 
{
  if (mask > 32)
    return FAILURE;

  if (isAddrForbidden(base, mask))
    return FAILURE;

  if (prefix_list == NULL) {
    prefix_list = createNode(base, mask);
    return SUCCESS;
  }

  struct PrefixNode *current = prefix_list;
  struct PrefixNode *prev = NULL;

  // Sprawdzanie, czy prefiks już istnieje
  while (current != NULL) {

    if (current->mask == mask)
      return FAILURE; // Prefiks już istnieje

    prev = current;
    current = current->next;
  }

  struct PrefixNode *newNode = createNode(base, mask);
  prev->next = newNode;

  return SUCCESS;
}

// Usuwanie prefiksu ze zbioru
int del(unsigned int base, char mask) 
{
  if (mask > 32)
    return FAILURE;

  struct PrefixNode *current = prefix_list;
  struct PrefixNode *prev = NULL;

  while (current != NULL) {
    
    if (current->mask == mask && NET_MASK(base, mask) == NET_MASK(current->base, current->mask)) 
    {
      // pominięcie węzła w strukturze
      if (prev == NULL) 
        prefix_list = current->next; // jezeli to jest pierwszy węzeł
      else 
        prev->next = current->next; 

      // zwolnienie pamięci
      free(current);
      return SUCCESS;
    }
    
    prev = current;
    current = current->next;
  }
  return FAILURE; // Nie znaleziono prefiksu
}

// Sprawdzanie, czy adres IP zawiera się w zbiorze prefiksów
// W zadaniu była deklaracja char check ale wtedy nie można byłoby zwrócić -1
// przy błędzie
int check(unsigned int ip) 
{
  struct PrefixNode *current = prefix_list;
  int bestMask = -1;

  while (current != NULL) {

    if (NET_MASK(ip, current->mask) == NET_MASK(current->base, current->mask)) {
      if (current->mask > bestMask) {
        bestMask = current->mask;
      }
    }
    current = current->next;
  }

  return bestMask;
}

// Zwalnianie pamięci listy prefiksów
void free_prefix_list() 
{
  struct PrefixNode *current = prefix_list;
  while (current != NULL) {
    struct PrefixNode *temp = current;
    current = current->next;
    free(temp);
  }
  prefix_list = NULL;
}

// Zwraca rozmiar struktury przechowującej prefiksy
long unsigned int RAM_usage()
{
  long unsigned int usage = 0;

  struct PrefixNode *current = prefix_list;
  while (current != NULL) {
    usage += sizeof(struct PrefixNode);
    current = current->next;
  }
  return usage;
}