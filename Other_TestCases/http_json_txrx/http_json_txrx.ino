#include <ArduinoJson.h>
#include <ezTime.h>
#include <ESP8266WiFi.h>


String readString;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  WiFi.begin("QJ", "1234567@123");
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
  waitForSync();
  Serial.println("UTC: " + UTC.dateTime());
  Serial.println("ISO8601:      " + UTC.dateTime(ISO8601));
  Timezone myTZ;
  myTZ.setLocation("in");
  Serial.print("India:         ");
  Serial.println(myTZ.dateTime());
  Serial.println(myTZ.dateTime(ISO8601));
  delay(5000);
  Serial.println(myTZ.dateTime(ISO8601));
}

void loop() { // run over and over
  

  if (Serial.available()){
    StaticJsonBuffer<600> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 
    JSONencoder["sensor_id"] = "EM_001";
    JSONencoder["created_at"] = "2019-11-22T10:19:20Z";
    JsonArray& values = JSONencoder.createNestedArray("values"); //JSON array
    while (Serial.available()) {
      delay(3);  //delay to allow buffer to fill
      if (Serial.available() >0) {
        char c = Serial.read();  //gets one byte from serial buffer
        if(c==','){
            Serial.println(readString);
            values.add(readString); //Add value to array
            readString = "";
        } else if(c=='\n' || c=='\r'){
            continue;  
        }
        else {
        readString += c; //makes the string readString
        }
      }
    }
    if (readString.length() >0){
      Serial.println(readString);
      values.add(readString);
      char JSONmessageBuffer[600];
      JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
      Serial.println(JSONmessageBuffer);
    
      Serial.println("\n\n");
      readString = "";
    }
    //if (Serial.available()) {
    //  Serial.write(Serial.read());
    //}
  }
}
