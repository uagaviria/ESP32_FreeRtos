# ESP32_FreeRtos
ESP32 Arduino: Creando una tarea FreeRTOS

# Sistemas embebidos 
Los sistemas embebidos son plataformas con recursos muy limitados en comparaci�n con una PC. Es por esto que generalmente no tienen un sistema operativo completo, sino s�lo el subconjunto de estos que pueden manejar ventajosamente. En algunos casos el OS no es un programa en s� mismo sino que es un conjunto de funciones que se ejecutan solo en momentos determinados del programa.

# Qu� es un OS?
* Es un conjunto de programas que ayuda al programador de
aplicaciones a gestionar los recursos de hardware disponibles, entre
ellos el tiempo del procesador y la memoria.
* La gesti�n del tiempo del procesador permite al programador de
aplicaciones escribir m�ltiples subprogramas como si cada uno fuera
el �nico que utiliza la CPU.
* Una parte del OS se encarga de asignar tiempo de ejecuci�n a
todos los programas que tiene cargados en base a un juego de
reglas conocido de antemano. A estos subprogramas se los llama
tareas.
* Con esto se logra la ilusi�n de que m�ltiples programas se ejecutan
simult�neamente, aunque en realidad s�lo pueden hacerlo de a uno
a la vez (en sistemas con un s�lo n�cleo, como es el caso general
de los sistemas embebidos).

# C�mo se administra el tiempo del CPU?
* El encargado de esta gesti�n es un componente del
OS llamado scheduler o programador de tareas. Su
funci�n es determinar qu� tarea debe estar en
ejecuci�n a cada momento.
* Ante la ocurrencia de ciertos eventos revisa si la
tarea en ejecuci�n debe reemplazarse por alguna
otra tarea. A este reemplazo se le llama cambio de
contexto de ejecuci�n.

# M�s contexto
* Se llama contexto de ejecuci�n al conjunto de
recursos que identifican el estado de ejecuci�n de
una tarea:
* IP (instruction pointer)
* SP (stack pointer)
* Registros del CPU
* Contenido de la pila en uso

# Cambiemos de contexto
* Cuando el scheduler determina que debe cambiarse
el contexto de ejecuci�n, invoca a otro componente
del OS llamado dispatcher para que guarde el
contexto completo de la tarea actual y lo reemplace
por el de la tarea entrante.
* Por esta raz�n debe reservarse un bloque de
memoria de datos para cada tarea. Esto limita la
cantidad de tareas simult�neas del sistema (pueden
sin embargo eliminarse y crearse nuevas tareas en
tiempo de ejecuci�n).
* Estos cambios de contexto se realizan de forma
transparente para la tarea, no agregan trabajo al
programador. Cuando la tarea retoma su ejecuci�n
no muestra ning�n s�ntoma de haberla pausado
alguna vez.
* Los OS trabajan en dos modos:
* En modo cooperativo, estos cambios solo ocurren
cuando la tarea en ejecuci�n relega voluntariamente el
uso del CPU.
* En cambio en modo preemptive el scheduler tiene la
facultad de remover una tarea sin el consentimiento de la
misma.
* En este caso debe preveerse que algunas operaciones no
deben ser interrumpidas, a estos pasajes del programa se los
llama secciones cr�ticas. Los OS permiten inhibir de este modo
8 RTOS - Sistemas Embebidos 2010 - FI- los cambios de contexto cuando es necesario.

El c�digo de configuraci�n y bucle
Comenzaremos nuestra funci�n de configuraci�n abriendo una conexi�n en serie, para poder obtener el resultado de nuestro programa de prueba. Esta ser� nuestra funci�n Arduino regular.

1
2
Serial.begin(112500);
delay(1000);
Luego, crearemos las tareas, con una llamada a la  funci�n xTaskCreate . Los argumentos para esta funci�n son los siguientes [1]:

TaskCode : en este argumento, necesitamos pasar un puntero a la funci�n que implementar� la tarea. Crearemos dos funciones, TaskOne y TaskTwo , que definiremos m�s adelante y las pasaremos en este argumento.

TaskName : el nombre de la tarea, en una cadena. Usaremos "TaskOne" y "TaskTwo".

StackDepth : el tama�o de la pila de la tarea, especificado como la cantidad de variables que puede contener (no la cantidad de bytes). No hay una manera simple de determinar el tama�o de la tarea [2], aunque se pueden hacer algunos c�lculos. En este simple ejemplo, aprobaremos un valor que sea lo suficientemente grande.

Par�metro : apunta a un par�metro que la funci�n de tarea puede recibir. Necesita ser del tipo (void *) [2]. En este caso, por simplicidad del c�digo, no lo usaremos, por lo que pasamos NULL en el argumento.

Prioridad: prioridad de la tarea. Crearemos ambas tareas con la misma prioridad.

TaskHandle: devuelve un descriptor de contexto que se puede utilizar para la referencia posterior de la tarea en llamadas a funciones (por ejemplo, para eliminar una tarea o cambiar su prioridad) [2]. Adem�s, para este simple ejemplo, no vamos a usarlo, por lo que ser� NULL.

Esta funci�n devuelve  pdPASS en caso de �xito o un c�digo de error que se puede ver aqu�  [1]. Por ahora, asumiremos que las tareas se crear�n sin ning�n problema, por lo que no vamos a hacer ninguna comprobaci�n de errores. Naturalmente, para una aplicaci�n de escenario de caso real, tendr�amos que hacer eso para confirmar que se crearon las tareas.

Por lo tanto, compruebe abajo la funci�n de configuraci�n completa, ya con las llamadas cree las dos tareas diferentes.
