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

