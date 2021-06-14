/*
  Auteur      : Soufiane Sabeg et Rhema Elias Mukadi Bolongo (Stagiaires)
  Programme   : Suivie des frigos
  Description : Ce programme vise a fonctionner plusieurs capteurs de température (DS18B20) en une seule pin, a but de realiser le travail demandé par notre client (Les saucisses a Robert) :
                  - Se connecter au Wifi et au MQTT (en utilisant le payload)
                  - Envoyer les valeurs a Thingsboard
                  - Lire sur un tableau de bord (Dashbord) crée dans le thingsboard.cloud et sur notre moniteur série les température recupérés
  Client      : Les saucisses a Robert
  Version     : 3
  Date        : 13 mai 2021
  
 */

// Librairie
#include "WIFIConnectorEntreprise.h"
#include "MQTTConnectorEntreprise.h"
#include <OneWire.h>
#include <DallasTemperature.h>



// Fil de signal est branché dans le port 2 de l'Arduino
#define ONE_WIRE_BUS 2
#define SWITCH 7

// Definir oneWire instance pour communiquer avec n'importe lequel OneWire appareil 
OneWire oneWire(ONE_WIRE_BUS);

// Passer notre oneWire reference a Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int numberOfDevices; // Numero des appareils trouvé

DeviceAddress tempDeviceAddress; //  On utilise cette variable pour stocker l'adresse d'appareil

String adresse ; 
String tempC ;
String state = "off" ;
unsigned long lastMillis = 0;  // Utilisé a la place du delais


void setup(void)
{
  
  // Démmarer la connexion WiFi et MQTT
  wifiConnect();
  MQTTConnect();
  // Commencer le moniteyr série
  Serial.begin(9600);
  
  // Commencement de la librairie
  sensors.begin();
  pinMode(SWITCH, INPUT);
  
  // Compter le nombre des capteurs
  numberOfDevices = sensors.getDeviceCount();
  
  // localiser les appareils trouvé
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Boucler et passer chaque capteur pour afficher son adresse
  for(int i=0;i<numberOfDevices; i++) {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}

void loop(void)
{ 
  
// Condition qui permet de savoir si on es connecté au Wifi et mqqt
if(WiFi.status()==3 && mqqtClient.connected())
 {
  
  sensors.requestTemperatures(); // Envoyer la commande pour avoir la temperature
  String value ; 
  // Boucler par tous les capteurs et afficher la temperature par adresse
  for(int i=0;i<numberOfDevices; i++) {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)){
    
    // Output the device ID
    Serial.print("Temperature for device: ");
    Serial.println(i,DEC);

    Serial.print ("Address : " ) ;
    adresse = printAddress(tempDeviceAddress) ; 
    
    Serial.print(adresse) ;
    
    Serial.print("\n") ;
    // Print the data
    tempC = String ( sensors.getTempC(tempDeviceAddress) );
    Serial.print("Temp C: ");
    Serial.println(tempC);
    //Serial.print(" Temp F: ");
    //Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit

    // Preparer l'envoie en reliant l'adresse du capteur et la temperature
    appendPayload(adresse , tempC  );   

      
    } 
      
  }
   
  Serial.println("");
  
   if(digitalRead(SWITCH) == LOW)
    state = "on";
  else state = "off";

    appendPayload("Porte", state);

    
  Serial.println("On envoie a Thingsboard...");
  Serial.println("");
  sendPayload();
  delay(2000);  
}


}

 //Fonction pour afficher l'adresse du capteur
String printAddress(DeviceAddress deviceAddress) {

  String var ; 
  
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
      var = String (deviceAddress[i])  ; 
  }

  return var ; 
  
}
