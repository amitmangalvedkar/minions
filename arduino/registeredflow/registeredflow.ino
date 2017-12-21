#include <SPI.h>
#include <Ethernet.h>
#include <IPStack.h>
#include <Countdown.h>
#include <MQTTClient.h>

//#include will be needed depending on your Ethernet shield type
#define MQTT_MAX_PACKET_SIZE 100
#define SIZE 100

#define MQTT_PORT 1883

#define PUBLISH_TOPIC "iot-2/evt/status/fmt/json"
#define SUBSCRIBE_TOPIC "iot-2/cmd/+/fmt/json"
#define AUTHMETHOD "use-token-auth"

int ledPin = 13;

#define CLIENT_ID "d:uguhsp:iotsample-arduino:00aabbccde03"
#define MS_PROXY "uguhsp.messaging.internetofthings.ibmcloud.com"
#define AUTHTOKEN "t*sf0tR@UxJ8emDrqP"
// Update these with values suitable for your network.
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x03 };

EthernetClient c;
IPStack ipstack(c);

MQTT::Client<IPStack, Countdown, 100, 1> client = MQTT::Client<IPStack, Countdown, 100, 1>(ipstack);

void messageArrived(MQTT::MessageData& md);

String deviceEvent;

int motor_running;
//Ground (middle pin) on L298N to Ground on Arduino
//IN1 to Digital Pin 7
//IN2 to Digital Pin 6
//IN3 to Digital Pin 5 <-- Not needed
//IN4 to Digital Pin 4 <-- Not needed
//Do not remove jumper pins from ENA or ENB. Only needed for PWM
//All the pins on the right side

const int IN1 = 7;
const int IN2 = 6;
//const int IN3 = 5;
//const int IN4 = 4;

//const int ENA = 9;
//const int ENB = 3;

void start_motor(int);
void stop_motor(int);


void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
  pinMode(ledPin, OUTPUT); 
  delay(1000);
  motor_running = 0;
}

void loop() {
  int rc = -1;
  if (!client.isConnected()) {
    Serial.print("Connecting using Registered mode with clientid : ");
    Serial.print(CLIENT_ID);
    Serial.print("\tto MQTT Broker : ");
    Serial.print(MS_PROXY);
    Serial.print("\ton topic : ");
    Serial.println(PUBLISH_TOPIC);
    
    rc = ipstack.connect(MS_PROXY, MQTT_PORT);
    
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 4;
    options.clientID.cstring = CLIENT_ID;
    options.username.cstring = AUTHMETHOD;
    options.password.cstring = AUTHTOKEN;
    options.keepAliveInterval = 10;
    rc = -1;
    while ((rc = client.connect(options)) != 0)
      ;
    //unsubscribe the topic, if it had subscribed it before.
 
    client.unsubscribe(SUBSCRIBE_TOPIC);
    //Try to subscribe for commands
    if ((rc = client.subscribe(SUBSCRIBE_TOPIC, MQTT::QOS0, messageArrived)) != 0) {
            Serial.print("Subscribe failed with return code : ");
            Serial.println(rc);
    } else {
          Serial.println("Subscribed\n");
    }
    Serial.println("Subscription tried......");
    Serial.println("Connected successfully\n");
    Serial.println("Temperature(in C)\tDevice Event (JSON)");
    Serial.println("____________________________________________________________________________");
  }

  MQTT::Message message;
  message.qos = MQTT::QOS0; 
  message.retained = false;

  char json[75] = "{\"d\":{\"myName\":\"Arduino Uno\",\"temperature\":";
//  char buffer[10];
  float tempValue = getTemp();
  dtostrf(tempValue,1,2, &json[43]);
//  dtostrf(getTemp(),1,2, buffer);

  char motorJson[19] = ",\"motorstate\":";
  strcat(motorJson, 'a');
  //motorJson[16] = '\0';
  strcat(json, motorJson);
  json[63] = '}';
  json[64] = '}';
  json[65] = '\0';
  Serial.print("\t");
  Serial.print(tempValue);
  Serial.print("\t\t");
  Serial.println(json);
  message.payload = json; 
  message.payloadlen = strlen(json);

  rc = client.publish(PUBLISH_TOPIC, message);
  if (rc != 0) {
    Serial.print("Message publish failed with return code : ");
    Serial.println(rc);
  }
  client.yield(1000);
}

void messageArrived(MQTT::MessageData& md) {
  Serial.print("\nMessage Received\t");
    MQTT::Message &message = md.message;
    int topicLen = strlen(md.topicName.lenstring.data) + 1;

    char * topic = md.topicName.lenstring.data;
    topic[topicLen] = '\0';
    
    int payloadLen = message.payloadlen + 1;

    char * payload = (char*)message.payload;
    payload[payloadLen] = '\0';
    
    String topicStr = topic;
    String payloadStr = payload;
    
    //Command topic: iot-2/cmd/blink/fmt/json

    if(strstr(topic, "/cmd/blink") != NULL) {
      Serial.print("Command IS Supported : ");
      Serial.print(payload);
      Serial.println("\t.....\n");
      

      //Blink
      for(int i = 0 ; i < 2 ; i++ ) {
        digitalWrite(ledPin, HIGH);
        delay(1000);
        digitalWrite(ledPin, LOW);
        delay(1000);
      }

    } else if(strstr(topic, "/cmd/start") != NULL) {
      Serial.print("Command IS Supported : ");
      Serial.print(payload);
      Serial.println("\t.....\n");
      
      start_motor(9);
      
    } else if(strstr(topic, "/cmd/stop") != NULL) {
      Serial.print("Command IS Supported : ");
      Serial.print(payload);
      Serial.println("\t.....\n");
      
      stop_motor(9);
      
    } else {
      Serial.println("Command Not Supported:");            
    }
}

void start_motor(int time_to_run) {
  if(time_to_run > 1) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    delay(time_to_run * 1000);
    motor_running = 1;
  } 
}

void stop_motor(int time_to_remain_idle) {
  if(time_to_remain_idle > 1) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    delay(time_to_remain_idle * 1000);
    motor_running = 0;
  }
}


/*
This function is reproduced as is from Arduino site => http://playground.arduino.cc/Main/InternalTemperatureSensor
*/
double getTemp(void) {
  unsigned int wADC;
  double t;

  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celcius.
  return (t);
}

