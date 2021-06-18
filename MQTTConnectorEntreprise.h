// Connection en utilisant le wifi local

// Librairies
#include <WiFiNINA.h>
#include "wifiStatus.h"

// Informations de la connexion au wifi chez le client
char ssid[] = "Objets_IDO";  // Wifi SSID (name)
char user[] = "";
char pass[] = "42Bidules!";  // Wifi mot de passe chez le client

int status = WL_IDLE_STATUS;     // the Wifi radio's status


WiFiClient wifiClient; 

// Fonction pour connecter au reseau wifi
void wifiConnect()
{
    
  // Essayer de se connecter au wifi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
   
    status = WiFi.begin(ssid, pass);
    Serial.println(status);

    // Attendre 10 secondes pour la connexion
    delay(10000);
  }

  // Afficher pour confirmer qu'ons est connecté
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

}
