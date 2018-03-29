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

# ¿Qué es un RTOS?
* Un RTOS es un sistema operativo de tiempo real.
* Esto significa que hace lo mismo que un OS común
pero además me da herramientas para que los
programas de aplicación puedan cumplir
compromisos temporales definidos por el
programador. El objetivo del mismo es diferente de
un OS convencional.
* Un RTOS se emplea cuando hay que administrar
varias tareas simultáneas con plazos de tiempo
estrictos.

#Cómo se define un sistema de tiempo real?
* Un STR está definido por:
* Los eventos externos que debe atender.
* La respuesta que debe producir ante estos eventos.
* Los requerimientos de temporización de esas respuestas.
* Los STR suaves se suelen diseñar como si fueran
STR duros. Luego se da mayor prioridad a la
atención de los eventos con compromisos
temporales estrictos.


