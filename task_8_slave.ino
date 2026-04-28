// slave board

void setup() {
  // put your setup code here, to run once:
  DDRB |= (1 << PB0);
  DDRD &= ~(1 << PD2);
  
  TWAR = (0x08 << 1);
  
  DDRC &= ~((1 << PC4) | (1 << PC5));
  PORTC |= (1 << PORTC4) | (1 << PORTC5);

  TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (!(TWCR & (1 << TWINT)));

  uint8_t status = (TWSR & B11111000);

  switch (status) {
    case B01100000: 
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
      break;
    case B10000000: 
      if (TWDR == 1) {
        PORTB |= (1 << PB0);
      } else {
        PORTB &= ~(1 << PB0);
      }
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
      break;
    case B10101000: 
      if (PIND & (1 << PD2)) { 
        TWDR = 1; // Send "Pressed"
      } else {
        TWDR = 0; // Send "Released"
       }
       TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
       break;
    case B11000000: 
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
      break;
    case B10111000:
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
      break;
    case B10100000:
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
      break;
    default:
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
      break;
  }
}
