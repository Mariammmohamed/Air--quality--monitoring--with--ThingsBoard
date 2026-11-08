
 
#include<Arduino.h>
 
// ---------------- Pin definitions -------------------
const int MQ = 34;

 

 
void setup() {
  Serial.begin(115200);
 

  

}
 
void loop() {
  
   int sensorValue = analogRead(MQ);
  Serial.print("Analog Sensor Value: ");
  Serial.println(sensorValue);
  delay(1000);
}