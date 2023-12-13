
#include <DFRobot_HX711.h>

DFRobot_HX711 MyScale(27, 32);

void setup() {
  Serial.begin(9600);
  
  MyScale.setCalibration(1992);
}

void loop() {
  // Get the weight of the object
  Serial.print(MyScale.readWeight(), 1);
  Serial.println("g");
  delay(200);
}
