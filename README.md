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
Esta Opcion permite buscar peliculas por su director. Solo es necesario ingresar el nombre del director a buscar y mostrará todas las peliculas del director.

## 4) Buscar por Genero
Esta Opcion permite buscar peliculas por su género. Solo es necesario ingresar género a buscar y mostrará todas las peliculas de ese género. Como observación, el genero debe ser ingresado en inglés, por ejemplo "Fantasy".

## 5)Buscar por Década
Esta opción permite ingresar el primer año de una decada y mostrar todas las peliculas de esta última. Se contempla también el caso de si un usuario ingresa el un año el cual no sea inicio de una decada (por ejemplo 1982), para esos casos se mostrarán las peliculas de 1980.

## 6)Buscar por Calificación
Esta opción permite buscar todas las peliculas en un rango de calificaciones estipulado por el usuario, al usuario se le pedirá primero que ingrese una calificación minima y luego una máxima y la función mostrara todas las peliculas encontradas en ese rango.

## 7)Buscar por Década y Género
Esta función nos permite buscar tanto por la decada de la pelicula como por el genero, primero se preguntará por la decada y luego por el genero que deseemos buscar. Al igual que en la función de buscar solo por década si se ingresa 1982 se buscará la decada de 1980, y al buscar por género, se debe ingresar el nombre del género en inglés, por ejemplo "Crime".

## 8)Salida
Esta opción es la que nos permitirá salir del programa éxitosamente.
