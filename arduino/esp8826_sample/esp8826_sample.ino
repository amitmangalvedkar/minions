#include <ESP8266WiFi.h> // WiFi Library
#include <PubSubClient.h> // MQTT library

const char *ssid =  "Gayatri06";   // cannot be longer than 32 characters!
const char *pass = "Saatvik17"; //

const char *mqtt_server = "l3l6wt.messaging.internetofthings.ibmcloud.com";
const int mqtt_port = 1883;
const char *mqtt_user = "use-token-auth";
const char *mqtt_pass = "password";
const char *mqtt_client_name = "d:l3l6wt:esp8266:esp8266_v1"; // Client connections cant have the same connection name
#define BUFFER_SIZE 100
const char *sub_topic = "iot-2/cmd/run/fmt/json";
const char *pub_topic = "iot-2/evt/counter/fmt/json";

unsigned long previousMillis = 0;
const long interval = 10000;   


WiFiClient wclient; //Declares a WifiClient Object using ESP8266WiFi
PubSubClient client(wclient); //instanciates client object
long lastMsg = 0;
char msg[50];
int value = 0;

void callback(const char* sub_topic, byte* payload, unsigned int length)
{
  Serial.println("Message arrived");
  Serial.println(sub_topic);

  for(int iCnt=0; iCnt < length; iCnt++)
  {
    Serial.print((char)payload[iCnt]);
    Serial.print(",");
  }

  Serial.println();
}

void setup() {
  // Setup console
  Serial.begin(115200);  //set the baud rate
  delay(10);
  Serial.println();
  Serial.println();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

void loop() {
  if (WiFi.status() != WL_CONNECTED) 
  {
    //wifi not connected?
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
        return;
    Serial.println("WiFi connected");
    Serial.println("IP = ");
    Serial.println(WiFi.localIP());

  }
  else
  {
    //Serial.println("WiFi Connected");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    //client object makes connection to server
    if (!client.connected()) 
    {
      Serial.println("Connecting to MQTT server");
      Serial.print(mqtt_server);
      Serial.print(mqtt_port);
      Serial.println("");
      //Authenticating the client object
      if (client.connect(mqtt_client_name,
                         mqtt_user,
                         mqtt_pass)) 
      {
        Serial.println("Connected to MQTT server");

        //Subscribe code
        client.subscribe(sub_topic); 
      }
      else 
      {
        Serial.println("Could not connect to MQTT server");   
      }
    }

    if (client.connected())
    {
      delay(1000);
      Serial.println("MQTT Connected--Looping");
      client.loop();
      long now = millis();
      if (now - lastMsg > 2000) {
        lastMsg = now;
        ++value;
        snprintf (msg, 75, "{\"d\":{\"value\":%ld}}", value);
        Serial.print("Publish message: ");
        Serial.print(msg);
        //Serial.println(" on %s", pub_topic);
        client.publish(pub_topic, msg);
      }     
    }

  }


} 
