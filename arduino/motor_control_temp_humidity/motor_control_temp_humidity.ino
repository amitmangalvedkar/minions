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

#include <dht.h>

#define dht_dpin A0 //no ; here. Set equal to channel sensor is on

dht DHT;

//Ground (middle pin) on L298N to Ground on Arduino
//IN1 to Digital Pin 7
//IN2 to Digital Pin 6
//Do not remove jumper pins from ENA or ENB. Only needed for PWM
//All the pins on the right side

const int IN1 = 7;
const int IN2 = 6;

bool start_motor(int);
bool stop_motor();

String temperature_humidity_measured();



void setup() {
  Serial.begin(9600);

  delay(1000);

  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
}

void loop() {

  //analogWrite(ENA, 255);
  //analogWrite(ENB, 255); 

  Serial.println(temperature_humidity_measured());

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

String temperature_humidity_measured() {
    DHT.read11(dht_dpin);

    String dht;
    dht = String(dht + "status \"humidity\":\"" + DHT.humidity + "%\",\"temperature\":" + DHT.temperature);

    delay(1000);
    return dht;
}

