//use only one of these at a time

//part 1: digitalWrite 1 Hz blink
void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}

//part 2: register 1 Hz blink
void setup() {
  DDRB |= (1 << 5); 
}

void loop() {
  PORTB |= (1 << 5);  
  delay(500);
  PORTB &= ~(1 << 5);
  delay(500);
}

//part 3: digitalWrite fast blink
void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  digitalWrite(13, LOW);
}

//part 4: register fast blink
void loop() {
  PORTB |= (1 << 5);  
  PORTB &= ~(1 << 5);
}
