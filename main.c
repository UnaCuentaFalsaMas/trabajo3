#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

//Estrucuturas
typedef struct
{
  int id;
  int x;
  int y;
} Node;

//Funciones
Node *createNode();
List *read_file(char *, int);
void menu();

//main
int main()
{
  int opcion;
  do
  {
    menu();
    scanf("%i", &opcion);
    List* listaUbicaciones;
    int max;
    char nombre_archivo[30];
    switch (opcion)
    {
    case 1:

    printf("Ingrese el nombre del archivo\n");
    scanf("%s",nombre_archivo);
    printf("Ingrese la cantidad de datos a leer\n");
    scanf("%i",&max);
    listaUbicaciones=read_file("texto.txt",47);
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      break;
    case 8:
      break;
    case 9:
      printf("Gracias por usar nuestros servicios\n%*cVuelva Pronto\n",10,' ');
      break;

    default:
      printf("La opcion %i no es valida intente nuevamente \n", opcion);
      break;
    }
  } while (opcion != 9);

  return 0;
}

void menu()
{
  printf("1.- Importar archivo\n");
  printf("2.- Distancia entre entregas\n");
  printf("3.- Mostrar 5 entregas mas cercanas\n");
  printf("4.- Crear ruta\n");
  printf("5.- Generar ruta aleatoria\n");
  printf("6.- Mejorar ruta\n");
  printf("7.- Mostrar rutas\n");
  printf("8.- Mejor ruta\n");
  printf("9.- Salir\n");
}

Node *createNode()
{
  Node *n = (Node *)malloc(sizeof(Node));
  return n;
}

List *read_file(char *nombre, int max)
{
  List *lista = create_list();
  Node *n = createNode();
  FILE *file = fopen(nombre, "r");
  int num;
  char leido[10];
  int id = 1;
  int cont = 0;
  while (fscanf(file, "%s", leido) != EOF && id <= max)
  {
    cont++;
    if (cont == 1)
    {
      Node *n = createNode();
      n->id = id;
      printf("%i ", id);
      num = atoi(leido);
      printf("%i ", num);
      n->x = num;
    }
    else
    {
      cont = 0;
      num = atoi(leido);
      n->y = num;
      printf("%i\n ", num);
      id++;
      push_back(lista, n);
    }
  }

  fclose(file);
  return lista;
}
