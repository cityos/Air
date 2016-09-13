void setup() {
pinMode(A6 , INPUT);
Serial.begin(9600);
}

void loop() {
int dB = 20 * log10(analogRead(A6)/0.005010);
int x = (-44) + dB;
int y = x + 92;
int rez = y-40;
Serial.print(rez);
Serial.println(" db");
delay(100);


}
