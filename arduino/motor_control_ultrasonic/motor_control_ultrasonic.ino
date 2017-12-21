/*****************************************************************************
# Copyright (c) 2017 IBM Corporation and other Contributors.
#
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#
# Contributors:
#   Amit M Mangalvedkar - Initial Contribution
# *****************************************************************************
*/


// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;

// defines variables
long duration;
int distance;

//Ground (middle pin) on L298N to Ground on Arduino
//IN1 to Digital Pin 7
//IN2 to Digital Pin 6
//Do not remove jumper pins from ENA or ENB. Only needed for PWM
//All the pins on the right side

const int IN1 = 7;
const int IN2 = 6;

bool start_motor(int);
bool stop_motor();

int distance_measured();



void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
}

void loop() {

  //analogWrite(ENA, 255);
  //analogWrite(ENB, 255); 

  String device_event = "status \"distance\":";

  String distance = String(distance_measured());

  device_event = String(device_event + distance);
  Serial.println(device_event);

  //Serial.println(distance_measured());
  
  if(Serial.available()) {
    int value = Serial.parseInt();

    if(value > 0) {
      start_motor(value);
    } else {
      stop_motor();
    }
  }
}

bool start_motor(int time_to_run) {
  if(time_to_run > 1) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    delay(time_to_run * 1000);

    stop_motor();
    return 1;
  } else {
    return 0;
  }
}

bool stop_motor() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    return 1;
}

int distance_measured() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance= duration*0.034/2;

  delay(1000);
  return distance;
}

