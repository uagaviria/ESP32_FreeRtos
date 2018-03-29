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

# La letra chica 
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

# Es fácil de implementar

* Hay mucha documentación disponible.
* Libro de texto escrito por uno de los diseñadores del
sistema.
* Incluye una demostración de sus funciones para cada
plataforma en la que está soportado.
* Nutrida comunidad de usuarios 
* Hay una opción con soporte comercial.
* Hay una opción con certificación SIL-3 para
sistemas de misión critica.

# Está pensado para microcontroladores

* Está escrito mayormente en C, salvo las partes
específicas de cada plataforma.
* Es liviano en tamaño de código y en uso de
memoria RAM.

# Primera mirada a FreeRTOS

* Es un mini kernel de tiempo real que puede trabajar en modos
cooperativo, preemptive o mixto.
* Mini kernel significa que provee los servicios mínimos e
indispensables
* Permite compilar solo las funciones que se vayan a usar,
acotando así su impacto en la memoria de código.
* Se puede definir y acotar en tiempo de compilación el uso de
memoria de datos por parte del sistema.
* Ofrece funciones de temporización, de comunicación entre
tareas, de sincronización entre tareas e interrupciones, y de
definición de secciones críticas

# Algunas opciones de configuración

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

# Cómo es una tarea en FreeRTOS?
* Citando a Richard Barry: “Las tareas se
implementan con funciones de C. Lo único especial
que tienen es su prototipo, que debe devolver void y
recibir un puntero a void como parámetro”.
* void vTareaEjemplo (void *parametros);
* Cada tarea tiene su propio punto de entrada,
sección de inicialización y lazo de control.
* Las funciones de tarea (en adelante, tareas) en FreeRTOS NO DEBEN RETORNAR BAJO NINGÚN
CONCEPTO. No deben incluir un return ni
ejecutarse hasta la llave de cierre
* Si una tarea deja de ser necesaria, puede eliminársela
explícitamente. 

# Cómo es una tarea en FreeRTOS?
* Las tareas tienen una prioridad de ejecución. 0 es la
menor prioridad.
* Se recomienda usar referencias a tskIDLE_PRIORITY (+1, +2,
etc)
* No hay límites a la cantidad de prioridades del sistema, pero se
gasta RAM por cada una. Usar con cuidado.
* Se pueden crear múltiples instancias de una misma
función de tarea
* Estas instancias pueden recibir un parámetro que las
caracterice
* IMPORTANTE: Cuando una tarea deja de estar en
ejecución, las referencias a variables alojadas en su pila
dejan de ser válidas.
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
Como el núcleo arduino del ESP32, se ejecuta sobre FreeRTOS y puede llamar a las API directamente en el boceto Arduino, no es necesaria una biblioteca adicional.

# Código

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

El siguiente código nos muestra que podemos correr nuestro programa en cualquiera de los dos nucleos del ESP32. y correr las tareas en los tiempos que mas nos convenga. los delay pueden ser modificados para esto. si corres el programa en consola te daras cuenta de lo que hablo.

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

Mientras que vTaskDelay () especifica un tiempo de activación relativo al momento en que se llama a la función, vTaskDelayUntil () especifica el tiempo absoluto (exacto) en el que desea desbloquear. 
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
de las tareas (salvo en un caso particular que se verá más
adelante).
* Esto simplifica el análisis de ejecución del sistema, a la vez que
transfiere al programador la total responsabilidad del cumplimiento
de los plazos.
* SIEMPRE ejecuta la tarea de mayor prioridad que esta en
condiciones de ejecutarse.
* Esto puede hacer que tareas de menor prioridad no reciban
ningún tiempo de ejecución. A esto se le denomina “starvation”
(hambreado) de la tarea.
* Ya veremos cómo una aplicación deja de estar en condiciones de
ejecutarse
* Si la aplicación está en modo preemptive, al cabo de un
tiempo el scheduler va a retirar la tarea en ejecución solo si
hubiera una de igual prioridad en condiciones de
ejecutarse.

* Recordar: “la tarea de mayor prioridad en
condiciones de ejecutarse” (en estado Ready)
* Si hubiere varias tareas de la misma prioridad en este
estado, el kernel las ejecuta secuencialmente a todas.
* Si hubiere alguna(s) de menor prioridad, no va(n) a recibir
tiempo alguno hasta que todas las de mayor prioridad salgan
del estado Ready.
* Se define una frecuencia llamada configTICK_RATE_HZ. La inversa es el lapso que el scheduler asigna a cada
tarea para ejecutarse antes de verificar si debe quitarla
del estado Running.
* La definición de esta frecuencia es un punto importante
del diseño.

# Consideraciones de diseño
* Siempre que varios procesos deban ocurrir en
paralelo, son candidatos a ser tareas.
* Como primer criterio para asignar prioridades, se
sugiere preguntarse que evento se debe atender y
cuál puede esperar si ambos ocurren
simultáneamente.
* Se debe evitar el hambreado de las tareas de menor
prioridad.
* Esto no es necesariamente un error, si la tarea de mayor
prioridad tiene trabajo que hacer, es correcto que no le de
paso a las demas.
* Es importante hacer un análisis detallado de los
eventos, sus respuestas y los compromisos
temporales antes de asignar las prioridades de
ejecución.




