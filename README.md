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

# Cómo se define un sistema de tiempo real?
* Un STR está definido por:
* Los eventos externos que debe atender.
* La respuesta que debe producir ante estos eventos.
* Los requerimientos de temporización de esas respuestas.
* Los STR suaves se suelen diseñar como si fueran
STR duros. Luego se da mayor prioridad a la
atención de los eventos con compromisos
temporales estrictos.

# ¿Por qué usar un RTOS?
* Para cumplir con compromisos temporales estrictos
* El RTOS ofrece funcionalidad para asegurar que una vez ocurrido
un evento, la respuesta ocurra dentro de un tiempo acotado. Es
importante aclarar que esto no lo hace por sí solo sino que brinda
al programador herramientas para hacerlo de manera más
sencilla que si no hubiera un RTOS.
* Esto implica que una aplicación mal diseñada puede fallar en la atención
de eventos aún cuando se use un RTOS.
* Para no tener que manejar el tiempo “a mano”
* El RTOS absorbe el manejo de temporizadores y esperas, de
modo que hace más facil al programador el manejo del tiempo.
* Tarea Idle
* Cuando ninguna de las tareas requiere del procesador, el sistema
ejecuta una tarea llamada idle u ociosa. Esto me permite
fácilmente contabilizar el nivel de ocupación del CPU, poner al
mismo en modo de bajo consumo o correr cualquier tarea que
pudiera ser de utilidad para el sistema cuando no debe atender
ninguno de sus eventos.
* Multitarea
* Simplifica sobremanera la programación de sistemas con
varias tareas.
* Escalabalidad
* Al tener ejecución concurrente de tareas se pueden
agregar las que haga falta, teniendo el único cuidado de
insertarlas correctamente en el esquema de ejecución del
sistema.
* Mayor reutilizabilidad del código
* Si las tareas se diseñan bien (con pocas o ninguna
dependencia) es más fácil incorporarlas a otras
aplicaciones.

# La letra chica 1
* Se gasta tiempo del CPU en determinar en todo
momento qué tarea debe estar corriendo. Si el
sistema debe manejar eventos que ocurren
demasiado rápido tal vez no haya tiempo para esto.
* Se gasta tiempo del CPU cada vez que debe
cambiarse la tarea en ejecución.
* Se gasta memoria de código para implementar la
funcionalidad del RTOS.
* Se gasta memoria de datos en mantener una pila y
un TCB (bloque de control de tarea) por cada tarea
* El tamaño de estas pilas suele ser configurable POR
TAREA, lo cual mitiga este impacto.

* Finalmente, debe hacerse un análisis de tiempos,
eventos y respuestas más cuidadoso. Al usar un
RTOS ya no es el programador quién decide cuándo
ejecutar cada tarea, sino el scheduler. Cometer un
error en el uso de las reglas de ejecución de tareas
puede llevar a que los eventos se procesen fuera del
tiempo especificado o que no se procesen en lo
absoluto.
* A la luz de los beneficios mencionados, en el caso
de que la plataforma de hardware lo permita y el
programador esté capacitado no hay razones para
no usar un RTOS.

# Tipo de tareas que vamos a implementar en
una aplicación de tiempo real
* Tareas periódicas
* Atienden eventos que ocurren constantemente y a una
frecuencia determinada. P. ej, destellar un led.
* Tareas aperiódicas
* Atienden eventos que no se sabe cuándo van a darse.
Estas tareas están inactivas (bloqueadas) hasta que no
ocurre el evento de interés. P. ej, una parada de
emergencia.
* Tareas de procesamiento contínuo
* Son tareas que trabajan en régimen permanente. P. ej,
muestrear un buffer de recepción en espera de datos
para procesar.
* Estas tareas deben tener prioridad menor que las otras,
ya que en caso contrario podrían impedir su ejecución.

# Caso de estudio
* Tarea de la primera práctica
* Destellar un led a una frecuencia determinada.
* Muestrear un pulsador y reflejar su estado en un led
(tarea periódica).
* Contar pulsaciones y destellar un led esa misma cantidad
de veces (tarea aperiódica).

#Por qué FreeRTOS?

# Es de código abierto

* No hay costo de implementación.
* El código está ampliamente comentado, es muy
sencillo verificar cómo hace su trabajo.
* Es relativemente sencillo de portar a plataformas
nuevas.

