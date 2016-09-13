#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif
//#include <TimerOne.h>

#include <EasyScheduler.h>
#include <ESP8266WiFi.h>
//const char* ssid     = "Innbox-internet-7ec5c2";
//const char* password = "INNBOX1201YUH001333";
Schedular Task1;
Schedular Task2;
const char* host = "api.cityos.io";
int conf = 0;
int buttonState;
bool cmarmander = true;
int led = 13;
int current;    

long millis_held;    // How long the button was held (milliseconds)
long secs_held;      // How long the button was held (seconds)
long prev_secs_held; // How long the button was held in the previous check
byte previous = HIGH;
unsigned long firstTime; // how long since the button was first pressed 

void sendDrop(float temp,float humid,float altitude,float uv, float light,int pm1, int pm25, int pm10 , int mic, int co, int no2)
{
     WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
    if (client.connected())
    {
        Serial.println("connection successful");
         String data = "{\"1\":"+String(temp)+",\"2\":"+String(humid)+",\"3\":"+String(altitude)+",\"4\":"+ String(uv)+",\"5\":"+ String(light)+",\"6\":"+ String(pm1)+",\"7\":"+ String(pm25)+",\"8\":"+ String(pm10)+",\"9\":"+ String(mic)+",\"10\":"+ String(co)+",\"11\":"+ String(no2)+"}";
        client.println("POST /device/18/readings HTTP/1.1");
        client.println("Host: api.cityos.io");
        client.println("Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6MiwiaWF0IjoxNDcyNjYwNDY4fQ.-4xgdG5H5gXIypI6kznIPTJPwX_HLda_H1S8zUMAXIw");
        client.println("Content-Type: application/json");
        client.print("Content-Length: ");
        client.println(data.length());
        client.println();
        client.println(data);
     Serial.println(data);
        while (client.available()) {
            char c = client.read();
            Serial.print(c);
        }
    }
    else {
        Serial.println("connection failed");
        client.stop();
    }
    yield();
}


String getMacAddress() {
byte mac[6];

WiFi.macAddress(mac);
String cMac = "";
for (int i = 0; i < 6; ++i) {
cMac += String(mac[i],HEX);
if(i<5)
cMac += "-";

}

cMac.toUpperCase();
return cMac;
}

void setup()
{
 
    Serial.begin(115200);
//WiFi.begin(ssid, password);
  //WiFi.beginSmartConfig();
  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(500);
  //  Serial.print(".");
  //}
    pinMode(conf,INPUT);
pinMode(led , OUTPUT);
 Task1.start();
Task2.start();


 
}

void loop()
{
 
  //sendData1();
// Serial.println("sleep");
 // wifi_set_sleep_type(MODEM_SLEEP_T);
 //ESP.deepSleep(9 * 1000000);
  
 // Serial.println(WiFi.RSSI());
 //sendDrop();
// WiFi.forceSleepBegin();
 //delay(1);
  

Task1.check(configureWifi,100); //every 100ms do this task so you can configure device
 Task2.check(sendData1,10000);//wait 10 sec and send data 
 //ESP.deepSleep(1, WAKE_RF_DEFAULT);
// delay(1);
 
}

void configureWifi(){

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
      if (secs_held >= 2) {
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
     delay(4000);
 WiFi.stopSmartConfig(); 
  cmarmander = true;   
    }
  }
  }

  void sendData1(){
    
   Serial.write(0x16);
if(Serial.find(0x17)) {
    float uv = Serial.parseFloat();
    // do it again:
    float light = Serial.parseFloat();
    // do it again:
    float temp = Serial.parseFloat();
    float pressure = Serial.parseFloat();
    float altitude = Serial.parseFloat();
    float humid = Serial.parseFloat();
    int pm1 = Serial.parseInt();
    int pm25 = Serial.parseInt();
    int pm10 = Serial.parseInt();
    int co = Serial.parseInt();
    int no2 = Serial.parseInt();
    int mic = Serial.parseInt();
    //Serial.write(0xDDD);
   /* Serial.println(pm1);
    Serial.println(pm25);
    Serial.println(pm10);
    Serial.println(co);
    Serial.println(no2);
   Serial.println(mic);
  Serial.println(uv);
    Serial.println(light);
    Serial.println(temp - 11.70); 
    Serial.println(pressure);
    Serial.println(altitude);
    Serial.println(humid + 17.00);*/
    
 sendDrop(temp-11.70,humid+17.00,altitude,uv,light,pm1,pm25,pm10,mic,co,no2);
 ESP.deepSleep(10 * 1000000, WAKE_RF_DEFAULT);
  }
  
    }
  
