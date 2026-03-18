volatile uint8_t counter; 

uint8_t eeprom_read_asm() {
  uint8_t result;
  asm volatile (
    "1: sbic %[eecr], %[eepe] \n\t"
    "rjmp 1b                 \n\t"
    "out %[eearl], __zero_reg__ \n\t"
    "sbi %[eecr], %[eere]    \n\t"
    "in %[res], %[eedr]      \n\t"
    : [res] "=d" (result)
    : [eecr] "I" (_SFR_IO_ADDR(EECR)), [eearl] "I" (_SFR_IO_ADDR(EEARL)),
      [eedr] "I" (_SFR_IO_ADDR(EEDR)), [eepe] "I" (EEPE), [eere] "I" (EERE)
  );
  return result;
}

void eeprom_write_asm(uint8_t val) {
  asm volatile (
    "2: sbic %[eecr], %[eepe] \n\t"
    "rjmp 2b                 \n\t"
    "out %[eearl], __zero_reg__ \n\t"
    "out %[eedr], %[data]   \n\t"
    "cli                     \n\t"
    "sbi %[eecr], %[eempe]   \n\t"
    "sbi %[eecr], %[eepe]    \n\t"
    "sei                     \n\t"
    : : [data] "d" (val), [eecr] "I" (_SFR_IO_ADDR(EECR)),
      [eearl] "I" (_SFR_IO_ADDR(EEARL)), [eedr] "I" (_SFR_IO_ADDR(EEDR)),
      [eepe] "I" (EEPE), [eempe] "I" (EEMPE)
  );
}

void setup() {
  Serial.begin(9600);
  
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); 
  OCR1A = 15624;

  counter = eeprom_read_asm();
  Serial.print("Boot Value: "); Serial.println(counter);
}

void loop() {
  bool secondPassed = false;
  uint8_t temp_reg;

  asm volatile (
    "in   %[temp], %[tifr]       \n\t" // Read flags
    "sbrs %[temp], %[ocf_bit]    \n\t" // Skip if 1 second has passed
    "rjmp end_asm                \n\t" 
    "inc  %[cnt]                 \n\t" // Increment register
    "ldi  %[temp], (1<<%[ocf_bit]) \n\t" 
    "out  %[tifr], %[temp]       \n\t" // Clear flag
    "ldi  %[passed], 1           \n\t" // Signal C++ that a second passed
    "end_asm:                    \n\t"
    : [cnt] "+d" (counter), [temp] "=&d" (temp_reg), [passed] "+d" (secondPassed)
    : [tifr] "I" (_SFR_IO_ADDR(TIFR1)), [ocf_bit] "I" (OCF1A)
  );

  if (secondPassed) {
    Serial.print("Live Count: ");
    Serial.println(counter);
  }

  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'S' || cmd == 's') {
      eeprom_write_asm(counter);
      Serial.print("Saved: "); Serial.println(counter);
    } else if (cmd == 'R' || cmd == 'r') {
      counter = 0;
      eeprom_write_asm(0);
      Serial.println("Reset to 0.");
    }
  }
}
