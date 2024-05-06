En este *repl* puedes encontrar varios ejemplos que te pueden ayudar con las tareas de estructura.

## Código de Ejemplo (tarea2)
Para ejecutar el ejemplo tarea2 primero debemos compilar (en la carpeta raíz)
````
gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2
````
Luego para ejecutarlo, debe ingresar el siguiente comando:
````
./tarea2
````

## Menu de funcionalidades
Al ejecutarlo se deberia de abrir el siguiente menu:
````
========================================
     Base de Datos de Películas
========================================
1) Cargar Películas
2) Buscar por id
3) Buscar por director
4) Buscar por género
5) Buscar por década
6) Buscar por rango de calificaciones
7) Buscar por década y género
8) Salir
Ingrese su opción:
````
## 1) Cargar Películas
Al seleccionar esta opcion, se empiezan a cargar todas las peliculas y automaticamente empezara a mostrar todas las peliculas que se estan cargando exitosamente.

## 2) Buscar por id
Esta opcion permite al usuario buscar la pelicula mediante el ID, ingrese el id y buscara la pelicula.
````
Ingrese su opción: 2
Ingrese el id de la película: tt0119567
Título: The Lost World: Jurassic Park, Año: 1997
Presione una tecla para continuar...
````
## 3) Buscar por Director
Esta Opcion permite buscar peliculas por su director. En el mapa de los directores, se usa el nombre del director como llave para encontrar la lista de peliculas del director ingresado. <br>
Por razones que desconozco, esta opcion no funciono

## 4) Buscar por Genero
Al ingresar un genero de una pelicula, esta la podra en mayusculas y luego busca en el mapa una entrada correspondiente a ese género.

## 5)Buscar por decada
Esta opcion hace que al buscar el año de inicio de una década mueste todas las películas en el mapa que se estrenaron durante esa década.
