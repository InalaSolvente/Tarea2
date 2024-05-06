#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
  char id[100];
  char title[100];
  List *genres;
  List *director;
  float calificacion;
  int year;
} Film;

char *espacioInicial(char *str) {

    while (isspace((unsigned char)*str)) str++; // Mientras el primer carácter es un espacio, 
                                                // avanzamos el puntero del inicio de la cadena

    return str; //Retornamos la cadena cambiada

}

void borrarComillas(char *str) { 
  int len = strlen(str); //Obtenemos la longitud de la cadena
  if (str[0] == '"') { //SI el primer carácter es una comilla:
    memmove(str, str + 1, len); //Movemos el inicio de la cadena a un espacio a la derecha
    len--; } //Bajamos en 1 el tamaño de la cadena

  if (len > 0 && str[len - 1] == '"') { //Si el último carácter es una comilla
    str[len - 1] = '\0'; //Lo cambiamos al carácter nulo para que ahí termine la cadena.
  }
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) Buscar por director");
  puts("4) Buscar por género");
  puts("5) Buscar por década");
  puts("6) Buscar por rango de calificaciones");
  puts("7) Buscar por década y género");
  puts("8) Salir");
}
/*
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2)
{
  return *(int *)key1 == *(int *)key2;// Compara valores enteros directamente
}

void mayus(char *palabra){
  for(unsigned int i = 0; palabra[i] != '\0'; i++)
    palabra[i] = toupper(palabra[i]);
}

int transformarDecada(int year){ return (year / 10 * 10);
}


/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
void cargar_peliculas(Map *pelis_byid, Map *peliPorGenero, Map *peliPorDirector) 
{
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) 
  {
    perror("Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    strcpy(peli->id, campos[1]);        // Asigna ID
    strcpy(peli->title, campos[5]);     // Asigna título
    peli->director = list_create(); // Asigna director
    peli->genres = list_create();       // Inicializa la lista de géneros
    peli->year =
        atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
    peli->calificacion = atof(campos[8]);

    borrarComillas(campos[11]); //Borramos las comillas en caso de que haya

    char *token = strtok(campos[11], ","); //Cortamos el campo de géneros hasta una coma

    while (token != NULL) { //Mientras token no sea NULL

        char *clean_token = espacioInicial(token); //Limpiaremos el género en caso de que tenga espacios al principio

        list_pushBack(peli->genres, strdup(clean_token)); //Ahora agregamos el género a la lista de géneros de la película

        token = strtok(NULL, ","); //Volvemos a cortar la siguiente parte de la cadena hasta la coma

    }
    
    borrarComillas(campos[14]); //Borramos las comillas en caso de que haya

    char *token2 = strtok(campos[14], ","); //Cortamos el campo de directores hasta una coma

    while (token2 != NULL) { //Mientras token2 no sea NULL

        char *clean_token2 = espacioInicial(token2); //Limpiaremos el director en caso de que tenga espacios al principio

        list_pushBack(peli->director, strdup(clean_token2)); //Ahora agregamos el director a la lista de directores de la película

        token2 = strtok(NULL, ","); //Volvemos a cortar la siguiente parte de la cadena hasta la coma

    }
    
    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);
    map_insert(peliPorDirector, peli->director, peli);
    map_insert(peliPorGenero, peli->genres, peli);
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas

  // Itera sobre el mapa para mostrar las películas cargadas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    printf("ID: %s, Título: %s ", peli->id, peli->title);
    for(char *director = list_first(peli->director); director != NULL; director = list_next(peli->director))
      printf("Director: %s", director);
    printf(" Año: %d %.1f\n", peli->year, peli->calificacion);
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }
}

/**
 * Busca y muestra la información de una película por su ID en un mapa.
 */
void buscarPorID(Map *pelis_byid) {
  char id[10]; // Buffer para almacenar el ID de la película
  
  // Solicita al usuario el ID de la película
  printf("Ingrese el id de la película: ");
  scanf("%s", id); // Lee el ID del teclado
  
  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(pelis_byid, id);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Film *peli = pair->value; // Obtiene el puntero a la estructura de la película
    
    // Muestra el título y el año de la película
    printf("Título: %s, Año: %d\n", peli->title, peli->year);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n", id);
  }
}

void buscarPorDirector(Map *peliPorDirector) {
  char director[300]; // Buffer para almacenar el ID de la película

  // Solicita al usuario el ID de la película
  printf("Ingrese el nombre del director de la película: ");
  scanf("%s", director); // Lee el ID del teclado

  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(peliPorDirector, director);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Film *peli = pair->value; // Obtiene el puntero a la estructura de la película
    // Muestra el título y el año de la película
    printf("Título: %s, Año: %d\n", peli->title, peli->year);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La pelicula de el/los siguiente/s director/es no fue encontrada %s\n", director);
  }
}

void buscarPorGenero(Map *peliPorGenero){
  char genero[300];
  puts("Ingrese el género de la película: ");
  scanf("%s", genero);
  mayus(genero);
  MapPair *pair = map_search(peliPorGenero, genero);
}

void buscarPorDecada(Map *peliPorDecada) {
  int decada,decadaPeli; // Variable para almacenar la década ingresada por el usuario

  // Solicita al usuario el año de inicio de la década
  puts("Ingrese el año de inicio de la década: ");
  scanf("%d", &decada); // Lee la década del teclado

  decada = (decada / 10) * 10;
  // Busca la primera película en el mapa usando la función map_first
  MapPair *pair = map_first(peliPorDecada);
  
  // Itera sobre todas las películas en el mapa
  
  while (pair != NULL) {
    Film *peli = pair->value;
    decadaPeli = (peli->year / 10) *10;
    if (decada == decadaPeli) {
      printf("Título: %s, Año: %d\n", peli->title, peli->year);
    }
    // Avanza al siguiente par clave-valor en el mapa
    pair = map_next(peliPorDecada);
  }
}

void buscarPorCalificaciones(Map *peliPorID){
  printf( "Ingrese la calificacion minima: ");
  float calificacionMin, calificacionMax,calificacionPeli;
  scanf("%f",&calificacionMin);
  printf( "Ingrese la calificacion maxima: ");
  scanf("%f",&calificacionMax);
  MapPair *pair = map_first(peliPorID);
  Film *peli;
  int cont = 0;
  while(pair != NULL){
    peli = pair->value;
    calificacionPeli = peli->calificacion;
    if(peli->calificacion >= calificacionMin && peli->calificacion <= calificacionMax){
      printf("Titulo: %s Año: %d Calificacion: %f\n", peli->title,peli->year,peli->calificacion);
      cont++;
    }
    pair = map_next(peliPorID);
  }
  if (cont == 0) printf("No se encontraron peliculas con esa calificacion\n");
}


int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)
  
  //Crea un mapa indexado por ID de película.
  Map *peliPorID = map_create(is_equal_str);

  //Crea un mapa indexado por género de película.
  Map *peliPorGenero = map_create(is_equal_str);
  
  //Crea un mapa indexado por década y género de película.
  Map *peliPorDirector = map_create(is_equal_str);
  
  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(peliPorID,peliPorGenero,peliPorDirector);
      break;
    case '2':
      buscarPorID(peliPorID);
      break;
    case '3':
      buscarPorDirector(peliPorDirector);
      break;
    case '4':
      buscarPorGenero(peliPorGenero);
      break;
    case '5':
      buscarPorDecada(peliPorID);
      break;
    case '6':
      buscarPorCalificaciones(peliPorID);
      break;
    case '7':
      //buscarPorDecadaGenero(peliPorDecadaGenero);
      break;
    default:
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}