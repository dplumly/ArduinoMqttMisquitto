/*
  Basic MQTT example

  This sketch demonstrates the basic capabilities of the library.
  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

*/
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>


const int triggerPin = 2;
long debounceDelay = 200;
long lastDebounceTime = 0;        // the last time the output pin was toggled
boolean currentState = LOW;       //stroage for current button state
boolean lastState = LOW;          //storage for last button state

//================================================================
// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 107);
IPAddress server(192, 168, 0, 119);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

EthernetClient ethClient;
PubSubClient client(ethClient);

//================================================================
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//================================================================
void setup()
{
  Serial.begin(57600);

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);

  pinMode(triggerPin, INPUT_PULLUP);
}


//================================================================
void loop()
{
    currentState = digitalRead(triggerPin);
  
    if ( (millis() - lastDebounceTime) > debounceDelay) 
    {
      if (currentState == HIGH && lastState == LOW) 
      {        
        if (digitalRead(triggerPin) == HIGH) 
        {
          Serial.println(" Start experience");
          client.publish("outTopic", "Start experience"); // publish msg to broker
        }
      }
      lastDebounceTime = millis();          //set the current time
    }
  lastState = currentState;


if (!client.connected()) {
  reconnect();
}
client.loop();
}
