/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continuous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

int conf = 0;
int buttonState;
bool cmarmander = true;
int led = 13;
int current;         // Current state of the button
                     // (LOW is pressed b/c i'm using the pullup resistors)
long millis_held;    // How long the button was held (milliseconds)
long secs_held;      // How long the button was held (seconds)
long prev_secs_held; // How long the button was held in the previous check
byte previous = HIGH;
unsigned long firstTime; // how long since the button was first pressed 

const char* host = "api.cityos.io";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "AF 41 BF BB 0C E3 52 75 5E DC 02 F4 3D DC 50 8B 0E 95 2A 53";

void setup() {
  Serial.begin(9600);
  Serial.println();
  pinMode(conf,INPUT);
pinMode(led , OUTPUT);
  
}

void loop() {
configureWifi();
sendDrop("temp",2)
}


void sendDrop(String namme ,int value){
 // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
 String data = "{\""+ String(namme) +"\":" + String(value) + "}";
  String url = "/device/18/readings";
  Serial.print("requesting URL: ");
  Serial.println(url);
client.print(String("POST ") + url +" HTTP/1.1\r\n");
  client.print("Host: api.cityos.io\r\n");
   client.print("User-Agent: ESP8266\r\n");
   client.print("Connection: close\r\n");
  client.print("Content-Type: application/json\r\n");
  client.print("Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6MiwiaWF0IjoxNDcyNjYwNDY4fQ.-4xgdG5H5gXIypI6kznIPTJPwX_HLda_H1S8zUMAXIw\r\n");
  client.print("Content-Length: ");
   client.print(data.length());
  client.print("\r\n\r\n");
  client.println(data);
  
  Serial.println("request sent");
  
while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
   String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection"); 
  }

  void configureWifi(){
  
  if(WiFi.status() != WL_CONNECTED) {
   digitalWrite(led , 1);
  }
   else {
    digitalWrite(led , 0);
    }

   current = digitalRead(conf);

  // if the button state changes to pressed, remember the start time 
  if (current == LOW && previous == HIGH && (millis() - firstTime) > 200) {
    firstTime = millis();
  }

  millis_held = (millis() - firstTime);
  secs_held = millis_held / 1000;

  // This if statement is a basic debouncing tool, the button must be pushed for at least
  // 100 milliseconds in a row for it to be considered as a push.
  if (millis_held > 50) {
    // check if the button was released since we last checked
    if (current == HIGH && previous == LOW) {
      // Button held for 1-3 seconds, print out some info
      if (secs_held >= 3) {
     cmarmander = false;
      }
    }
  }

  previous = current;
  prev_secs_held = secs_held;
  
  while(cmarmander != true){
  WiFi.beginSmartConfig(); 
  digitalWrite(led,1);
  delay(500);
  digitalWrite(led,0);
  delay(500);
     if (WiFi.smartConfigDone()) {
          Serial.println("done");
      digitalWrite(led,0); 
     delay(3000);
 WiFi.stopSmartConfig(); 
  cmarmander = true;   
    }
  }
  }
