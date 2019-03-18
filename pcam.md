# ¿Cómo aplicamos la metodología PCAM?

## Particionado
Para el **particionado de datos** lo que hacemos es dividir la imagen en la cantidad de nodos disponibles, de la siguiente manera:

- Sea p número de nodos, h la altura de la imagen, w el ancho de la imagen, x la altura de la matriz de macro bloques, k es equivalente al número de macro bloques por proceso, y r es equivalente al número de macro bloques que sobran, es decir, posiblemente el x no sea exactamente divisible entre el número de nodos, por esto la necesidad de r.

     > x = h / 16

     > k = x / p

     > r = x % p

- Así, tenemos que, el nodo maestro se llevará ((k + r) * 16) * w píxeles, y los nodos esclavos (k * 16) * w píxeles. Lo que esto garantiza es que cada proceso tendrá una matriz que es divisible por 16 en su altura (*scatter*).

- Luego de esto, se envía la imagen comprimida completamente a cada nodo (*broadcast*), con el objetivo de buscar esa imagen dividida previamente en la totalidad de la imagen comprimida.

- Esto puede verse mejor en [particionamiento]()

Para el **particionado de los cálculos** tenemos que cada proceso será ejecutado por cada nodo, es decir, si hay 3 nodos, entonces tendremos 3 procesos. En cada proceso se crearán tantos hilos como procesadores hayan en el nodo, es decir si el nodo tiene 24 procesadores, entonces se crearán 24 hilos en cada nodo. Cada uno de estos hilos es encargado de procesar una cantidad de macro bloques, es decir, buscarlos en la imagen comprimida y guarda su posición.

## Comunicaciones
1. El proceso maestro lee las imágenes necesarias (original y comprimida), hace el particionamiento explicado anteriormente y envía las partes de la imagen original a cada proceso esclavo, quedándose con la primera parte de la imagen él mismo, en este paso, el proceso maestro se bloquea hasta que todos los esclavos reciban su parte de la imagen. Por consiguiente se hace un *broadcast* de la imagen comprimida a todos los procesos esclavos.

2. Cada proceso hace sus cómputos, al terminar, convierte sus resultados de una matriz a un arreglo, para hacer fácil el envío de datos.

3. Este arreglo con los resultados es enviado al proceso maestro haciendo un *gather*, es decir, cada proceso se bloquea hasta que el proceso maestro finalize sus cómputos, en caso de que no haya terminado. Por consiguiente imprimiendo el arreglo resultante en forma de matriz.

Este tipo de comunicación es *bloqueante / bufereada*, es decir, cada paso de mensajes es posible debido a llamadas bloqueantes y el uso de bufers.

## Aglomeración
Como ya se explicó, se crean tantas tareas como número de nodos hayan. Cada tarea es encargada de buscar una parte de la imagen original en la imagen comprimida. A su vez, cada tarea se divide en *n* subtareas, siendo *n* el número de procesadores de cada nodo. Cada subtarea es encargada de procesar *y* macro bloques, es decir, buscarlos en la imagen comprimida y guardar su posicón. El número de macro bloques que procesa cada tarea depende del tamaño de la sub imagen que tiene la tarea y también de cómo la directiva *#pragma omp for* divida las iteraciones sobre las subtareas.

## Mapeo

Cada nodo disponible se usa al máximo. Si cada nodo tiene 24 cores disponibles, entonces al programa se le debe indicar que use *n\*24* cores, donde *n* es equivalente al número de nodos. Junto con esto, la forma de repartir la imagen original en cada nodo, hace que la comunicación entre procesos sea mínima, es decir, cada nodo es encargado de *recibir* una única vez y procesar su pedazo de imagen, por consiguiente *enviar* una única vez sus resultados. Adicional a esto, los hilos que se crean para cada tarea no necesitan comunicarse entre sí, puesto que cada hilo es encargado de procesar un macro bloque que es independiente de todos los demás.

- Esto puede verse mejor en [mapeo]()

En conclusión, la forma de dividir los datos y el procesamiento es una forma "eficiente", puesto que se usan todos los recursos computacionales disponibles, y además se minimizan al máximo las comunicaciones entre procesos.
