#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ezTime.h>
#include <SoftwareSerial.h>

//WiFi Credentials
String ssid = "HelioKraft";
String password = "64250897";

//Server URL
//String URL = "http://192.168.43.6:5000/postjson";
String URL = "http://ems.elliotsystemsonline.com:3008/api/DAL/RawData";

//Global Variables
String readString;

//Software Serial
SoftwareSerial espSerial(13,15); // RX, TX

//NTP Timezone
//Timezone myTZ;
 
void setup() {
  Serial.begin(9600);         //Serial connection
  espSerial.begin(9600);
  WiFi.begin(ssid, password);   //WiFi connection
  while (WiFi.status() != WL_CONNECTED) {  
    //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
  Serial.println("Sync for NTP Connection");
  waitForSync(); //For NTP synchronization
  espSerial.print("A");
  //myTZ.setLocation("in");
}
int count = 0; 
int batchCount = 1;
void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    if (Serial.available()){
      StaticJsonBuffer<600> JSONbuffer;   //Declaring static JSON buffer
      JsonObject& JSONencoder = JSONbuffer.createObject(); 
      JSONencoder["bot_id"] = "EM_001";
      JSONencoder["created_at"] = UTC.dateTime(ISO8601);
      JSONencoder["batch"] = batchCount;
      JsonArray& values = JSONencoder.createNestedArray("regsiters"); //JSON array
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
        batchCount++;
        if(batchCount == 4){
            batchCount=1;
        }
      }
      //if (Serial.available()) {
      //  Serial.write(Serial.read());
      //}
    }
    espSerial.print("A");
    delay(1000);
  } else { 
    Serial.println("Error in WiFi connection"); 
    delay(5000);  //Send a request every 30 seconds
  } 
}
