/*
  Auteur      : Soufiane Sabeg et Rhema Elias Mukadi Bolongo (Stagiaires)
  Programme   : Suivie des frigos
  Description : Ce programme vise a fonctionner plusieurs capteurs de température (DS18B20) en une seule pin, a but de realiser le travail demandé par notre client (Les saucisses a Robert) :
                  - Se connecter au Wifi et au MQTT (en utilisant le payload)
                  - Envoyer les valeurs a Thingsboard
                  - Lire sur un tableau de bord (Dashbord) crée dans le thingsboard.cloud et sur notre moniteur série les température recupérés
  Client      : Les saucisses a Robert
  Version     : Finale
  Date        : 13 mai 2021
  
 */

// Librairie
#include "MQTTConnectorEntreprise.h"
#include "WIFIConnectorEntreprise.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Definir les broches
#define ONE_WIRE_BUS 2 // Fil de signal est branché dans le port 2 de l'Arduino
#define SWITCH 3 // Pin connecté a reed switch

// Definir oneWire instance pour communiquer avec n'importe lequel OneWire appareil 
OneWire oneWire(ONE_WIRE_BUS);

// Passer notre oneWire reference a Dallas Temperature. 
DallasTemperature sensors(&oneWire);

DeviceAddress tempDeviceAddress; //  On utilise cette variable pour stocker l'adresse d'appareil

// Declaration des variables
int numberOfDevices; // Numero des appareils trouvé
String adresse; 
String tempC;
String state = "off";
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

  // Intialiser la switch
  pinMode(SWITCH, INPUT_PULLUP);
  
  // Compter le nombre des capteurs
  numberOfDevices = sensors.getDeviceCount();
  
  // Localiser les appareils trouvé
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
    
  // Boucler et passer chaque capteur pour afficher son adresse
  for(int i=0; i < numberOfDevices; i++) {
    
    // Chercher le fil pour l'adresse
    if(sensors.getAddress(tempDeviceAddress, i)) {
      
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } 
    else
    {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    } 
    
  } // Fin de condition Number of devices
  
} // Fin des crochets parametres

// Notre grande boucle loop
void loop(void)
{ 

// Condition qui permet de savoir si on es connecté au Wifi et mqqt
if(WiFi.status()==3 && mqqtClient.connected())
 {
  
  sensors.requestTemperatures(); // Envoyer la commande pour avoir la temperature
  
  String value; 
  
  // Boucler par tous les capteurs et afficher la temperature par adresse
  int proximity = digitalRead(SWITCH); // Lire le statut de la switch    
  
  // Deux condition qui prouve si la porte est ouverte ou fermé
  if (proximity == 0) // Si la pin lit low, la switch est fermé.
    {
      // Affecter OP pour OPENED
      state = "OP";
    }
    else
    {
      // Affecter CL pour CLOSED
      state = "CL";
    }
    
    // Preparer l'envoie du statut de la porte
    appendPayload("Porte", state);

    // Boucle qui permet de compter les capteurs branché et afficher leurs adresses
  for(int i=0; i < numberOfDevices; i++) {
    
    // Chercher le fil pour l'adresse
    if(sensors.getAddress(tempDeviceAddress, i)){
    
    // Afficher l'ID du capteur
    Serial.print("Temperature for device: ");
    Serial.println(i, DEC);
    
    // Afficher l'adresse
    Serial.print ("Address : "); 
    adresse = printAddress(tempDeviceAddress); 
    
    Serial.print(adresse);
    
    Serial.print("\n");
    
    // Afficher l'information
    tempC = String ( sensors.getTempF(tempDeviceAddress) ); // Pour une temperature en Celius c'est (getTempC)
    // Afficher la temperature lu en Farnhait
    Serial.print(" Temp F: ");
    Serial.println(tempC);
           
    // Preparer l'envoie en reliant l'adresse du capteur et la temperature
    appendPayload(adresse, tempC); 
     
    } // Fin de la condtion des adresses 
    
  } // Fin de boucle DS18 
    
  Serial.println("");
     
  Serial.println("On envoie a Thingsboard...");
  Serial.println("");

  // Envoie de l'information a Thingsboard
  sendPayload();
  delay(500);
  
  } // Fin de la connexion au wifi

} // Fin de la boucle loop


 //Fonction pour afficher l'adresse du capteur
String printAddress(DeviceAddress deviceAddress) {

  // Variable
  String var ; 

  // Boucle qui affiche les 8 cases pour chaque adresse complete
  for (uint8_t i = 0; i < 8 ; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
      var = String (deviceAddress[i])  ; 
  }
  
  return var ;  // Retourner l'adresse
  
}
