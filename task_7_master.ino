void setup() {
    // put your setup code here, to run once:
    DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

    PORTB |= (1 << PORTB2);
}

void loop() {
    // put your main code here, to run repeatedly:
    uint8_t vals[] = {85, 170, 255};

    for (uint8_t i = 0; i < 3; i++) {
        PORTB &= ~(1 << PORTB2);

        SPDR = vals[i];

        while (!(SPSR & (1 << SPIF)));
        PORTB |= (1 << PORTB2);
    }
}
