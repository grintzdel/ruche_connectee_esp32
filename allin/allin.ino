//Librairies des jauges de contrainte
#include "DFRobot_HX711.h"
#define sck_capt1 16 // d27
#define dout_capt1 17 // d32
DFRobot_HX711 P_cafe(dout_capt1,sck_capt1);
float poids;
//Librairies du capteur de luminosité
#include <Wire.h>
#include "Arduino.h"
#include "SI114X.h"
SI114X SI1145 = SI114X();



void setup() {
    // jauges de contrainte
    Serial.begin(115200);
    Serial.println("ESP-32"); 
    P_cafe.setCalibration(11);
    Serial.begin(115200);
    
    // le capteure de luminosité
    Serial.println("Beginning Si1145!");
    while (!SI1145.Begin()) {
        Serial.println("Si1145 is not ready!");
        delay(1000);
    }
    Serial.println("Si1145 is ready!");


}



void loop() {
    //jauge de contrainte
    Serial.println("\n");
    Serial.print(P_cafe.readWeight(), 1);
    //poids = P_cafe.readWeight(), 1;
    //int poids = poids/50;
    //poids = poids*50;
    //Serial.println(poids);
    Serial.println(" g");
    delay(200);

    //capteur de lunimosité
    Serial.print("//--------------------------------------//\r\n");
    Serial.print("Visible: "); 
    Serial.println(SI1145.ReadVisible());
    Serial.print("Infrarouge: "); 
    Serial.println(SI1145.ReadIR());
    //La vraie valeur d'UV doit être divisée par 100
    Serial.print("UV: ");  
    Serial.println((float)SI1145.ReadUV() / 100);
    delay(1000);
}
