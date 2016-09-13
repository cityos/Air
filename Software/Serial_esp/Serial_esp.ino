int pm1 ;
    int pm25 ;
    int pm10;
    int co ;
    int no2 ;
    int mic ;
 float uv ;
    // do it again:
    float light ;
    // do it again:
    float temp ;
    float pressure ;
    float altitude;
    float humid;
    int x = 1;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
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
     Serial.println(pm1);
    Serial.println(pm25);
    Serial.println(pm10);
    Serial.println(co);
    Serial.println(no2);
   Serial.println(mic);
  Serial.println(uv);
    Serial.println(light);
    Serial.println(temp);
    Serial.println(pressure);
    Serial.println(altitude);
    Serial.println(humid);
   // Serial.println("F");
 
  }
  delay(9000);
}
