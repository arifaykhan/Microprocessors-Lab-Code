void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}

void setup() {
  DDRB |= (1 << 5); 
}

void loop() {
  PORTB |= (1 << 5);  
  delay(500);
  PORTB &= ~(1 << 5);
  delay(500);
}
