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

    debug.begin(115200);
    debug.println("Démarage!");
    Wire.begin();

    /*if using WIO link,must pull up the power pin.*/
    // pinMode(PIN_GROVE_POWER, OUTPUT);
    // digitalWrite(PIN_GROVE_POWER, 1);

    dht.begin();
}

void loop() {
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
