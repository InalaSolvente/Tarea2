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


//Función para imprimir siempre las peliculas con el mismo formato
void imprimirPelicula(Film *peli){
  printf("\nID: %s, Título: %s, Año: %d, Director(es):", peli->id,peli->title, peli->year);
  for(char *director = list_first(peli->director); director != NULL; director = list_next(peli->director)) printf(" %s", director);
  printf(" Genero(s):");
  for(char *genero = list_first(peli->genres); genero != NULL;genero = list_next(peli->genres)) printf(" %s",genero);
  printf(" Calificación: %.1f", peli->calificacion);
  printf("\n");
}

List* split_string(const char *str, const char *delim) {
    List *result = list_create();
    char *token = strtok((char *)str, delim);

    while (token != NULL) {
        // Eliminar espacios en blanco al inicio del token
        while (*token == ' ') {
            token++;
        }

        // Eliminar espacios en blanco al final del token
        char *end = token + strlen(token) - 1;
        while (*end == ' ' && end > token) {
            *end = '\0';
            end--;
        }

        // Copiar el token en un nuevo string
        char *new_token = strdup(token);

        // Agregar el nuevo string a la lista
        list_pushBack(result, new_token);

        // Obtener el siguiente token
        token = strtok(NULL, delim);
    }

    return result;
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
    peli->director = split_string(campos[14] , ","); // Inicializa lista de directores
    peli->genres = split_string(campos[11] , ",");       
    //Inicializa la lista de géneros
    peli->year =
        atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
    //Asigna calificación
    peli->calificacion = atof(campos[8]);

    // Inserta la película en cada uno de los mapas
    map_insert(pelis_byid, peli->id, peli);
    map_insert(peliPorDirector, peli->director, peli);
    map_insert(peliPorGenero, peli->genres, peli);
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas

  // Itera sobre el mapa para mostrar las películas cargadas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    imprimirPelicula(peli);
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
    // Se imprime la pelicula
    imprimirPelicula(peli);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n", id);
  }
}

void buscarPorDirector(Map *peliPorDirector) {
  char director[100]; // Buffer para almacenar el nombre del director

  //Limpiar el buffer
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
  // Solicita al usuario el nombre del director de la película
  printf("Ingrese el nombre del director de la película: ");
  fgets(director, sizeof(director), stdin); // Lee el nombre del director del usuario

  // Elimina el carácter de nueva línea
  size_t len = strlen(director);
  if (len > 0 && director[len - 1] == '\n') {
      director[len - 1] = '\0';
  }

  // Itera sobre el mapa para buscar películas por el director ingresado
  MapPair *pair = map_first(peliPorDirector);
  int contador = 0;
  while (pair != NULL) {
    Film *peli = pair->value;
    // Busca el nombre del director en la lista de directores de la película
    for (char *dir = list_first(peli->director); dir != NULL; dir = list_next(peli->director)) {
      // Compara el nombre del director con el director actual en la lista (sin distinción de mayúsculas/minúsculas)
      if (strcasecmp(director, dir) == 0) {
        // Si el nombre del director coincide, muestra la información de la película
        imprimirPelicula(peli);
        contador++;
        break; // No es necesario seguir buscando en esta película
      }
    }
    pair = map_next(peliPorDirector); // Avanza al siguiente par en el mapa
  }

  // Si no se encontraron películas del director, informa al usuario
  if (contador == 0) printf("No se encontraron películas del director: %s\n", director);
  //Sino, muestra el número de películas encontradas
  else printf("Se encontraron %d películas del director: %s\n", contador, director);
}


void buscarPorGenero(Map *peliPorGenero) {
  char genero[100]; //Se guarda memoria para la variable a utilizar

  //Limpia el buffer
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
  
  // Solicita al usuario el nombre del género de la película
  printf("Ingrese el género de la película: ");
  fgets(genero, sizeof(genero), stdin); // Lee el nombre del género del usuario

  // Elimina el carácter de nueva línea
  size_t len = strlen(genero);
  if (len > 0 && genero[len - 1] == '\n') {
      genero[len - 1] = '\0';
  }

  // Itera sobre el mapa para buscar películas por el género ingresado
  MapPair *pair = map_first(peliPorGenero);
  int contador = 0; // Variable para indicar si se encontraron películas del género
  while (pair != NULL) {
    Film *peli = pair->value;
    // Busca el nombre del género en la lista de géneros de la película
    for (char *gen = list_first(peli->genres); gen != NULL; gen = list_next(peli->genres)) {
      // Compara el nombre del género con el género actual
      if (strcasecmp(genero, gen) == 0) {
        // Si el nombre del género coincide, muestra la información de la película
        imprimirPelicula(peli);
        contador++;
        break; // No es necesario seguir buscando en esta película
      }
    }
    pair = map_next(peliPorGenero); // Avanza al siguiente par en el mapa
  }

  // Si no se encontraron películas del género, informa al usuario
  if (contador == 0) printf("No se encontraron películas del género: %s\n", genero);
  //Si se encontraron, se imprime cuantas
  else printf("Se encontraron %d películas del género: %s\n", contador, genero);
}


void buscarPorDecada(Map *peliPorID) {
  int decada,decadaPeli,contador = 0; // Variable para almacenar la década ingresada por el usuario

  // Solicita al usuario el año de inicio de la década
  puts("Ingrese el año de inicio de la década: ");
  scanf("%d", &decada); // Lee la década del teclado

  decada = (decada / 10) * 10;
    // Nos posicionamos al inicio del mapa para iterar sobre todas las peliculas
  MapPair *pair = map_first(peliPorID);
  
  // Itera sobre todas las películas en el mapa
  
  while (pair != NULL) {
    Film *peli = pair->value;
    decadaPeli = (peli->year / 10) *10;
    //Si la decada es igual a la buscada, se imprime la peli
    if (decada == decadaPeli) {
      imprimirPelicula(peli);
      contador++;
    }
    // Avanza al siguiente par clave-valor en el mapa
    pair = map_next(peliPorID);
  }
  //Si no se encontraron películas en la decada, se informa al usuario
  if (contador == 0) printf("No se encontraron películas en la década: %d",decada);
  //Si se encontraron, se imprime cuantas
  else printf("Se encontraron %d películas en la década: %d",contador,decada);
}

void buscarPorCalificaciones(Map *peliPorID){
  //Se le pide al usuario que ingrese la calificación mínima
  printf( "Ingrese la calificacion minima: ");
  //Reserva de memoria para las variables que vamos a utilizar
  float calificacionMin, calificacionMax,calificacionPeli;
  scanf("%f",&calificacionMin);
  //Se le pide al usuario que ingrese la calificación máxima
  printf( "Ingrese la calificacion maxima: ");
  scanf("%f",&calificacionMax);
  //Manejo de error por si la calificación máxima es menor que la mínima
  if(calificacionMax < calificacionMin){
    printf("Error: La calificacion maxima debe ser mayor o igual a la minima");
    return;
  }
  //Nos ubicamos al inicio del mapa para iterar sobre todas las peliculas
  MapPair *pair = map_first(peliPorID);
  Film *peli;
  int cont = 0;
  while(pair != NULL){
    peli = pair->value;
    calificacionPeli = peli->calificacion;
    //Si la peli cumple con el rango de calificación, se imprime
    if(peli->calificacion >= calificacionMin && peli->calificacion <= calificacionMax){
      imprimirPelicula(peli);
      cont++;
    }
    pair = map_next(peliPorID);
  }

  if (cont == 0) printf("No se encontraron peliculas en ese rango de calificacion\n");
  else printf("Se encontraron %d peliculas en ese rango de calificacion\n",cont);
}

void buscarPorDecadaGenero(Map *peliPorID, Map *peliPorGenero) {
  //Reserva de memoria para las variables que vamos a utilizar
  int decada,decadaPeli,contador = 0; 
  char genero[100];

  // Limpiar el buffer de entrada
  int c;
  while ((c = getchar()) != '\n' && c != EOF);

  // Solicita al usuario el año de inicio de la década
  printf("Ingrese el año de inicio de la década: ");
  scanf("%d", &decada); // Lee la década del teclado

  // Solicita al usuario el género de la película
  printf("Ingrese el género de la película: ");
  scanf("%s", genero); // Lee el género del teclado

  // Elimina el carácter de nueva línea
  size_t len = strlen(genero);
  if (len > 0 && genero[len - 1] == '\n') {
      genero[len - 1] = '\0';
  }

  decada = (decada / 10) * 10;

  // Itera primero sobre el mapa de películas por género para buscar películas que coincidan con la década y el género
  MapPair *pair = map_first(peliPorGenero);

  while (pair != NULL) {
    Film *peli = pair->value;
    // Busca el género en la lista de géneros de la película
    for (char *gen = list_first(peli->genres); gen != NULL; gen = list_next(peli->genres)) {
// Compara el nombre del género con el género actual en la lista
      if (strcasecmp(genero, gen) == 0) {
        // Comprueba si la película está en la década buscada
        decadaPeli = (peli->year / 10) * 10;
        if (decadaPeli == decada) {
          // Si todo coincidió, se mostrará la pelicula
          imprimirPelicula(peli);
          contador++; // Marca que se encontraron películas
        }
      }
    }
    pair = map_next(peliPorGenero); // Avanza al siguiente par en el mapa
  }

  //Se imprime la cantidad de peliculas encontradas
  if (contador != 0) printf("Se encontraron %d peliculas",contador);
  // Si no se encontraron películas, informa al usuario
  else printf("No se encontraron películas");
  printf(" en la decada de %d y del genero %s\n", decada, genero);
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
      buscarPorDecadaGenero(peliPorID,peliPorGenero);
      break;
    default:
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}