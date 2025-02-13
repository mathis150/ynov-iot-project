#define solPin A1
#define airPin A2
#define luxPin A3
#define RED 3
#define GREEN 5
#define BLUE 6

int sol;
int air;
int lux;

void setup() {
  Serial.begin(9600);
  
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
   
   float calc = ((float)sol / 1023) * 100;
   Serial.print("sol = ");
   Serial.print(calc); // afficher pourcentage
   Serial.print("% | Val = ");
   Serial.println(sol); // afficher variable
   calc = ((float)air / 1023) * 100;
   Serial.print("air = ");
   Serial.print(calc); // afficher pourcentage
   Serial.print("% | Val = ");
   Serial.println(air); // afficher pourcentage
   calc = ((float)lux / 1023) * 100;
   Serial.print("lux = ");
   Serial.print(calc); // afficher pourcentage
   Serial.print("% | Val = ");
   Serial.println(lux); // afficher pourcentage

   delay(1000);
}
