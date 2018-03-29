# ESP32_FreeRtos
ESP32 Arduino: Creando una tarea FreeRTOS

# Sistemas embebidos 
Los sistemas embebidos son plataformas con recursos muy limitados en comparación con una PC. Es por esto que generalmente no tienen un sistema operativo completo, sino sólo el subconjunto de estos que pueden manejar ventajosamente. En algunos casos el OS no es un programa en sí mismo sino que es un conjunto de funciones que se ejecutan solo en momentos determinados del programa.

# Qué es un OS?
* Es un conjunto de programas que ayuda al programador de
aplicaciones a gestionar los recursos de hardware disponibles, entre
ellos el tiempo del procesador y la memoria.
* La gestión del tiempo del procesador permite al programador de
aplicaciones escribir múltiples subprogramas como si cada uno fuera
el único que utiliza la CPU.
* Una parte del OS se encarga de asignar tiempo de ejecución a
todos los programas que tiene cargados en base a un juego de
reglas conocido de antemano. A estos subprogramas se los llama
tareas.
* Con esto se logra la ilusión de que múltiples programas se ejecutan
simultáneamente, aunque en realidad sólo pueden hacerlo de a uno
a la vez (en sistemas con un sólo núcleo, como es el caso general
de los sistemas embebidos).

# Cómo se administra el tiempo del CPU?
* El encargado de esta gestión es un componente del
OS llamado scheduler o programador de tareas. Su
función es determinar qué tarea debe estar en
ejecución a cada momento.
* Ante la ocurrencia de ciertos eventos revisa si la
tarea en ejecución debe reemplazarse por alguna
otra tarea. A este reemplazo se le llama cambio de
contexto de ejecución.

# Más contexto
* Se llama contexto de ejecución al conjunto de
recursos que identifican el estado de ejecución de
una tarea:
* IP (instruction pointer)
* SP (stack pointer)
* Registros del CPU
* Contenido de la pila en uso

# Cambiemos de contexto
* Cuando el scheduler determina que debe cambiarse
el contexto de ejecución, invoca a otro componente
del OS llamado dispatcher para que guarde el
contexto completo de la tarea actual y lo reemplace
por el de la tarea entrante.
* Por esta razón debe reservarse un bloque de
memoria de datos para cada tarea. Esto limita la
cantidad de tareas simultáneas del sistema (pueden
sin embargo eliminarse y crearse nuevas tareas en
tiempo de ejecución).
* Estos cambios de contexto se realizan de forma
transparente para la tarea, no agregan trabajo al
programador. Cuando la tarea retoma su ejecución
no muestra ningún síntoma de haberla pausado
alguna vez.
* Los OS trabajan en dos modos:
* En modo cooperativo, estos cambios solo ocurren
cuando la tarea en ejecución relega voluntariamente el
uso del CPU.
* En cambio en modo preemptive el scheduler tiene la
facultad de remover una tarea sin el consentimiento de la
misma.
* En este caso debe preveerse que algunas operaciones no
deben ser interrumpidas, a estos pasajes del programa se los
llama secciones críticas. Los OS permiten inhibir de este modo
8 RTOS - Sistemas Embebidos 2010 - FI- los cambios de contexto cuando es necesario.

El código de configuración y bucle
Comenzaremos nuestra función de configuración abriendo una conexión en serie, para poder obtener el resultado de nuestro programa de prueba. Esta será nuestra función Arduino regular.

1
2
Serial.begin(112500);
delay(1000);
Luego, crearemos las tareas, con una llamada a la  función xTaskCreate . Los argumentos para esta función son los siguientes [1]:

TaskCode : en este argumento, necesitamos pasar un puntero a la función que implementará la tarea. Crearemos dos funciones, TaskOne y TaskTwo , que definiremos más adelante y las pasaremos en este argumento.

TaskName : el nombre de la tarea, en una cadena. Usaremos "TaskOne" y "TaskTwo".

StackDepth : el tamaño de la pila de la tarea, especificado como la cantidad de variables que puede contener (no la cantidad de bytes). No hay una manera simple de determinar el tamaño de la tarea [2], aunque se pueden hacer algunos cálculos. En este simple ejemplo, aprobaremos un valor que sea lo suficientemente grande.

Parámetro : apunta a un parámetro que la función de tarea puede recibir. Necesita ser del tipo (void *) [2]. En este caso, por simplicidad del código, no lo usaremos, por lo que pasamos NULL en el argumento.

Prioridad: prioridad de la tarea. Crearemos ambas tareas con la misma prioridad.

TaskHandle: devuelve un descriptor de contexto que se puede utilizar para la referencia posterior de la tarea en llamadas a funciones (por ejemplo, para eliminar una tarea o cambiar su prioridad) [2]. Además, para este simple ejemplo, no vamos a usarlo, por lo que será NULL.

Esta función devuelve  pdPASS en caso de éxito o un código de error que se puede ver aquí  [1]. Por ahora, asumiremos que las tareas se crearán sin ningún problema, por lo que no vamos a hacer ninguna comprobación de errores. Naturalmente, para una aplicación de escenario de caso real, tendríamos que hacer eso para confirmar que se crearon las tareas.

Por lo tanto, compruebe abajo la función de configuración completa, ya con las llamadas cree las dos tareas diferentes.
