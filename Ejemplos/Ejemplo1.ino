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