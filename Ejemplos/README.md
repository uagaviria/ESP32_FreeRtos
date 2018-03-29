# Task (Tarea)
Se implementan en C y su prototipo tiene que devolver un void y tener como argumento un puntero void:
void FuncionTask (void *pvParametro);

Cada Task es un peque�o programa, tiene un punto de entrada y se ejecuta eternamente debido a un bucle infinito.

No se puede salir de un Task mediante un return, sino hay que eliminar el Task.
```C++
void FuncionTask (void *pvParametro)
{
    //definici�n de variables
    
    for(;;)
    {
        //c�digo de la funci�n
    }
    //como por aqu� no puede pasar nunca, en caso de que lo haga se elimina la funci�n
    vTaskDelete( NULL );
}
```
Estados de las Tareas
Cada aplicaci�n consiste en varias Task.

Como el microcontrolador (normalmente) s�lo tiene un n�cleo, s�lo se ejecuta una tarea en cada momento. Por lo que las tareas pueden estar "funcionado" o "no funcionando".
Esto no es cierto, es un modelo simple del funcionamiento que nos vale por ahora. Pero, hay que tener en cuenta que el estado "no funcionando" posee varios subestados.

En el estado funcionando, se est� ejecutando c�digo. Cuando est� en no funcionando, la tarea est� inactiva esperando a que el programador la active.

Cuando una tarea pasa de no funcionando a funcionando lo hace a partir de la �ltima instrucci�n que ejecut� al dejar el estado funcionando.

La transici�n de no funcionando a funcionado se denomina "switched in" o "swapped in" y, al contrario, "switched out" o "swapped out". Estos cambios s�lo los puede realizar el scheduler (programador de tareas) del FreeRTOS.

Creaci�n de Tareas
Funci�n de la API xTaskCreate()
Esta funci�n es sirve para crear tareas.

portBASE_TYPE xTaskCreate( pdTASK_CODE pvTaskCode, const signed char * const pcName, unsigned short usStackDepth, void *pvParameters, unsigned portBASE_TYPE uxPriority, xTaskHandle *pxCreatedTask);

pvTaskCode: es un puntero a la funci�n que implementa la tarea
pcName: un nombre descriptivo de la tarea. S�lo sirver para debug
usStackDepth: cada tarea tiene una pila de programa propia en el kernel, este par�metro determina el tama�o de la pila (en words).
El tama�o de la pila para la tareas en reposo se define mediante al constante de configMINIMAL_STACK_SIZE.
pvParameters: son los par�metros que necesita la tarea
uxPriority: define la prioridad de la funci�n de 0 a configMAX_PRIORITIRES-1
Se recomienda usar el n�mero de prioridad inferior posible para ahorrar RAM.
pxCreatedTask: para pasar un argumento a la tarea. Si no se utilizan argumentos, se pone NULL.

Hay dos posibles valores de retorno:

    pdTRUE la tarea se ha creado
    errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY no hay suficiente memoria para crear la tarea.
Las tareas se crean en el main y, al finalizar, hay que llamar a la funci�n vTaskStartScheduler(); para iniciar el programador de tareas y que se ejecuten.

Prioridad de las Tareas
El par�metro uxPriority asigna la prioridad inicial al crearse la tarea, pero esta se puede cambiar llamando a la funci�n de la API vTaskPrioritySet().

El valor m�ximo de prioridades disponible es el definido por la constante configMAX_PRIORITIRES en el archivo FreeRTOSConfig.h.

FreeRTOS no tiene un l�mite m�ximo de valores de prioridad pero s� est� limitado por la RAM del microcontrolador.

No hay resticciones en la definici�n de la prioridad se puede compartir el nivel de prioridad o poner un �nico nivel a todo.

El planificador de tareas activar� las tareas con un mayor grado de prioridad.

Si el planificador va a activar una tarea prioritaria y hay varias con la misma prioridad, el planificador actiavar� una y desactivar� la otra sucesivamente.

Para poderse ejecutar la siguiente tarea, el planificador se activa al finalizar cada periodo de las tareas.

Este priodo, se determina con la constante configTICK_RATE_HZ en el archivo FreeRTOSConfig.h.

Como la API del FreeRTOS trabaja con tick, la constante portTICK_RATE_MS proporciona el ratio de conversi�n de ticks a milisegundos.

"Tick Count" determina el total de ticks que han ocurrido desde que se inicialializ� el planificador teniendo en cuenta que no se haya saturado.

Estado "Not Running"
Debido a problemas a tiempos en el inicio de las tareas, s�lo con el estado Running y not Running no vale. Hay que crear estados nuevos.

Estado Blocked
Cuando una tarea est� esperando un evento se encuentra en este estado.

Para entrar en este estado, puede ocurrir dos tipos de eventos:

eventos temporales. Espera a que pase un periodo determinado

Eventos de sincronizaci�n. espera a que termine otra tarea o a una interrupci�n.

Para que exista este estado, se necesitan los sem�foros.

Se puede bloquear un evento de sincronizaci�n esperando un tiempo. Por ejemplo, si en 10ms tiene que ocurrir una interrupci�n.

La funci�n para introducir retardos es void vTaskDelay( portTickType xTicksToDelay). Siendo xTicksToDelay el n�mero de tick que va a permanecer la tarea en estado Blocked.
Si se quiere trabajar en ms en vez de ticks, tenemos la constante portTICK_RATE_MS y, dividiendo por ella, tendremos ms.

Otra funci�n es vTaskDelayUntil(), similar a vTaskDelay pero que determina el n� de ticks en el que va a dejar de estar bloqueada la tarea.

void vTaskDelayUntil(portTickType *pxPreviousWakeTime, portTickType xTimeIncrement);

pxPreviousWakeTime es el periodo en el que est�. Se actualiza autom�ticamente.
xTimeIncrement indica el n�mero de tick que tienen que transcurrir para que salga de estado Blocked
Estado Suspended
Si la tarea est� en estado suspended, el planificador de tareas no toma en cuenta dicha tarea.

Para poder poner una tarea en este estado, hay que llamar a la funci�n de la API vTaskSuspend() y para salir de dicho estado, vTaskResume() o vTaskResumeFromISR().

En la mayor�a de las aplicaciones no se usa este estado.

Estado Ready
Las tareas que est�n en notRunning pero no est�n Blocked o Suspended se encuentan en Ready. Est�n esperando a que el planificador las active.

Tareas Idle y Idle Hook
Como el microprocesador necesita realizar alguna tarea, si tenemos todas en estado Blocked, necesitamos alguna por defecto. Esto se realiza autom�ticamente al llamar la funci�n vTaskStartSchedule() que crea la tarea Idle que, b�sicamente, es un loop.

Esta tarea Idle tiene la prioridad inferior posible (prioridad cero).

Funciones Idle Hook
Se puede a�adir alguna funcionalidad espec�fica a la funci�n Idle utilizando una funci�n Idle Hook (funci�n que se ejecuta autom�ticamente en el loop de la funci�n Idle).

Normalmente se utilizan para:

medir la capacidad no utlizada del microprocesardor.
poner el microprocesardor en modo baja corriente
ejecutar cosas en baja prioridad, en background o procesos continuos
Las funciones Hook tiene que tener el nombre y el prototipo void vApplicationIdleHook( void );

Limitaciones
Nunca se pueder bloquear o suspender

Si la aplicaci�n usa la funci�n vTaskDelete() entonces la funci�n Hook tiene que volver al punto de llamada en un tiempo razonable.
cambiar la prioridad de las tareas

Seleccionando la prioridad de las tareas
Como regla general, la prioridad de las tareas se selecciona dependiendo de su importancia en el tiempo. Sin embargo, hay que tener en cuenta cosas como el uso del microprocesador, tiempo de ejecuci�n... y que nunca se puede saturar.

Existe una t�cnica llamada Rate Monotonic Scheduling (RMS) que dicta una prioridad �nica para cada tarea dependiendo del periodo de ejecuci�n.

La prioridad mayor se asigna a la tarea que tenga la frecuencia mayor y viceversa. Mediante este sistema te aseguras de maximizar los tiempos de la aplicaci�n.

El problema es si existen variaciones en los tiempos de ejecuci�n.

Funci�n vTaskPrioritySet()
La funci�n vTaskPrioritySet se utiliza para cambiar la prioridad de las tareas una vez el programador de tareas se haya inicializado.

Hay que tener en cuenta que esta funci�n s�lo se est� disponible si el par�metro de configuraci�n INCLUDE_vTaskPrioritySet del fichero FreeRTOSConfig.h est� a uno.

void vTaskPrioritySet( xTaskHandle pxTask, unsigned portBASE_TYPE uxNewPriority);

pxTask es el enlace a la tarea que se quiere modificar es el par�metro pxCreatedTask de la funci�n xTaskCreate(). Una tarea puede modificar su prioridad mandando un NULL en este par�metro
uxNewPriority la prioridad que queremos poner. El valor es entre 0 y configMAX_PRIORITIES-1
Funci�n uxTaskPriorityGet()
Sirve para averiguar el valor de la prioridad de la funci�n.

Hay que tener en cuenta que esta funci�n s�lo se est� disponible si el par�metro de configuraci�n INCLUDE_vTaskPriorityGet del fichero FreeRTOSConfig.h est� a uno.

unsigned portBASE_TYPE uxTaskPriorityGet( xTaskHandle pxTask);

pxTask es el enlace a la tarea que se quiere modificar es el par�metro pxCreatedTask de la funci�n xTaskCreate(). Una tarea puede modificar su prioridad mandando un NULL en este par�metro
devuelve el valor de la prioridad
Eliminaci�n de Tareas
Funci�n vTaskDelete()
Una tarea puede usar la funci�n vTaskDelete para eliminarse a ella misma o a otra tarea.

Hay que tener en cuenta que esta funci�n s�lo se est� disponible si el par�metro de configuraci�n INCLUDE_vTaskDelete del fichero FreeRTOSConfig.h est� a uno.

Las tareas eliminadas dejar�n de existir y no podr�n volver a entrar en estado Running.

La tarea Idle es la responsable de liberar la memoria cuando se ejecuta esta funci�n. Por eso, hay que tener en cuenta que podemos relentizar el sistema si llamamos muchas veces a esta funci�n.

S�lo la memoria usada por un el kernel se libera autom�ticamente, el resto se tiene que liberar expl�citamente.

void vTaskDelete( xTaskHandle pxTaskToDelete);

pxTaskToDelete es el enlace a la tarea que se quiere modificar es el par�metro pxCreatedTask de la funci�n xTaskCreate(). Una tarea puede modificar su prioridad mandando un NULL en este par�metro.
Algoritmo del Planificador de tareas
Priorizar la programaci�n preferente
FreeRTOS seleccionar la tarea que est� activa dependiendo de:

cada tarea tiene su prioridad
cada tarea puede estar en distintos estados
s�lo una tarea puede estar en estado activo
el planificador selecciona la tarea con mayor prioridad que se tenga que ejecutar.
Este tipo de esquema se denomina de prioridad fija porque cada tarea no modifica el kernel; y preferente porque si una tarea entra en Ready o se modifica su prioridad pasa a estado Running si la que est� activa tiene una prioridad menor.

Las tareas esperan en estado Blocked hasta que transcurra el periodo definido o que ocurra una interrupci�n esperada.

Planificador de tareas cooperativo
Cuando se utiliza un planificador cooperativo se produce un cambio de tarea si la tarea activa pasa a Blocked o se ejecuta la funci�n taskYIELD().

Las tareas dejan de ser preferentes y no comparten tiempos de procesamiento.

Se puede mezclar un sistema preferente y uno cooperativo pero no depender�a del tiempo.