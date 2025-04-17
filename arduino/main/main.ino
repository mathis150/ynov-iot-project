#define airPin A1
#define solPin A2
#define luxPin A3
#define gazPin A4
#define thermaPin A5

#define RED 5
#define GREEN 6
#define POMPE 7
#define BLUE 9

#define SERIES_RESISTOR 100000.0  // Résistance fixe de 100 kΩ
#define VCC 5.0  // Tension d'alimentation Arduino

#define A 0.0009032679
#define B 0.000248772
#define C 0.0000002041094

#include <SoftwareSerial.h>

bool pompeActive = false;

SoftwareSerial arduinoSerial(2, 3); // RX sur D2, TX sur D3
int sol;
int air;
int lux;
int gaz;
int rawValue;

void setup() {
  Serial.begin(115200);  // Debug sur le moniteur série
  arduinoSerial.begin(4800);   // Communication avec l'ESP8266
  
  pinMode(solPin, INPUT);
  pinMode(airPin, INPUT);
  pinMode(luxPin, INPUT);
  pinMode(gazPin, INPUT);
  pinMode(thermaPin, INPUT);
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(POMPE, OUTPUT);
  
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(POMPE, LOW);
  
  analogWrite(RED, 0);
  analogWrite(BLUE, 0);
  analogWrite(GREEN, 0);
}

void loop() {
   
   sol = analogRead(solPin);
   air = analogRead(airPin);
   lux = analogRead(luxPin);
   gaz = analogRead(gazPin);
   rawValue = analogRead(thermaPin);
   
   float voltage = (rawValue / 1023.0) * VCC;
   float resistance = SERIES_RESISTOR * (voltage / (VCC - voltage));
   float lnR = log(resistance);
   float temperatureKelvin = 1.0 / (A + (B * lnR) + (C * lnR * lnR * lnR));
   
   float calc_sol = ((float)sol / 1023) * 100;
   float calc_air = ((float)air / 1023) * 100;
   float calc_lux = ((float)lux / 1023) * 100;
   float calc_gaz = ((float)gaz / 1023) * 100;
   float calc_temp = ((3950.0 / (log(resistance / 100000.0) + (3950.0 / 298.15))) - 273.15) * -1;

   if(calc_lux < 25) {
     analogWrite(RED, 100);
     analogWrite(BLUE, 100);
     analogWrite(GREEN, 100);
   } else {
     analogWrite(RED, 0);
     analogWrite(BLUE, 0);
     analogWrite(GREEN, 0);
   }

   //Information capteur d'humidité des sols
   Serial.print("sol = ");
   Serial.print(calc_sol); // afficher pourcentage
   Serial.print("% | Val = ");
   Serial.println(sol); // afficher variable

   //Informations capteur d'humidité de l'air
   Serial.print("air = ");
   Serial.print(calc_air); // afficher pourcentage
   Serial.print("% | Val = ");
   Serial.println(air); // afficher pourcentage

   //Informations capteur de lux
   Serial.print("lux = ");
   Serial.print(calc_lux); // afficher pourcentage
   Serial.print("% | Val = ");
   Serial.println(lux); // afficher pourcentage

   //Informations capteur de lux
   Serial.print("gaz = ");
   Serial.print(calc_gaz); // afficher pourcentage
   Serial.print("% | Val = ");
   Serial.println(gaz); // afficher pourcentage
   
   //Informations capteur de température
   Serial.print("temp = ");
   Serial.print(calc_temp);
   Serial.println("°C");
   
   // Construction propre du JSON
   String jsonData = "{\"device\": \"raspberry_device\", \"data\": {";
   jsonData += "\"lux\": " + String(calc_lux, 2) + ",";
   jsonData += "\"humidity_air\": " + String(calc_air, 2) + ",";
   jsonData += "\"humidity_sol\": " + String(calc_sol, 2) + ",";
   jsonData += "\"gaz\": " + String(calc_gaz, 2) + ",";
   jsonData += "\"thermal\": " + String(calc_temp, 2);
   jsonData += "}}";

   if(calc_sol <= 35 && !pompeActive) {
    pompeActive = true;
    digitalWrite(POMPE, HIGH);
   Serial.println("Pompage...");
   } else if(calc_sol >= 45 && pompeActive) {
    pompeActive = false;
    digitalWrite(POMPE, LOW);
   Serial.println("Pompage désactivé...");
   }

   // Affichage pour le debug
   Serial.println("Envoi JSON: " + jsonData);

   // Envoi des données à l'ESP8266
   arduinoSerial.println(jsonData);
   
   delay(1000);
}
