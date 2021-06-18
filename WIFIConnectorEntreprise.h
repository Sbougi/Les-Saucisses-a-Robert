#include <MQTT.h>

MQTTClient mqqtClient;  // Créer un client MQTT pour gérer les messages entre l'objet et le broker

// Commencement de la payload
String payload ="{";

const char mqttServer[] = "thingsboard.cloud"; // L'adresse IP du IDO ThingsBoard cloud (Broker) dans la classe
//const char mqttServer[] = "198.164.130.75"; // L'adresse IP du IDO ThingsBoard serveur (Broker) dans la classe
const int mqttServerPort = 1883; // MQTT port

const char key[] = "7tjjymR4rIemSUPToeJR"; // code d'acces
const char secret[] = ""; // broker secret
const char device[] = "2852db10-cf64-11eb-ae0e-1f8899a6f9b3"; // identifiant du broker




MQTTClient mqttClient;  // Creer un client MQTT pour transferer les messages en l'objet et le broker

// Fonction pour connecter le MQTT
void MQTTConnect() {
  
   mqqtClient.begin(mqttServer, mqttServerPort, wifiClient);
  Serial.print("Connecting to broker...");
  
  while (!mqqtClient.connect(device, key, secret)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConnected to MQTT!\n");

}

// Fonction pour preparer la payload qu'ons va envoyer
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

// Fonction pour envoyer la payload
void sendPayload()
{
  char attributes[300];
  payload += "}";
  payload.toCharArray(attributes, 300);
  mqqtClient.publish("v1/devices/me/telemetry", attributes);
  Serial.println(payload);
  payload="{";
  
}

// Fonction pour les details de la payload qu'ons envoie
void payloadDetails()
{

  Serial.print("payload -> ");
  Serial.println(payload);
  Serial.print("payload length -> ");
  Serial.println(payload.length());
  
}
