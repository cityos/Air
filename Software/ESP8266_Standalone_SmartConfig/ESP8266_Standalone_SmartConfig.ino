    /**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * This example runs directly on ESP8266 chip.
 *
 * You need to install this for ESP8266 development:
 *   https://github.com/esp8266/Arduino
 *
 * Please be sure to select the right ESP8266 module
 * in the Tools -> Board menu!
 *
 * NOTE: SmartConfig might not work in your environment.
 *       Please try basic ESP8266 SmartConfig examples
 *       before using this sketch!
 *
 * Change Blynk auth token to run :)
 *
 **************************************************************/
  // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
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


void setup(){
  pinMode(conf,INPUT);
pinMode(led , OUTPUT);
  Serial.begin(9600);
 // WiFi.mode(WIFI_STA);
  /*WiFi.status() != WL_CONNECTED*/

}

void loop()
{
configureWifi();
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
