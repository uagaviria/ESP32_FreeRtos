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

# �Qu� es un RTOS?
* Un RTOS es un sistema operativo de tiempo real.
* Esto significa que hace lo mismo que un OS com�n
pero adem�s me da herramientas para que los
programas de aplicaci�n puedan cumplir
compromisos temporales definidos por el
programador. El objetivo del mismo es diferente de
un OS convencional.
* Un RTOS se emplea cuando hay que administrar
varias tareas simult�neas con plazos de tiempo
estrictos.

# C�mo se define un sistema de tiempo real?
* Un STR est� definido por:
* Los eventos externos que debe atender.
* La respuesta que debe producir ante estos eventos.
* Los requerimientos de temporizaci�n de esas respuestas.
* Los STR suaves se suelen dise�ar como si fueran
STR duros. Luego se da mayor prioridad a la
atenci�n de los eventos con compromisos
temporales estrictos.

# �Por qu� usar un RTOS?
* Para cumplir con compromisos temporales estrictos
* El RTOS ofrece funcionalidad para asegurar que una vez ocurrido
un evento, la respuesta ocurra dentro de un tiempo acotado. Es
importante aclarar que esto no lo hace por s� solo sino que brinda
al programador herramientas para hacerlo de manera m�s
sencilla que si no hubiera un RTOS.
* Esto implica que una aplicaci�n mal dise�ada puede fallar en la atenci�n
de eventos a�n cuando se use un RTOS.
* Para no tener que manejar el tiempo �a mano�
* El RTOS absorbe el manejo de temporizadores y esperas, de
modo que hace m�s facil al programador el manejo del tiempo.
* Tarea Idle
* Cuando ninguna de las tareas requiere del procesador, el sistema
ejecuta una tarea llamada idle u ociosa. Esto me permite
f�cilmente contabilizar el nivel de ocupaci�n del CPU, poner al
mismo en modo de bajo consumo o correr cualquier tarea que
pudiera ser de utilidad para el sistema cuando no debe atender
ninguno de sus eventos.
* Multitarea
* Simplifica sobremanera la programaci�n de sistemas con
varias tareas.
* Escalabalidad
* Al tener ejecuci�n concurrente de tareas se pueden
agregar las que haga falta, teniendo el �nico cuidado de
insertarlas correctamente en el esquema de ejecuci�n del
sistema.
* Mayor reutilizabilidad del c�digo
* Si las tareas se dise�an bien (con pocas o ninguna
dependencia) es m�s f�cil incorporarlas a otras
aplicaciones.

# La letra chica 1
* Se gasta tiempo del CPU en determinar en todo
momento qu� tarea debe estar corriendo. Si el
sistema debe manejar eventos que ocurren
demasiado r�pido tal vez no haya tiempo para esto.
* Se gasta tiempo del CPU cada vez que debe
cambiarse la tarea en ejecuci�n.
* Se gasta memoria de c�digo para implementar la
funcionalidad del RTOS.
* Se gasta memoria de datos en mantener una pila y
un TCB (bloque de control de tarea) por cada tarea
* El tama�o de estas pilas suele ser configurable POR
TAREA, lo cual mitiga este impacto.

* Finalmente, debe hacerse un an�lisis de tiempos,
eventos y respuestas m�s cuidadoso. Al usar un
RTOS ya no es el programador qui�n decide cu�ndo
ejecutar cada tarea, sino el scheduler. Cometer un
error en el uso de las reglas de ejecuci�n de tareas
puede llevar a que los eventos se procesen fuera del
tiempo especificado o que no se procesen en lo
absoluto.
* A la luz de los beneficios mencionados, en el caso
de que la plataforma de hardware lo permita y el
programador est� capacitado no hay razones para
no usar un RTOS.

# Tipo de tareas que vamos a implementar en
una aplicaci�n de tiempo real
* Tareas peri�dicas
* Atienden eventos que ocurren constantemente y a una
frecuencia determinada. P. ej, destellar un led.
* Tareas aperi�dicas
* Atienden eventos que no se sabe cu�ndo van a darse.
Estas tareas est�n inactivas (bloqueadas) hasta que no
ocurre el evento de inter�s. P. ej, una parada de
emergencia.
* Tareas de procesamiento cont�nuo
* Son tareas que trabajan en r�gimen permanente. P. ej,
muestrear un buffer de recepci�n en espera de datos
para procesar.
* Estas tareas deben tener prioridad menor que las otras,
ya que en caso contrario podr�an impedir su ejecuci�n.

# Caso de estudio
* Tarea de la primera pr�ctica
* Destellar un led a una frecuencia determinada.
* Muestrear un pulsador y reflejar su estado en un led
(tarea peri�dica).
* Contar pulsaciones y destellar un led esa misma cantidad
de veces (tarea aperi�dica).

#Por qu� FreeRTOS?

# Es de c�digo abierto

* No hay costo de implementaci�n.
* El c�digo est� ampliamente comentado, es muy
sencillo verificar c�mo hace su trabajo.
* Es relativemente sencillo de portar a plataformas
nuevas.

