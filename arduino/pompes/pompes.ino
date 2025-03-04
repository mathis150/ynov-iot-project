#define activePompe 2

int used = false;

void setup() {
  Serial.begin(9600);
  pinMode(activePompe, INPUT);
}

void loop() {
  if(used == false) {
    Serial.println("Activation de la pompe");
  
    digitalWrite(activePompe, HIGH);
  
    delay(2000);
    
    digitalWrite(activePompe, LOW);

    used = true;
  }
}
