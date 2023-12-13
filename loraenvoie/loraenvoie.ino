#include <SPI.h>
#include <LoRa.h>

//On définit les PINS
#define ss 5
#define rst 4
#define dio0 2

int counter = 0;







#include "DHT.h"
#define DHTTYPE DHT22   
#define DHTPIN 14     // SIG pin
DHT dht(DHTPIN, DHTTYPE);
#if defined(ARDUINO_ARCH_AVR)
    #define debug  Serial

#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
    #define debug  SerialUSB
#else
    #define debug  Serial
#endif



void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");







  debug.begin(115200);
    debug.println("Démarage!");
    Wire.begin();

    /*if using WIO link,must pull up the power pin.*/
    // pinMode(PIN_GROVE_POWER, OUTPUT);
    // digitalWrite(PIN_GROVE_POWER, 1);

    dht.begin();

}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(10000);





  float temp_hum_val[2] = {0};
  
  
    if (!dht.readTempAndHumidity(temp_hum_val)) {
        debug.print("Humidité: ");
        debug.print(temp_hum_val[0]);
        debug.print(" %\t");
        debug.print("Température: ");
        debug.print(temp_hum_val[1]);
        debug.println(" *C");
    } else {
        debug.println("échec d'obtenir les valeurs.");
    }

    delay(1500);
}
