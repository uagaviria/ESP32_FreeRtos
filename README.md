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

# La letra chica 
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

# Es f�cil de implementar

* Hay mucha documentaci�n disponible.
* Libro de texto escrito por uno de los dise�adores del
sistema.
* Incluye una demostraci�n de sus funciones para cada
plataforma en la que est� soportado.
* Nutrida comunidad de usuarios 
* Hay una opci�n con soporte comercial.
* Hay una opci�n con certificaci�n SIL-3 para
sistemas de misi�n critica.

# Est� pensado para microcontroladores

* Est� escrito mayormente en C, salvo las partes
espec�ficas de cada plataforma.
* Es liviano en tama�o de c�digo y en uso de
memoria RAM.

# Primera mirada a FreeRTOS

* Es un mini kernel de tiempo real que puede trabajar en modos
cooperativo, preemptive o mixto.
* Mini kernel significa que provee los servicios m�nimos e
indispensables
* Permite compilar solo las funciones que se vayan a usar,
acotando as� su impacto en la memoria de c�digo.
* Se puede definir y acotar en tiempo de compilaci�n el uso de
memoria de datos por parte del sistema.
* Ofrece funciones de temporizaci�n, de comunicaci�n entre
tareas, de sincronizaci�n entre tareas e interrupciones, y de
definici�n de secciones cr�ticas

# Algunas opciones de configuraci�n

* FreeRTOSConfig.h
* configUSE_PREEMPTION
* configCPU_CLOCK_HZ
* configTICK_RATE_HZ
* configMAX_PRIORITIES
* configMINIMAL_STACK_SIZE
* configTOTAL_HEAP_SIZE
* configUSE_USE_MUTEXES
* configUSE_CO_ROUTINES
* #define INCLUDE_vTaskDelete 1

# Tareas en FreeRTOS

# C�mo es una tarea en FreeRTOS?
* Citando a Richard Barry: �Las tareas se
implementan con funciones de C. Lo �nico especial
que tienen es su prototipo, que debe devolver void y
recibir un puntero a void como par�metro�.
* void vTareaEjemplo (void *parametros);
* Cada tarea tiene su propio punto de entrada,
secci�n de inicializaci�n y lazo de control.
* Las funciones de tarea (en adelante, tareas) en FreeRTOS NO DEBEN RETORNAR BAJO NING�N
CONCEPTO. No deben incluir un return ni
ejecutarse hasta la llave de cierre
* Si una tarea deja de ser necesaria, puede elimin�rsela
expl�citamente. 

# C�mo es una tarea en FreeRTOS?
* Las tareas tienen una prioridad de ejecuci�n. 0 es la
menor prioridad.
* Se recomienda usar referencias a tskIDLE_PRIORITY (+1, +2,
etc)
* No hay l�mites a la cantidad de prioridades del sistema, pero se
gasta RAM por cada una. Usar con cuidado.
* Se pueden crear m�ltiples instancias de una misma
funci�n de tarea
* Estas instancias pueden recibir un par�metro que las
caracterice
* IMPORTANTE: Cuando una tarea deja de estar en
ejecuci�n, las referencias a variables alojadas en su pila
dejan de ser v�lidas.
* Al ser funciones de C, se aplican las mismas reglas de
visibilidad de variables => se puede compartir memoria
entre las tareas usando variables globales.
* En este caso se debe cuidar el acceso a este recurso
compartido. 

# Ejemplo 1: 
Dos tareas de igual prioridad se apropian
del CPU, pero son administradas por el scheduler.
Es increible cuando ejecute este ejemplo en consola, sabiendo por mi experiencia que cada for next siempre se ejecuta por separado y aqui las dos tareas tienen prioridad. como si estuvieran en paralelo.
# Importante: 
Como el n�cleo arduino del ESP32, se ejecuta sobre FreeRTOS y puede llamar a las API directamente en el boceto Arduino, no es necesaria una biblioteca adicional.

# C�digo

```C++
#include <Arduino.h>
void setup() {

  Serial.begin(112500);
  delay(1000);

  xTaskCreate(Tarea1,"Tarea1",10000,NULL,1,NULL);
  xTaskCreate(Tarea2,"Tarea2",10000,NULL,1,NULL);

}

void loop() {
  delay(1000);
}

void Tarea1( void * parameter )
{

    for( int i = 0;i<10;i++ ){

        Serial.println("Hola desde la tarea 1");
        delay(1000);
    }

    Serial.println("Finalizando tarea 1");
    vTaskDelete( NULL );

}

void Tarea2( void * parameter)
{

    for( int i = 0;i<10;i++ ){

        Serial.println("Hola desde la tarea 2");
        delay(1000);
    }
    Serial.println("Finalizando tarea 2");
    vTaskDelete( NULL );
}
```

El siguiente c�digo nos muestra que podemos correr nuestro programa en cualquiera de los dos nucleos del ESP32. y correr las tareas en los tiempos que mas nos convenga. los delay pueden ser modificados para esto. si corres el programa en consola te daras cuenta de lo que hablo.

# Recursos de software # 1 vTaskDelay()
* void vTaskDelay( portTickType xTicksToDelay );
* Produce una demora en la tarea que la llama. Cede el
control del CPU mientras este tiempo no expira.
* INCLUDE_vTaskDelay must be defined as 1 for this
function to be available.
* Uso:
vTaskDelay (500 / portTICK_RATE_MS);

# Recursos de software 2 vTaskDelayUntil()
* void vTaskDelayUntil (portTickType * pxPreviousWakeTime, portTickType xTimeIncrement);

Mientras que vTaskDelay () especifica un tiempo de activaci�n relativo al momento en que se llama a la funci�n, vTaskDelayUntil () especifica el tiempo absoluto (exacto) en el que desea desbloquear. 
[Link Text](http://web.ist.utl.pt/~ist11993/FRTOS-API/group___task_ctrl.html#vTaskDelayUntil)

```C++
#include <Arduino.h>
const portTickType xDelay = 5000 / portTICK_RATE_MS;
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

void loop1(void *pvParameters) {
  while (1) {
     Serial.println("loop1");
     vTaskDelay (xDelay); //Ejecuta esta tarea cada 5000 milisegundos
  }
}

void loop2(void *pvParameters) {
  while (1) {
     Serial.println("loop2");
     //vTaskDelay(5000);
     delay(300);
  }
}

void loop3(void *pvParameters) {
  while (1) {
     Serial.println("loop3");
     //vTaskDelay(5000);
     delay(4000);
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(loop1, "loop1", 4096, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(loop2, "loop2", 4096, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(loop3, "loop3", 4096, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}

void loop() {
   Serial.println("loop0");
   delay(4000);
}
```

# Algoritmo de scheduling
* Como elige el scheduler la tarea a ejecutar? Usa un algoritmo
llamado Fixed priority preemptive scheduling
* Fixed priority significa que el kernel NO MODIFICA las prioridades
de las tareas (salvo en un caso particular que se ver� m�s
adelante).
* Esto simplifica el an�lisis de ejecuci�n del sistema, a la vez que
transfiere al programador la total responsabilidad del cumplimiento
de los plazos.
* SIEMPRE ejecuta la tarea de mayor prioridad que esta en
condiciones de ejecutarse.
* Esto puede hacer que tareas de menor prioridad no reciban
ning�n tiempo de ejecuci�n. A esto se le denomina �starvation�
(hambreado) de la tarea.
* Ya veremos c�mo una aplicaci�n deja de estar en condiciones de
ejecutarse
* Si la aplicaci�n est� en modo preemptive, al cabo de un
tiempo el scheduler va a retirar la tarea en ejecuci�n solo si
hubiera una de igual prioridad en condiciones de
ejecutarse.

* Recordar: �la tarea de mayor prioridad en
condiciones de ejecutarse� (en estado Ready)
* Si hubiere varias tareas de la misma prioridad en este
estado, el kernel las ejecuta secuencialmente a todas.
* Si hubiere alguna(s) de menor prioridad, no va(n) a recibir
tiempo alguno hasta que todas las de mayor prioridad salgan
del estado Ready.
* Se define una frecuencia llamada configTICK_RATE_HZ. La inversa es el lapso que el scheduler asigna a cada
tarea para ejecutarse antes de verificar si debe quitarla
del estado Running.
* La definici�n de esta frecuencia es un punto importante
del dise�o.

# Consideraciones de dise�o
* Siempre que varios procesos deban ocurrir en
paralelo, son candidatos a ser tareas.
* Como primer criterio para asignar prioridades, se
sugiere preguntarse que evento se debe atender y
cu�l puede esperar si ambos ocurren
simult�neamente.
* Se debe evitar el hambreado de las tareas de menor
prioridad.
* Esto no es necesariamente un error, si la tarea de mayor
prioridad tiene trabajo que hacer, es correcto que no le de
paso a las demas.
* Es importante hacer un an�lisis detallado de los
eventos, sus respuestas y los compromisos
temporales antes de asignar las prioridades de
ejecuci�n.




