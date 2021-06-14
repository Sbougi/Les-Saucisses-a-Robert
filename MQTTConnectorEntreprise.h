#include <MQTT.h>

MQTTClient mqqtClient;  // Créer un client MQTT pour gérer les messages entre l'objet et le broker

// Commencement de la payload
String payload ="{";

const char mqttServer[] = "thingsboard.cloud"; // L'adresse IP du IDO ThingsBoard cloud (Broker) dans la classe
//const char mqttServer[] = "198.164.130.75"; // L'adresse IP du IDO ThingsBoard serveur (Broker) dans la classe
const int mqttServerPort = 1883; // broker mqtt port

const char key[] = "5CvmrGx8nxKvvltvpVGm"; // broker access toke.n
const char secret[] = ""; // broker secret
const char device[] = "79f481c0-cd35-11eb-8c1b-1520a2e6ced5"; // broker device identifier




MQTTClient mqttClient;  // Create a MQTT client to handle messagin between object and broker

void MQTTConnect() {
  
  mqqtClient.begin(mqttServer, mqttServerPort, wifiClient);
  Serial.print("Connecting to broker...");
  
  while (!mqqtClient.connect(device, key, secret)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConnected to MQTT!\n");

}

void appendPayload(String Name, String Val)
{

  if( payload != "{")
  {
    payload += ",";  
  }
  
  payload += "\"";
  payload += Name; 
  payload += "\": ";
  payload += Val;
  

  //Serial.println(payload);
  
}

void sendPayload()
{
  char attributes[200];
  payload += "}";
  payload.toCharArray(attributes, 200);
  mqqtClient.publish("v1/devices/me/telemetry", attributes);
  Serial.println(payload);
  payload="{";
  
}

void payloadDetails()
{

  Serial.print("payload -> ");
  Serial.println(payload);
  Serial.print("payload length -> ");
  Serial.println(payload.length());
  
}
