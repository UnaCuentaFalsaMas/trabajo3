#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "list.h"
#include "Map.h"

//Estructuras
typedef struct
{
    int id;
    int x;
    int y;
    float distancia;
} Node;

typedef struct
{
    float total_recorrido;
    List *ruta;
} Estado;

//Funciones del Menu
Map *read_file(char *, int);    //1
void distancia_entregas(Map *); //2
void entregas_cercanas(Map *);  //Muestra las entregas cercanas respecto a coordenadas
void crearRuta(Map *, int, Map *);
void rutaaleatoria(Map *, int, Map *);

//Funciones auxiliares
Node *createNode();
void menu();
float calculo_distancia(Node *, Node *);
void mostrar_distancias(Map *, Node *); //Muestras las distancias de los puntos en el mapa con respecto al nodo apuntado
int is_equal_int(void *, void *);
int is_equal_float(void *, void *);
int is_equal_string(void *, void *);
int lower_than_int(void *, void *);
int lower_than_float(void *, void *);
int lower_than_string(void *, void *);
int greater_than_int(void *, void *);

//main
int main()
{
    Map *mapaUbicaciones;
    Map *estados = createMap(is_equal_string);
    setSortFunction(estados, lower_than_string);
    int max;
    int opcion;
    do
    {
        menu();
        scanf("%i", &opcion);
        char nombre_archivo[30];
        switch (opcion)
        {
        case 1:

            printf("Ingrese el nombre del archivo\n");
            scanf("%s", nombre_archivo);
            printf("Ingrese la cantidad de datos a leer\n");
            scanf("%i", &max);
            mapaUbicaciones = read_file(nombre_archivo, max);
            /*
      Node *aux = firstMap(mapaUbicaciones);
      while (aux){
      printf("%d %d %d\n", aux->id, aux->x, aux->y);
      aux = nextMap(mapaUbicaciones);
      }
      */

            break;
        case 2:
            distancia_entregas(mapaUbicaciones);
            break;
        case 3:
            entregas_cercanas(mapaUbicaciones);
            break;
        case 4:
            crearRuta(mapaUbicaciones, max, estados);
            break;
        case 5:
            rutaaleatoria(mapaUbicaciones, max, estados);
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            printf("Gracias por usar nuestros servicios\n%*cVuelva Pronto\n", 10, ' ');
            break;

        default:
            printf("La opcion %i no es valida intente nuevamente \n", opcion);
            break;
        }
    } while (opcion != 9);

    return 0;
}

//Función 1: Lee n coordenadas de entregas desde un archivo
Map *read_file(char *nombre, int max)
{
    Map *mapa = createMap(is_equal_int);
    setSortFunction(mapa, lower_than_int);
    Node *n;
    FILE *file = fopen(nombre, "r");
    int num;
    char lectura[10];
    int id = 1;
    int cont = 0;
    while (fscanf(file, "%s", lectura) != EOF && id <= max)
    {
        fgetc(file);
        num = atoi(lectura);
        //printf("%d\n", num);

        if (cont == 0)
        {
            n = createNode();
            n->id = id;
            n->x = num;
            cont++;
        }
        else
        {
            n->y = num;
            ;
            id++;
            cont = 0;
            insertMap(mapa, &n->id, n);
        }
    }
    fclose(file);
    return mapa;
}

//Función 2: Entrega la distancia entre dos entregas dispuestas por el usuario
void distancia_entregas(Map *mapa_local)
{
    int id;
    float distancia;
    Node *aux1, *aux2;

    printf("Ingrese la ID de la entrega 1: ");
    scanf("%d", &id);
    aux1 = searchMap(mapa_local, &id);

    printf("Ingrese la ID de la entrega 2: ");
    scanf("%d", &id);
    aux2 = searchMap(mapa_local, &id);

    distancia = calculo_distancia(aux1, aux2);
    printf("La distancia entre la entrega %d y la entrega %d es de %0.f m.\n", aux1->id, aux2->id, distancia);
    printf("----------------------------------\n");
}

//Funcion 3: Muestra 3 entregas mas cercanas respecto a coordenadas
void entregas_cercanas(Map *mapa_original)
{
    Node *nodo = createNode();
    printf("Ingrese su ubicacion (x,y)\n");
    printf("Escriba la coordenada x:\n");
    scanf("%i", &nodo->x);
    printf("Escriba la coordenada y:\n");
    scanf("%i", &nodo->y);

    Node *aux;
    Map *mapa_distancias = createMap(is_equal_int);
    setSortFunction(mapa_distancias, lower_than_int);

    //Se guardan las distancias en un mapa que guarda las distancias en orden
    aux = firstMap(mapa_original);
    while (aux)
    {
        if (aux->id == 0)
        {
            aux = nextMap(mapa_original);
            continue;
        }
        aux->distancia = trunc(calculo_distancia(nodo, aux));
        insertMap(mapa_distancias, &aux->distancia, aux);
        //printf("ID: %d | (%d, %d) | %0.f metros\n", aux->id, aux->x, aux->y, aux->distancia);
        aux = nextMap(mapa_original);
    }

    if (!firstMap(mapa_distancias))
        return;
    else
        printf("Entregas ordenadas desde la mas cercana a la mas lejana:\n");

    //Se muestran las distancias en orden
    aux = firstMap(mapa_distancias);
    int cont = 0;
    while (aux && cont < 3)
    {
        printf("ID: %d | (%d, %d) | %0.f m.\n", aux->id, aux->x, aux->y, aux->distancia);
        aux = nextMap(mapa_distancias);
        cont++;
    }
    printf("----------------------------------\n");

    if (cont != 3)
    {
        printf("Solo habian %i entregas en la base de datos\n", cont);
    }
}

//Función 4: Crea un ruta según lo que disponga el usuario y la guarda con un nombre
void crearRuta(Map *mapa_original, int max, Map *estados)
{
    Estado estado_actual;
    Node *nodo_inicial = createNode(), *aux;
    int id;

    nodo_inicial->id = 0;
    printf("Ingrese su ubicacion (x, y)\n");
    printf("x: ");
    scanf("%d", &nodo_inicial->x);
    printf("y: ");
    scanf("%d", &nodo_inicial->y);

    insertMap(mapa_original, &nodo_inicial->id, nodo_inicial);
    //printf("ID: %d | (%d, %d)\n", nodo_inicial->id, nodo_inicial->x, nodo_inicial->y);
    printf("%d entregas en total\n", max);
    printf("----------------------------------\n");

    Map *mapa_local = mapa_original;

    mostrar_distancias(mapa_local, nodo_inicial);

    estado_actual.ruta = create_list();
    estado_actual.total_recorrido = 0;

    for (int i = 0; i < max; i++)
    {
        printf("Ingrese su proxima parada: ");
        scanf("%d", &id);

        aux = searchMap(mapa_local, &id);
        if (aux == NULL)
            break;
        else
            eraseMap(mapa_local, &id);

        push_back(estado_actual.ruta, &id);
        estado_actual.total_recorrido += aux->distancia;

        mostrar_distancias(mapa_local, aux);
    }
    printf("----------------------------------\n");
    printf("Todas las ciudades visitadas\n");
    printf("Distancia final recorrida: %0.f metros\n", estado_actual.total_recorrido);

    printf("Ingrese el nombre de la ruta creada: ");
    char nombre_ruta[30];
    getchar();
    scanf("%[^\n]s", nombre_ruta);
    //printf("%s\n", nombre_ruta);
    printf("----------------------------------\n");

    /* Se guarda en el mapa "estados" el dato "estado_actual" de tipo Estado con clase nombre_ruta   */
    insertMap(estados, nombre_ruta, &estado_actual);
}

//Funcion 5: Entrega una ruta aleatoria
void rutaaleatoria(Map *mapa_original, int max, Map *estados)
{
    int i, id, bool;

    Node *aux = createNode();
    Node *p = createNode();
    srand(time(NULL));
    Estado aleatorio;
    aleatorio.ruta = create_list();

    for (i = 0; i < max; i++)
    {
        if (i > 0)
        {
            id = rand() % max + 1;
            p = first(aleatorio.ruta);
            while (p)
            {
                bool = 0;
                if (p->id == id)
                {
                    id = rand() % max + 1;
                    p = first(aleatorio.ruta);
                    bool = 1;
                }
                if (bool != 1)
                {
                    p = next(aleatorio.ruta);
                }
            }
        }
        else
        {
            id = rand() % max + 1;
        }

        aux = searchMap(mapa_original, &id);
        aleatorio.total_recorrido += aux->distancia;
        push_back(aleatorio.ruta, aux);
    }

    printf("\n\nIngrese el nombre de la ruta creada: ");
    char nombre_ruta[30];
    getchar();
    scanf("%[^\n]s", nombre_ruta);
    printf("\n\n----------------------------------\n\n");

    insertMap(estados, nombre_ruta, &aleatorio);
    /*p=first(aleatorio.ruta);
  for(i=0;i<max;i++){
    printf("%d-",p->id);
    p=next(aleatorio.ruta);
  }*/
}

void menu()
{
    printf("1. Importar archivo\n");
    printf("2. Distancia entre entregas\n");
    printf("3. Mostrar 3 entregas mas cercanas\n");
    printf("4. Crear ruta\n");
    printf("5. Generar ruta aleatoria\n");
    printf("6. Mejorar ruta\n");
    printf("7. Mostrar rutas\n");
    printf("8. Mejor ruta\n");
    printf("9. Salir\n");
}

Node *createNode()
{
    Node *n = (Node *)malloc(sizeof(Node));
    return n;
}

void mostrar_distancias(Map *mapa_local, Node *nodo_referencia)
{
    Node *aux;
    Map *mapa_distancias = createMap(is_equal_int);
    setSortFunction(mapa_distancias, lower_than_int);

    //Se guardan las distancias en un mapa que guarda las distancias en orden
    aux = firstMap(mapa_local);
    while (aux)
    {
        if (aux->id == 0)
        {
            aux = nextMap(mapa_local);
            continue;
        }
        aux->distancia = trunc(calculo_distancia(nodo_referencia, aux));
        insertMap(mapa_distancias, &aux->distancia, aux);
        //printf("ID: %d | (%d, %d) | %0.f metros\n", aux->id, aux->x, aux->y, aux->distancia);
        aux = nextMap(mapa_local);
    }

    if (!firstMap(mapa_distancias))
        return;
    else
        printf("Distancias:\n");

    //Se muestran las distancias en orden
    aux = firstMap(mapa_distancias);
    while (aux)
    {
        printf("ID: %d | (%d, %d) | %0.f m.\n", aux->id, aux->x, aux->y, aux->distancia);
        aux = nextMap(mapa_distancias);
    }
    printf("----------------------------------\n");

    return;
}

float calculo_distancia(Node *cordenada_1, Node *cordena_2)
{
    float x = cordena_2->x - cordenada_1->x;
    float y = cordena_2->y - cordenada_1->y;
    float distancia = pow(x, 2) + pow(y, 2);
    distancia = sqrt(distancia);
    return distancia;
}

//Función para comparar claves de tipo int. Retorna 1 si son iguales
int is_equal_int(void *key1, void *key2)
{
    if (*(int *)key1 == *(int *)key2)
        return 1;
    return 0;
}

int is_equal_float(void *key1, void *key2)
{
    if (*(float *)key1 == *(float *)key2)
        return 1;
    return 0;
}

//Función para comparar claves de tipo int. Retorna 1 si key1 < key2
int lower_than_int(void *key1, void *key2)
{
    if (*(int *)key1 < *(int *)key2)
        return 1;
    return 0;
}

int lower_than_float(void *key1, void *key2)
{
    if (*(float *)key1 < *(float *)key2)
        return 1;
    return 0;
}

//Función para comparar claves de tipo int. Retorna 1 si key1 > key2
int greater_than_int(void *key1, void *key2)
{
    if (*(int *)key1 > *(int *)key2)
        return 1;
    return 0;
}

int is_equal_string(void *key1, void *key2)
{
    if (strcmp((char *)key1, (char *)key2) == 0)
        return 1;
    return 0;
}

int lower_than_string(void *key1, void *key2)
{
    if (strcmp((char *)key1, (char *)key2) < 0)
        return 1;
    return 0;
}
