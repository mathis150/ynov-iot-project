#define solPin A1
#define airPin A2
#define luxPin A3
#define RED 3
#define GREEN 5
#define BLUE 6

#include <SoftwareSerial.h>

SoftwareSerial arduinoSerial(2, 3); // RX sur D2, TX sur D3
int sol;
int air;
int lux;

void setup() {
  Serial.begin(115200);
  arduinoSerial.begin(9600);   // Communication avec l'ESP8266
  
  pinMode(solPin, INPUT);
  pinMode(airPin, INPUT);
  pinMode(luxPin, INPUT);
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
}

void loop() {
   
   sol = analogRead(solPin);
   air = analogRead(airPin);
   lux = analogRead(luxPin);
  
   analogWrite(RED, 255);
   analogWrite(BLUE, 255);
   analogWrite(GREEN, 255);
   
   float calc_sol = ((float)sol / 1023) * 100;
   Serial.print("sol = ");
   Serial.print(calc_sol); // afficher pourcentage
   Serial.print("% | Val = ");
   Serial.println(sol); // afficher variable
   float calc_air = ((float)air / 1023) * 100;
   Serial.print("air = ");
   Serial.print(calc_air); // afficher pourcentage
   Serial.print("% | Val = ");
   Serial.println(air); // afficher pourcentage
   float calc_lux = ((float)lux / 1023) * 100;
   Serial.print("lux = ");
   Serial.print(calc_lux); // afficher pourcentage
   Serial.print("% | Val = ");
   Serial.println(lux); // afficher pourcentage

   arduinoSerial.println("[{\"device\": \"raspberry_device\",\"data\": {\"lux\": "+ String(calc_lux) +",\"humidity_air\": "+ String(calc_air) +",\"humidity_sol\": "+ String(calc_sol) +"}}]");

   delay(1000);
}
