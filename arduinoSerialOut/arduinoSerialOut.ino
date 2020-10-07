#include <SoftwareSerial.h>
SoftwareSerial ESPserial(2, 3); // RX | TX

void setup(){
 Serial.begin(115200);
 ESPserial.begin(115200);  
 Serial.print(char(169)); // Copyright Symbol
 delay(2000);
}
void loop()
{
  String humi="3344";
  String temp="5566";
  //Serial.print("H: ");
  //Serial.print(humi); 
  //Serial.print("% ");
  //Serial.print(" T: ");
  //Serial.print(temp); 
  //Serial.print(char(176));
  Serial.println("65536,32567,4567,65536,65536,65536,65536,4356,65536,65536,35446,65536,65536,65536,65536,65536,65536,65536,65536,65536,65536,65536,65536,9875");
  ESPserial.println("65536,32567,4567,65536,65536,65536,65536,4356,65536,65536,35446,65536,65536,65536,65536,65536,65536,65536,65536,65536,65536,65536,65536,9875");
  String str =String('H')+String(humi)+String('T')+String(temp);
  //Serial1.println(str);
  delay(5000);
}
