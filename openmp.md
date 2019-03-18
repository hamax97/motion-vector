# Descripción de la implementación en OpenMP.

1. Nuestra implementación serial consiste en buscar macro blocque por macro bloque de la imagen original, o sin comprimir en la imagen desplazada o comprimida.

2. Al ser encontrado un match (suma de diferencias igual a 0), o una suma de diferencias mínima, se guarda la posición de este macro bloque y se coloca en una matriz.

A grandes rasgos, esta es nuestra implementación serial. Por lo tanto, nuestra implementación en OpenMP consiste en:

1. Cada macro bloque de la imagen original va a ser buscado por un hilo diferente en la imagen comprimida.

2. Cuando un hilo encuentra la posición adecuada para este macro bloque en la imagen comprimida, la guarda y luego pueden suceder dos cosas.

2.1. Si no hay suficientes hilos como para buscar cada macro bloque por un hilo diferente, el hilo que ya encontro una posición se le asigna otro macro bloque. Cuántos macro bloques se asignan a cada hilo?, esto dependerá de la cantidad de hilos que sean definidos en la variable de ambiente OMP_NUM_THREADS y tambien de OpenMP en su directiva #pragma omp for, es decir, depende de cómo OpenMP divida la cantidad de iteraciones sobre los hilos.

2.2. Si hay suficientes hilos como para buscar cada macro bloque por un hilo diferente, el hilo que ya encontró una posición entonces morirá, puesto que no es necesario que busque más macro bloques.