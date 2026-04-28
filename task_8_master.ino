// master board3

void setup() {
  // put your setup code here, to run once:
  DDRB |= (1 << PB0);
  DDRD &= ~(1 << PD2);
  
  TWSR &= ~((1 << TWPS0 | (1 << TWPS1)));
  TWBR = 72;

  DDRC &= ~((1 << PC4) | (1 << PC5));
  PORTC |= (1 << PORTC4) | (1 << PORTC5);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t button = (PIND & (1 << PD2)) ? 1 : 0;
  uint8_t slavebutton = 0;
  
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));

  TWDR = (0x08 << 1) | 0; // Load data
  TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
  while (!(TWCR & (1 << TWINT)));

  TWDR = button; // Load data
  TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
  while (!(TWCR & (1 << TWINT)));

  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));

  TWDR = (0x08 << 1) | 1; // Load data
  TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
  while (!(TWCR & (1 << TWINT)));

  TWCR = (1 << TWINT) | (1 << TWEN); 
  while (!(TWCR & (1 << TWINT)));

  slavebutton = TWDR;

  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

  if (slavebutton == 1) {
    PORTB |= (1 << PB0);  // LED ON
  } else {
    PORTB &= ~(1 << PB0); // LED OFF
  }
  delay(100);
}
