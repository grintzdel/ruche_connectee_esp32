// Librairies MQTT
#include <WiFi.h>
#include <PubSubClient.h>

// Connexion au Wifi
//const char *ssid_Router = "STI2D-SIN"; //Enter the router name
//const char *password_Router = "SIN2020-2021"; //Enter the router password
const char *ssid_Router = "nomWifi"; //Enter the router name
const char *password_Router = "mdpWifi"; //Enter the router password




// MQTT
const char *mqtt_server = "test.mosquitto.org";
const int mqttPort = 1883;
long tps=0;

WiFiClient espClient;
PubSubClient client(espClient);

// Jauges de contrainte
#include "DFRobot_HX711.h"
#define sck_capt1 16 // d27
#define dout_capt1 17 // d32
DFRobot_HX711 P_cafe(dout_capt1,sck_capt1);
float poids;
int poids_final;

// Capteur de luminosité
#include <Wire.h>
#include "Arduino.h"
#include "SI114X.h"
SI114X SI1145 = SI114X();

// Capteur de température et d'humidité
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

String humidite;
String temperature;




void setup_wifi(){
  WiFi.begin(ssid_Router, password_Router);
  Serial.print("\nWaiting for WiFi... ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);
}
void setup_mqtt(){
  client.setServer(mqtt_server, mqttPort);
  reconnect();
}

void reconnect(){
  while (!client.connected()) {
    Serial.println("Connection au serveur MQTT ...");
    if (client.connect("ESP32Client")) {
      Serial.println("MQTT connecté");
    }
    else {
      Serial.print("echec, code erreur= ");
      Serial.println(client.state());
      Serial.println("nouvel essai dans 20s");
    delay(20000);
    }
  }
}
//Fonction pour publier un float sur un topic 
  void mqtt_publish(String topic, float t){
  char top[topic.length()+1];
  topic.toCharArray(top,topic.length()+1);
  char t_char[50];
  String t_str = String(t);
  t_str.toCharArray(t_char, t_str.length() + 1);
  client.publish(top,t_char);
  }


void setup() {
    // Jauges de contrainte
    Serial.begin(115200);
    Serial.println("ESP-32"); 
    P_cafe.setCalibration(11);
    Serial.begin(115200);
    
    // le capteur de luminosité
    Serial.println("Beginning Si1145!");
    while (!SI1145.Begin()) {
        Serial.println("Si1145 is not ready!");
        delay(1000);
    }
    Serial.println("Si1145 is ready!"); 
    
    //température et humidité 
    debug.begin(115200);
    debug.println("Démarage!");
    Wire.begin();
    dht.begin();
    
  //mqtt wifi
  Serial.begin(115200);
  setup_wifi();
  setup_mqtt();
  client.publish("my/mqtt/topic", "Hello from ESP32");
  
}
     


void loop() {
    Serial.println("//--------------------------------------//\r\n");
    Serial.println("\n");
   
    //balance
    poids = P_cafe.readWeight(), 1;
    poids_final = poids;
    poids_final =  poids_final/100;
    poids_final =  poids_final*100;
    Serial.print("poids : ");
    Serial.print(poids_final);
    Serial.println(" g");

    //lumière
    Serial.print("Visible: "); 
    Serial.println(SI1145.ReadVisible());
    Serial.print("Infrarouge: "); 
    Serial.println(SI1145.ReadIR());
    //La vraie valeur des UV doit être divisée par 100
    Serial.print("UV: ");  
    Serial.println((float)SI1145.ReadUV() / 100);
    delay(1000);

    //température et humidité
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

    float temperature = temp_hum_val[1];
    float humidite = temp_hum_val[0];
    float uv = SI1145.ReadUV();
    uv = uv/ 100;
    float ir = SI1145.ReadIR();
    float visible = SI1145.ReadVisible();

   // envoie des données
   reconnect();
   client.loop(); 
   if (millis()-tps>30000){
     tps=millis();
     reconnect();
     mqtt_publish("my/mqtt/topic", poids_final);
     Serial.println("\n envoie du poids sur le broker");
     delay(30000);
     reconnect();
     mqtt_publish("my/mqtt/topic", temperature);
     Serial.println("\n envoie de la temperature sur le broker");
     delay(30000);
     reconnect();
     mqtt_publish("my/mqtt/topic", humidite);
     Serial.println("\n envoie de l'humidité sur le broker");
     delay(30000);
     reconnect();
     mqtt_publish("my/mqtt/topic", uv);
     Serial.println("\n envoie des uv sur le broker");
     delay(30000);
     reconnect();
     mqtt_publish("my/mqtt/topic", ir);
     Serial.println("\n envoie des ir sur le broker");
     delay(30000);
     reconnect();
     mqtt_publish("my/mqtt/topic", visible);
     Serial.println("\n envoie du visible sur le broker");
     delay(30000);
    }
    
}
