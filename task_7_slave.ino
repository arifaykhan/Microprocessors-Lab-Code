void setup() {
    // put your setup code here, to run once:
    DDRB |= (1 << DDB4);

    SPCR = (1 << SPE);

    UBRR0H = 0;
    UBRR0L = 103;
    UCSR0B = (1 << TXEN0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void loop() {
    // put your main code here, to run repeatedly:
    while (!(SPSR & (1 << SPIF)));

    uint8_t received = SPDR;

    if (received >= 100) uart_transmit((received / 100) + '0');
    if (received >= 10)  uart_transmit(((received / 10) % 10) + '0');
    uart_transmit((received % 10) + '0');
    uart_transmit('\n');
}
