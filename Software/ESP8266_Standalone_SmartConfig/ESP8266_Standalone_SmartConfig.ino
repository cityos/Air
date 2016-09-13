
#include <ESP8266WiFi.h>
int conf = 0;
int buttonState;
bool cmarmander = true;
int led = 13;
int current;        
                     
long millis_held;   
long secs_held;    
long prev_secs_held; 
byte previous = HIGH;
unsigned long firstTime; 


void setup(){
  pinMode(conf,INPUT);
pinMode(led , OUTPUT);
  Serial.begin(9600);
 

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
