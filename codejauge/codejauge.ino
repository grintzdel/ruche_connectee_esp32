#include "DFRobot_HX711.h"


#define sck_capt1 16 // d27

#define dout_capt1 17 // d32



DFRobot_HX711 P_cafe(dout_capt1,sck_capt1);




void setup() {
 
 Serial.begin(9600);
  
Serial.println("ESP-32");
 
 // permet de tare la balance
  
P_cafe.setCalibration(11);

}



void loop() {
 
 Serial.print(P_cafe.readWeight(), 1);

 Serial.println(" g");

  delay(200);
}
