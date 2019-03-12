## Fecha: Enero 26 a Marzo 3: Semana 1 -Proyecto2

- Creación de repositorio: https://github.com/hamax97/motion-vector 
- Definición entorno de desarrollo: Lenguaje C con editor de texto Emacs
- Entendimiento del problema Vectores de Movimiento
- Definición y planteamiento de algoritmo serial. Link: https://github.com/hamax97/motion-vector/blob/master/serial-algorithm.md 
- Lectura de archivo .bmp. Link: https://github.com/hamax97/motion-vector/tree/bmp-reader 
- Implementación algoritmo serial: Link: https://github.com/hamax97/motion-vector/tree/serial 
- Pruebas del algoritmo serial en el cluster (DCA)

## Fecha: Marzo 5 a 10: Semana 2 - Proyecto2
- Paralelización de algoritmo serial con hilos, librería OpenMP. Link: https://github.com/hamax97/motion-vector/tree/openmp
- Pruebas del algoritmo con hilos en el cluster (DCA)

## Fecha: Marzo 12 a 17: Semana 2 - Proyecto2
- Paralelización distribuida de algoritmo con hilos, libreria OpenMPI. Link: https://github.com/hamax97/motion-vector/tree/mpi
- Todos los cómputos funcionan distribuidamente, pero tenemos un problema al hacer Gather de los resultados, no llegan correctamente.