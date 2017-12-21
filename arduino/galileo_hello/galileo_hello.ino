void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("He");
  system("ifconfig eth0 > /dev/cu.usbmodem1411");
  delay(1000);
}
