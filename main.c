#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
typedef struct
{
  int id;
  int x;
  int y;
} Node;
List *read_file();
Node *createNode();

int main()
{
printf("ingreso\n");
List* direcciones=read_file();

    return 0;
}

Node *createNode()
{
  Node *n = (Node *)malloc(sizeof(Node));
  return n;
}


List *read_file()
{
  List *lista = create_list();
  Node *n = createNode();
  FILE *file = fopen("texto.txt", "r");
  int num;
  char leido[10];
  int id = 1;
  int cont = 0;
  while (fscanf(file, "%s", leido) != EOF)
  {
    cont++;
    if (cont == 1)
    {
      Node *n = createNode();
      n->id = id;
      printf("%i ",id);
      num=atoi(leido);
      printf("%i ",num);
      n->x = num;
    }
    else
    {
      cont = 0;
      num=atoi(leido);
      n->y = num;
      printf("%i\n ",num);
      id++;
      push_back(lista, n);
    }
  }

  fclose(file);
  return lista;
}

