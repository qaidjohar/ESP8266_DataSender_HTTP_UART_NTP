#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ezTime.h>

//WiFi Credentials
String ssid = "QJ";
String password = "1234567@123";

//Server URL
//String URL = "http://192.168.43.6:5000/postjson";
String URL = "http://testdomain.elliotsystemsonline.com:3001/api/DAL/RawData";

//Global Variables
String readString;

//NTP Timezone
Timezone myTZ;
 
void setup() {
  Serial.begin(115200);         //Serial connection
  WiFi.begin(ssid, password);   //WiFi connection
  while (WiFi.status() != WL_CONNECTED) {  
    //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
  Serial.println("Sync for NTP Connection");
  waitForSync(); //For NTP synchronization
  myTZ.setLocation("in");
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    if (Serial.available()){
      StaticJsonBuffer<600> JSONbuffer;   //Declaring static JSON buffer
      JsonObject& JSONencoder = JSONbuffer.createObject(); 
      JSONencoder["sensor_id"] = "EM_001";
      JSONencoder["created_at"] = myTZ.dateTime(ISO8601);
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
        readString = "";

        HTTPClient http;    //Declare object of class HTTPClient
        http.begin(URL);      //Specify request destination
        http.addHeader("Content-Type", "application/json");  //Specify content-type header
        int httpCode = http.POST(JSONmessageBuffer);   //Send the request
        String payload = http.getString();  //Get the response payload
        Serial.println(httpCode);   //Print HTTP return code
        Serial.println(payload);    //Print request response payload
        http.end();  //Close connection
      }
      //if (Serial.available()) {
      //  Serial.write(Serial.read());
      //}
    }
  } else { 
    Serial.println("Error in WiFi connection"); 
    delay(5000);  //Send a request every 30 seconds
  }
}
