// Connection using WPA2 Enterpriser WIFI network


#include <WiFiNINA.h>
//#include <SPI.h>
//#include <WiFi101.h>
#include "wifiStatus.h"

//please enter your sensitive data for log on

char ssid[] = "Objets_IDO";  // your WPA2 enterprise network SSID (name)
char user[] = "";  // your WPA2 enterprise username
char pass[] = "42Bidules!";  // your WPA2 enterprise password

int status = WL_IDLE_STATUS;     // the Wifi radio's status


WiFiClient wifiClient; 

void wifiConnect()
{
    
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
   
    status = WiFi.begin(ssid, pass);
    //status = WiFi.beginEnterprise(ssid, user, pass);
    Serial.println(status);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

}
