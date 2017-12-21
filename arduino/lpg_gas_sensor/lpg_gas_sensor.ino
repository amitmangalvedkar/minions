//int inPin = 7;


void setup() {
    Serial.begin(57600);
//  pinMode(inPin, INPUT);
}

void loop() {
    float sensor_volt;
    float sensorValue;

    sensorValue = analogRead(A0);
    
    sensor_volt = sensorValue/1024*5.0;

    
  //  sensor_volt = digitalRead(inPin);
  //  Serial.print("sensor_volt = ");
    Serial.println(sensor_volt);
  //  Serial.println(" V");
    delay(1000);
}

