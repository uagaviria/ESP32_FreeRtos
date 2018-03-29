# Task (Tarea)
Se implementan en C y su prototipo tiene que devolver un void y tener como argumento un puntero void:
void FuncionTask (void *pvParametro);

Cada Task es un pequeño programa, tiene un punto de entrada y se ejecuta eternamente debido a un bucle infinito.

No se puede salir de un Task mediante un return, sino hay que eliminar el Task.
```C++
void FuncionTask (void *pvParametro)
{
    //definición de variables
    
    for(;;)
    {
        //código de la función
    }
    //como por aquí no puede pasar nunca, en caso de que lo haga se elimina la función
    vTaskDelete( NULL );
}
```
Estados de las Tareas
Cada aplicación consiste en varias Task.

Como el microcontrolador (normalmente) sólo tiene un núcleo, sólo se ejecuta una tarea en cada momento. Por lo que las tareas pueden estar "funcionado" o "no funcionando".
Esto no es cierto, es un modelo simple del funcionamiento que nos vale por ahora. Pero, hay que tener en cuenta que el estado "no funcionando" posee varios subestados.

En el estado funcionando, se está ejecutando código. Cuando está en no funcionando, la tarea está inactiva esperando a que el programador la active.

Cuando una tarea pasa de no funcionando a funcionando lo hace a partir de la última instrucción que ejecutó al dejar el estado funcionando.

La transición de no funcionando a funcionado se denomina "switched in" o "swapped in" y, al contrario, "switched out" o "swapped out". Estos cambios sólo los puede realizar el scheduler (programador de tareas) del FreeRTOS.

Creación de Tareas
Función de la API xTaskCreate()
Esta función es sirve para crear tareas.

portBASE_TYPE xTaskCreate( pdTASK_CODE pvTaskCode, const signed char * const pcName, unsigned short usStackDepth, void *pvParameters, unsigned portBASE_TYPE uxPriority, xTaskHandle *pxCreatedTask);

pvTaskCode: es un puntero a la función que implementa la tarea
pcName: un nombre descriptivo de la tarea. Sólo sirver para debug
usStackDepth: cada tarea tiene una pila de programa propia en el kernel, este parámetro determina el tamaño de la pila (en words).
El tamaño de la pila para la tareas en reposo se define mediante al constante de configMINIMAL_STACK_SIZE.
pvParameters: son los parámetros que necesita la tarea
uxPriority: define la prioridad de la función de 0 a configMAX_PRIORITIRES-1
Se recomienda usar el número de prioridad inferior posible para ahorrar RAM.
pxCreatedTask: para pasar un argumento a la tarea. Si no se utilizan argumentos, se pone NULL.

Hay dos posibles valores de retorno:

    pdTRUE la tarea se ha creado
    errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY no hay suficiente memoria para crear la tarea.
Las tareas se crean en el main y, al finalizar, hay que llamar a la función vTaskStartScheduler(); para iniciar el programador de tareas y que se ejecuten.

Prioridad de las Tareas
El parámetro uxPriority asigna la prioridad inicial al crearse la tarea, pero esta se puede cambiar llamando a la función de la API vTaskPrioritySet().

El valor máximo de prioridades disponible es el definido por la constante configMAX_PRIORITIRES en el archivo FreeRTOSConfig.h.

FreeRTOS no tiene un límite máximo de valores de prioridad pero sí está limitado por la RAM del microcontrolador.

No hay resticciones en la definición de la prioridad se puede compartir el nivel de prioridad o poner un único nivel a todo.

El planificador de tareas activará las tareas con un mayor grado de prioridad.

Si el planificador va a activar una tarea prioritaria y hay varias con la misma prioridad, el planificador actiavará una y desactivará la otra sucesivamente.

Para poderse ejecutar la siguiente tarea, el planificador se activa al finalizar cada periodo de las tareas.

Este priodo, se determina con la constante configTICK_RATE_HZ en el archivo FreeRTOSConfig.h.

Como la API del FreeRTOS trabaja con tick, la constante portTICK_RATE_MS proporciona el ratio de conversión de ticks a milisegundos.

"Tick Count" determina el total de ticks que han ocurrido desde que se inicialializó el planificador teniendo en cuenta que no se haya saturado.

Estado "Not Running"
Debido a problemas a tiempos en el inicio de las tareas, sólo con el estado Running y not Running no vale. Hay que crear estados nuevos.

Estado Blocked
Cuando una tarea está esperando un evento se encuentra en este estado.

Para entrar en este estado, puede ocurrir dos tipos de eventos:

eventos temporales. Espera a que pase un periodo determinado

Eventos de sincronización. espera a que termine otra tarea o a una interrupción.

Para que exista este estado, se necesitan los semáforos.

Se puede bloquear un evento de sincronización esperando un tiempo. Por ejemplo, si en 10ms tiene que ocurrir una interrupción.

La función para introducir retardos es void vTaskDelay( portTickType xTicksToDelay). Siendo xTicksToDelay el número de tick que va a permanecer la tarea en estado Blocked.
Si se quiere trabajar en ms en vez de ticks, tenemos la constante portTICK_RATE_MS y, dividiendo por ella, tendremos ms.

Otra función es vTaskDelayUntil(), similar a vTaskDelay pero que determina el nº de ticks en el que va a dejar de estar bloqueada la tarea.

void vTaskDelayUntil(portTickType *pxPreviousWakeTime, portTickType xTimeIncrement);

pxPreviousWakeTime es el periodo en el que está. Se actualiza automáticamente.
xTimeIncrement indica el número de tick que tienen que transcurrir para que salga de estado Blocked
Estado Suspended
Si la tarea está en estado suspended, el planificador de tareas no toma en cuenta dicha tarea.

Para poder poner una tarea en este estado, hay que llamar a la función de la API vTaskSuspend() y para salir de dicho estado, vTaskResume() o vTaskResumeFromISR().

En la mayoría de las aplicaciones no se usa este estado.

Estado Ready
Las tareas que están en notRunning pero no están Blocked o Suspended se encuentan en Ready. Están esperando a que el planificador las active.

Tareas Idle y Idle Hook
Como el microprocesador necesita realizar alguna tarea, si tenemos todas en estado Blocked, necesitamos alguna por defecto. Esto se realiza automáticamente al llamar la función vTaskStartSchedule() que crea la tarea Idle que, básicamente, es un loop.

Esta tarea Idle tiene la prioridad inferior posible (prioridad cero).

Funciones Idle Hook
Se puede añadir alguna funcionalidad específica a la función Idle utilizando una función Idle Hook (función que se ejecuta automáticamente en el loop de la función Idle).

Normalmente se utilizan para:

medir la capacidad no utlizada del microprocesardor.
poner el microprocesardor en modo baja corriente
ejecutar cosas en baja prioridad, en background o procesos continuos
Las funciones Hook tiene que tener el nombre y el prototipo void vApplicationIdleHook( void );

Limitaciones
Nunca se pueder bloquear o suspender

Si la aplicación usa la función vTaskDelete() entonces la función Hook tiene que volver al punto de llamada en un tiempo razonable.
cambiar la prioridad de las tareas

Seleccionando la prioridad de las tareas
Como regla general, la prioridad de las tareas se selecciona dependiendo de su importancia en el tiempo. Sin embargo, hay que tener en cuenta cosas como el uso del microprocesador, tiempo de ejecución... y que nunca se puede saturar.

Existe una técnica llamada Rate Monotonic Scheduling (RMS) que dicta una prioridad única para cada tarea dependiendo del periodo de ejecución.

La prioridad mayor se asigna a la tarea que tenga la frecuencia mayor y viceversa. Mediante este sistema te aseguras de maximizar los tiempos de la aplicación.

El problema es si existen variaciones en los tiempos de ejecución.

Función vTaskPrioritySet()
La función vTaskPrioritySet se utiliza para cambiar la prioridad de las tareas una vez el programador de tareas se haya inicializado.

Hay que tener en cuenta que esta función sólo se está disponible si el parámetro de configuración INCLUDE_vTaskPrioritySet del fichero FreeRTOSConfig.h está a uno.

void vTaskPrioritySet( xTaskHandle pxTask, unsigned portBASE_TYPE uxNewPriority);

pxTask es el enlace a la tarea que se quiere modificar es el parámetro pxCreatedTask de la función xTaskCreate(). Una tarea puede modificar su prioridad mandando un NULL en este parámetro
uxNewPriority la prioridad que queremos poner. El valor es entre 0 y configMAX_PRIORITIES-1
Función uxTaskPriorityGet()
Sirve para averiguar el valor de la prioridad de la función.

Hay que tener en cuenta que esta función sólo se está disponible si el parámetro de configuración INCLUDE_vTaskPriorityGet del fichero FreeRTOSConfig.h está a uno.

unsigned portBASE_TYPE uxTaskPriorityGet( xTaskHandle pxTask);

pxTask es el enlace a la tarea que se quiere modificar es el parámetro pxCreatedTask de la función xTaskCreate(). Una tarea puede modificar su prioridad mandando un NULL en este parámetro
devuelve el valor de la prioridad
Eliminación de Tareas
Función vTaskDelete()
Una tarea puede usar la función vTaskDelete para eliminarse a ella misma o a otra tarea.

Hay que tener en cuenta que esta función sólo se está disponible si el parámetro de configuración INCLUDE_vTaskDelete del fichero FreeRTOSConfig.h está a uno.

Las tareas eliminadas dejarán de existir y no podrán volver a entrar en estado Running.

La tarea Idle es la responsable de liberar la memoria cuando se ejecuta esta función. Por eso, hay que tener en cuenta que podemos relentizar el sistema si llamamos muchas veces a esta función.

Sólo la memoria usada por un el kernel se libera automáticamente, el resto se tiene que liberar explícitamente.

void vTaskDelete( xTaskHandle pxTaskToDelete);

pxTaskToDelete es el enlace a la tarea que se quiere modificar es el parámetro pxCreatedTask de la función xTaskCreate(). Una tarea puede modificar su prioridad mandando un NULL en este parámetro.
Algoritmo del Planificador de tareas
Priorizar la programación preferente
FreeRTOS seleccionar la tarea que está activa dependiendo de:

cada tarea tiene su prioridad
cada tarea puede estar en distintos estados
sólo una tarea puede estar en estado activo
el planificador selecciona la tarea con mayor prioridad que se tenga que ejecutar.
Este tipo de esquema se denomina de prioridad fija porque cada tarea no modifica el kernel; y preferente porque si una tarea entra en Ready o se modifica su prioridad pasa a estado Running si la que está activa tiene una prioridad menor.

Las tareas esperan en estado Blocked hasta que transcurra el periodo definido o que ocurra una interrupción esperada.

Planificador de tareas cooperativo
Cuando se utiliza un planificador cooperativo se produce un cambio de tarea si la tarea activa pasa a Blocked o se ejecuta la función taskYIELD().

Las tareas dejan de ser preferentes y no comparten tiempos de procesamiento.

Se puede mezclar un sistema preferente y uno cooperativo pero no dependería del tiempo.