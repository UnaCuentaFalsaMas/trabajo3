#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "list.h"
#include "Map.h"

//Estructuras

// Estructura para guardar las coordenadas de las entregas
typedef struct
{
    int id;
    int x;
    int y;
    float distancia;
} Node;

// Estructura para guardar la ruta
typedef struct
{
    float total_recorrido;
    int *ruta;
    char nombre[30];
} Estado;

//Funciones del Menu
Map *read_file(char *, int);            //Función 1
void distancia_entregas(Map *);         //Función 2
void entregas_cercanas(Map *);          //Función 3
void crearRuta(Map *, int, Map *);      //Función 4
void ruta_aleatoria(Map *, int, Map *); //Función 5
void mejorar_ruta(Map *, int, Map *);   //Función 6
void mostrar_rutas(Map*, int);          //Función 7
void mejor_ruta(Map *,int, Map *);      //Función 8

//Funciones auxiliares
Node *createNode();
void menu();
float calculo_distancia(Node *, Node *);
Map *pasar_mapa(Map*);
void mostrar_distancias(Map *, Node *); //Muestras las distancias de los puntos en el mapa con respecto al nodo apuntado
int is_equal_int(void *, void *);
int is_equal_float(void *, void *);
int is_equal_string(void *, void *);
int lower_than_int(void *, void *);
int lower_than_float(void *, void *);
int lower_than_string(void *, void *);

//main
int main()
{
    Map *mapaUbicaciones;
    Map *estados = createMap(is_equal_string);
    setSortFunction(estados, lower_than_string);
    int max;
    int opcion;
    int verificar = 0;
    do
    {
        menu();
        scanf("%i", &opcion);
        char nombre_archivo[30];
        switch (opcion)
        {
        case 1:

            printf("Ingrese el nombre del archivo\n");
            getchar();
            scanf("%[^\n]s", nombre_archivo);
            printf("Ingrese la cantidad de datos a leer\n");
            scanf("%i", &max);
            mapaUbicaciones = read_file(nombre_archivo, max);
            verificar = 1;
            break;
        case 2:
            if(verificar == 0){
                printf("SELECCIONE LA OPCION 1 PRIMERO\n");
                printf("------------------------------\n");
                break;
            }
            distancia_entregas(mapaUbicaciones);
            break;
        case 3:
            if(verificar == 0){
                printf("SELECCIONE LA OPCION 1 PRIMERO\n");
                printf("------------------------------\n");
                break;
            }
            entregas_cercanas(mapaUbicaciones);
            break;
        case 4:
            if(verificar == 0){
                printf("SELECCIONE LA OPCION 1 PRIMERO\n");
                printf("------------------------------\n");
                break;
            }
            crearRuta(mapaUbicaciones, max, estados);
            break;
        case 5:
            if(verificar == 0){
                printf("SELECCIONE LA OPCION 1 PRIMERO\n");
                printf("------------------------------\n");
                break;
            }
            ruta_aleatoria(mapaUbicaciones, max, estados);
            break;
        case 6:
            if(verificar == 0){
                printf("SELECCIONE LA OPCION 1 PRIMERO\n");
                printf("------------------------------\n");
                break;
            }
            mejorar_ruta(mapaUbicaciones, max, estados);
            break;
        case 7:
            if(verificar == 0){
                printf("SELECCIONE LA OPCION 1 PRIMERO\n");
                printf("------------------------------\n");
                break;
            }
            mostrar_rutas(estados, max);
            break;
        case 8:
            if(verificar == 0){
                printf("SELECCIONE LA OPCION 1 PRIMERO\n");
                printf("------------------------------\n");
                break;
            }
            mejor_ruta(mapaUbicaciones,max, estados);
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
    if(aux1 == NULL){
        printf("Ingrese una ID valida\n");
        while(aux1 == NULL){
            scanf("%d", &id);
            aux1 = searchMap(mapa_local, &id);
        }
    }

    printf("Ingrese la ID de la entrega 2: ");
    scanf("%d", &id);
    aux2 = searchMap(mapa_local, &id);
    if(aux2 == NULL){
        printf("Ingrese una ID valida\n");
        while(aux2 == NULL){
            scanf("%d", &id);
            aux2 = searchMap(mapa_local, &id);
        }
    }


    distancia = calculo_distancia(aux1, aux2);
    printf("La distancia entre la entrega %d y la entrega %d es de %0.f m.\n", aux1->id, aux2->id, distancia);
    printf("----------------------------------\n");
}

//Funcion 3: Muestra 3 entregas más cercanas respecto a coordenadas
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
    Estado *estado_actual = (Estado *) malloc (sizeof(Estado) * 1000);
    estado_actual->ruta = (int*) malloc(sizeof(int) * max);
    estado_actual->total_recorrido = 0;
    Node *nodo_inicial = createNode(), *aux;
    Map *mapa_local = pasar_mapa(mapa_original);
    int id;

    nodo_inicial->id = 0;
    printf("Ingrese su ubicacion (x, y)\n");
    printf("x: ");
    scanf("%d", &nodo_inicial->x);
    printf("y: ");
    scanf("%d", &nodo_inicial->y);

    insertMap(mapa_original, &nodo_inicial->id, nodo_inicial);
    printf("%d entregas en total\n", max);
    printf("----------------------------------\n");

    //Se muestran las distancias desde el nodo inicial hasta las entregas
    mostrar_distancias(mapa_local, nodo_inicial);

    //Una vez se elija el nodo inicial, el usuario va cambiando de posición y se muestran las distancias desde su posición hasta las otras entregas
    for (int i = 0; i < max; i++)
    {
        printf("Ingrese su proxima parada: ");
        scanf("%d", &id);

        aux = searchMap(mapa_local, &id);
        if (aux == NULL){
            printf("Ingrese una parada valida.\n");
            i--;
            continue;
        }else{
            //Se elimina del mapa local la entrega ya visitada y se guarda su ID en el arreglo ruta
            eraseMap(mapa_local, &id);
            estado_actual->ruta[i] = id;
            estado_actual->total_recorrido += aux->distancia;
        }

        mostrar_distancias(mapa_local, aux);
    }
    printf("----------------------------------\n");
    printf("Todas las ciudades visitadas. Volviendo al punto de origen.\n");
    estado_actual->total_recorrido += calculo_distancia(aux, firstMap(mapa_original));
    printf("Distancia final recorrida: %0.f metros\n", estado_actual->total_recorrido);

    printf("Ingrese el nombre de la ruta creada: ");
    getchar();
    scanf("%[^\n]s", estado_actual->nombre);
    printf("----------------------------------\n");

    // Se guarda en el mapa "estados" el dato "estado_actual" de tipo Estado con clave "estado_actual->nombre"
    insertMap(estados, estado_actual->nombre, estado_actual);
}

//Funcion 5: Entrega una ruta aleatoria
void ruta_aleatoria(Map *mapa_original, int max, Map *estados)
{
    int i,j,id,bool;
 
    Node* aux = createNode();
    Estado *aleatorio=(Estado *) malloc (sizeof(Estado) * 1000);
    aleatorio->ruta=(int*) malloc(sizeof(int) * max);
    srand(time(NULL));
    int arreglo [max+1];
    
    //Se verifica con un arreglo que no se vuelva a visitar la misma ID
    for(j=0;j<=max;j++){
        arreglo[j]=0;
    }

    //Se ingresa una ID aleatoria en la ruta, esta no se repite debido al arreglo verificado
    for(i=0;i<max;i++){
        bool=0;
        id=rand()%max+1;
        do{
            if(arreglo[id]!=1){
                aleatorio->ruta[i]=id;
                arreglo[id]=1;
                bool=1;
            }else id=rand()%max+1;
        }while(bool==0);
    }
    
    aleatorio->total_recorrido = 0;
    aleatorio->total_recorrido += calculo_distancia(firstMap(mapa_original), searchMap(mapa_original, &aleatorio->ruta[0]));
    
    for (i = 0; i < max - 1; i++){
        aleatorio->total_recorrido += calculo_distancia(searchMap(mapa_original, &aleatorio->ruta[i]), searchMap(mapa_original, &aleatorio->ruta[i + 1]));
    }
    aleatorio->total_recorrido += calculo_distancia(searchMap(mapa_original, &aleatorio->ruta[max - 1]), firstMap(mapa_original));

    printf("--------- RUTA ALEATORIA ---------\n");
    printf("Ruta generada: ");
    for (i = 0; i < max; i++){
        printf("%d", aleatorio->ruta[i]);
        if (i + 1 < max) printf(" - ");
        
    }
    printf("\nDistancia recorrida en ruta generada: %0.f metros\n", aleatorio->total_recorrido);
    printf("Ingrese un nombre para la ruta generada: ");
    getchar();
    scanf("%[^\n]s", aleatorio->nombre);
    printf("----------------------------------\n");
   insertMap(estados, aleatorio->nombre, aleatorio);
}

//Función 6: Se carga el mapa de las rutas por nombre. El usuario elige si hacer un intercambio manual o automático entre 2 entregas.
void mejorar_ruta(Map *mapa_original, int max, Map *estados){
    int opcion, aux, id1, id2, i, j = -1, k = -1;
    Estado *estado_mejorado = (Estado *) malloc (sizeof(Estado) * 1000);; //La copia es para no alterar la ruta original, en caso de que la ruta alterada sea peor que la original
    estado_mejorado->ruta=(int*) malloc(sizeof(int) * max);
    srand(time(NULL));

    getchar();
    printf("----------------------------------\n");
    printf("Ingrese el nombre de la ruta a mejorar: ");
    scanf("%[^\n]s", estado_mejorado->nombre);
    Estado *estado_a_mejorar = searchMap(estados, estado_mejorado->nombre);

    printf("--------------- RUTA --------------\n");

    //Se copia la ruta original para no perderla por si la mejora no es mejor (xd)
    for (i = 0; i < max; i++){
        estado_mejorado->ruta[i] = estado_a_mejorar->ruta[i];
    }

    //Se muestra la ruta escogida a mejorar
    for (i = 0; i < max; i++){
        printf("%d", estado_mejorado->ruta[i]);
        if (i + 1 < max) printf(" - ");
    }
    printf("\n");
    printf("----------------------------------\n");
    printf("Ajuste automatico (1) / Ajuste manual (2): ");
    scanf("%d", &opcion);
    if (opcion == 2){
        printf("Ingrese la primera ID a intercambiar: ");
        scanf("%d", &id1);

        printf("Ingrese la segunda ID a intercambiar: ");
        scanf("%d", &id2);
    }
    else{
        //Se escogen dos entregas aleatorias a intercambiar
        id1 = rand()%max + 1;
        do{
            id2 = rand()%max + 1;
        }while (id1 == id2);

        printf("Se intercambiaran los ordenes de la entrega %d y la entrega %d.\n", id1, id2);
    }

    //Se intercambia el orden de las entregas
    for (int i = 0; i < max; i++){
        if (estado_mejorado->ruta[i] == id1){
            j = i;
            break;
        }
    }
    for (int i = 0; i < max; i++){
        if (estado_mejorado->ruta[i] == id2){
            k = i;
            break;
        }
    }

    aux = estado_mejorado->ruta[j];
    estado_mejorado->ruta[j] = estado_mejorado->ruta[k];
    estado_mejorado->ruta[k] = aux;

    //Se calcula la distancia de la nueva ruta
    estado_mejorado->total_recorrido = 0;
    estado_mejorado->total_recorrido += calculo_distancia(firstMap(mapa_original), searchMap(mapa_original, &estado_mejorado->ruta[0]));
    
    for (i = 0; i < max - 1; i++){
        estado_mejorado->total_recorrido += calculo_distancia(searchMap(mapa_original, &estado_mejorado->ruta[i]), searchMap(mapa_original, &estado_mejorado->ruta[i + 1]));
    }
    estado_mejorado->total_recorrido += calculo_distancia(searchMap(mapa_original, &estado_mejorado->ruta[max - 1]), firstMap(mapa_original));

    for (int i = 0; i < max; i++){
        printf("%d", estado_mejorado->ruta[i]);
        if (i + 1 < max) printf(" - ");
    }
    printf("\nDistancia recorrida en ruta mejorada: %0.f metros\n", estado_mejorado->total_recorrido);

    //Si la nueva ruta es mejor, se borra la ruta sain mejorar del mapa de rutas y se ingresa la ruta mejorada
    if (estado_mejorado->total_recorrido < estado_a_mejorar->total_recorrido){
        eraseMap(estados, estado_mejorado->nombre);
        insertMap(estados, estado_mejorado->nombre, estado_mejorado);
        printf("La ruta fue mejorada exitosamente. Los cambios fueron aplicados.\n");
    }
    else{
        printf("La ruta mejorada es peor que la anterior. Los cambios fueron revertidos.\n");
    }
    
}

//Función 7: Se muestran las rutas guardadas de la mejor a la peor.
void mostrar_rutas(Map* rutas_original, int max){
    //Se define un mapa ordenado de menor a mayor con clave igual a la distancia total de la ruta
    Map *rutas_orden_local = createMap(is_equal_float);
    setSortFunction(rutas_orden_local, lower_than_float);

    Estado *aux;
    //Se copian los contenidos del mapa ordenado según el nombre de las rutas al mapa que los ordena por distancia
    aux = firstMap(rutas_original);
    while (aux){
        insertMap(rutas_orden_local, &aux->total_recorrido, aux);
        aux = nextMap(rutas_original);
    }
    printf("-------------- RUTAS --------------\n");
    aux = firstMap(rutas_orden_local);
    while (aux){
        printf("%s | ", aux->nombre);
        for (int i = 0; i < max; i++){
            printf("%d", aux->ruta[i]);
            if (i + 1 < max) printf(" - ");
        }
        printf(" | %0.f m.\n", aux->total_recorrido);
        aux = nextMap(rutas_orden_local);
    }
    printf("----------------------------------\n");
}

//Funcion 8: Se guarda la ruta optima
void mejor_ruta(Map *mapa_original,int max, Map *estados){

    Estado *estado_actual = (Estado *) malloc (sizeof(Estado) * 1000);
    estado_actual->ruta = (int*) malloc(sizeof(int) * max);
    estado_actual->total_recorrido = 0;
    //METODO CAMINO HAMILTONIANO 
    Node* nodo_inicial=createNode();
    Node* auxi = createNode();
    Node* auxj = createNode();
    Node* nodo1 = createNode();
    Node *nodo2 = createNode();
    printf("Ingrese coordenada x: ");
    scanf("%d",&nodo_inicial->x);
    printf("Ingrese coordenada y: ");
    scanf("%d",&nodo_inicial->y);
    nodo_inicial->id=0;
    float M_distancias[max+1][max+1];
    int array_id[max+1];
    int i,j,k,b,auxd,auxid;

    for(i=0;i<=max;i++){
        if(i>0){
            auxi=searchMap(mapa_original,&i);
        }else auxi=nodo_inicial;
        
        for(j=0;j<=max;j++){
            if(j>0){
                auxj=searchMap(mapa_original,&j);
            }else auxj=nodo_inicial;
            M_distancias[i][j]=calculo_distancia(auxi,auxj);
        }
    }
    for(k=0;k<=max;k++){
        M_distancias[k][0]=0;
    }

    array_id[0]=0;
    for(i=0;i<=max;i++){
        if(i>0) array_id[i]=auxid;
        b=0;
        auxd=99999;
        for(j=0;j<=max;j++){
            if(M_distancias[array_id[i]][j]!=0 && M_distancias[array_id[i]][j]<auxd){
                auxd=M_distancias[array_id[i]][j];
                for(k=0;k<=i;k++){
                    if(array_id[k]==j){
                        b=1;
                        break;
                    } 
                }
                if(b==0){
                    auxid=j;
                }
                b=0;
            }
        }
        for(k=0;k<=max;k++){
            M_distancias[k][auxid]=0;
        }
    }


    //Orden resultante de los nodos por su ID (ruta óptima)
    printf("Ruta optima: ");
    for(i=1;i<max;i++){
        printf("%d - ",array_id[i]);
        estado_actual->ruta[i - 1] = array_id[i];
    }
    estado_actual->ruta[i-1] = array_id[i];
    printf("%d\n",array_id[i]);

    //Guarda el nombre de la ruta
    strcpy(estado_actual->nombre, "ruta optima");

    i = array_id[1];
    nodo1 = searchMap(mapa_original,&i);
    estado_actual->total_recorrido+= calculo_distancia(nodo_inicial,nodo1);
   
    for(int cont = 1; cont<max;cont++){
        i = array_id[cont];
        j = array_id[cont + 1];
        nodo1 = searchMap(mapa_original,&i);
        nodo2 = searchMap(mapa_original,&j);
        estado_actual->total_recorrido+= calculo_distancia(nodo1,nodo2);
        nodo1 = createNode();
        nodo2 = createNode();
    }
    //Calcular la vuelta al inicio
    nodo1 = searchMap(mapa_original,&j);
    estado_actual->total_recorrido+= calculo_distancia(nodo1,nodo_inicial);

    insertMap(estados,estado_actual->nombre,estado_actual);
    printf("RUTA GUARDADA EXITOSAMENTE\n");
}

//Display del programa
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

//Función que muestra las distancias entre un punto y las entregas
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

//Función que calcula la distancia entre dos puntos
float calculo_distancia(Node *cordenada_1, Node *cordena_2)
{
    float x = cordena_2->x - cordenada_1->x;
    float y = cordena_2->y - cordenada_1->y;
    float distancia = pow(x, 2) + pow(y, 2);
    distancia = sqrt(distancia);
    return distancia;
}

//Función que hace la copia de un mapa
Map *pasar_mapa(Map* mapa_original){
  Map *entregas = createMap(is_equal_int);
  Node *aux2 = createNode();
  Node *aux = firstMap(mapa_original);
  while (aux)
  {
    aux2->x = aux->x;
    aux2->y = aux->y;
    aux2->id = aux->id;
    insertMap(entregas,&aux2->id,aux2);
    aux2 = createNode();
    aux = nextMap(mapa_original);
  }
  return entregas;
}

//Función para comparar claves de tipo int. Retorna 1 si son iguales
int is_equal_int(void *key1, void *key2)
{
    if (*(int *)key1 == *(int *)key2)
        return 1;
    return 0;
}

//Función para comparar claves de tipo float. Retorna 1 si son iguales
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

//Función para comparar claves de tipo float. Retorna 1 si key1 < key2
int lower_than_float(void *key1, void *key2)
{
    if (*(float *)key1 < *(float *)key2)
        return 1;
    return 0;
}

//Función para comparar claves de tipo string. Retorna 1 si son iguales
int is_equal_string(void *key1, void *key2)
{
    if (strcmp((char *)key1, (char *)key2) == 0)
        return 1;
    return 0;
}

//Función para comparar claves de tipo string. Retorna 1 si key1 < key2
int lower_than_string(void *key1, void *key2)
{
    if (strcmp((char *)key1, (char *)key2) < 0)
        return 1;
    return 0;
}
