#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

volatile uint8_t digit = 0;
volatile bool running = true; // Flag to track if the counter is active

const uint8_t segmentMasks[10] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
    0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111
};

void displayDigit(uint8_t n) {
    uint8_t mask = segmentMasks[n];
    // Clear pins
    PORTB &= ~((1 << PB0) | (1 << PB1));
    PORTD &= ~((1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));

    // Map bits to ports (Check your wiring if mirrored!)
    if (mask & (1 << 0)) PORTB |= (1 << PB1);
    if (mask & (1 << 1)) PORTB |= (1 << PB0);
    if (mask & (1 << 2)) PORTD |= (1 << PD7);
    if (mask & (1 << 3)) PORTD |= (1 << PD6);
    if (mask & (1 << 4)) PORTD |= (1 << PD5);
    if (mask & (1 << 5)) PORTD |= (1 << PD4);
    if (mask & (1 << 6)) PORTD |= (1 << PD3);
}

// Timer1 Interrupt: Handles the counting every 0.5s
ISR(TIMER1_COMPA_vect) {
    if (running) {
        digit++;
        if (digit > 9) digit = 0;
        displayDigit(digit);
    }
}

// External Interrupt 0: Handles the button toggle on PD2
ISR(INT0_vect) {
    running = !running; // Toggle pause/resume
}

void setup() {
    // Outputs for display
    DDRB |= (1 << DDB0) | (1 << DDB1);
    DDRD |= (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
    
    // Input for Button (PD2 / INT0)
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2); // Enable Pull-up
    PORTD |= (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7);

    // External Interrupt Setup
    EICRA |= (1 << ISC01);  // Trigger INT0 on Falling Edge
    EIMSK |= (1 << INT0);   // Enable INT0

    // Timer1 Setup (0.5s intervals)
    TCCR1A = 0; 
    TCCR1B = (1 << WGM12) | (1 << CS12); 
    OCR1A = 31249; 
    TIMSK1 |= (1 << OCIE1A); 

    displayDigit(0);
    sei(); // Global interrupt enable
}

void loop() {
    // Main loop stays empty
}
