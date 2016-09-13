void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
getMacAddress();
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
