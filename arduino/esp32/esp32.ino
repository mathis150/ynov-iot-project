#define solPin A0 // port de connexion du capteur
#define airPin A3 // port de connexion du capteur
#define luxPin A6 // port de connexion du capteur

int sol;
int air;
int lux;

void setup() {
  Serial.begin(115200);

   pinMode(solPin, INPUT);
   pinMode(airPin, INPUT);
   pinMode(luxPin, INPUT);
}
}

void loop() {
   sol = analogRead(solPin);
   air = analogRead(airPin);
   lux = analogRead(luxPin);
   
   float calc = ((float)sol / 1023) * 100;
   Serial.print("sol = ");
   Serial.print(calc); // afficher pourcentage
   Serial.print("%");
   Serial.print(" | Val = ");
   Serial.println(sol); // afficher variable
   Serial.print("air = ");
   Serial.println(air); // afficher pourcentage
   Serial.print("lux = ");
   Serial.println(lux); // afficher pourcentage

   delay(1000);
}
