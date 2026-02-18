#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t countdown = 0;

void setup() {

    DDRB |= (1 << DDB0);    //set pb0 as output
    PORTB &= ~(1 << PORTB0); //clear port pb0 just in case 
    
    DDRD &= ~(1 << DDD2);   //set pd2 as input

    EICRA |= (1 << ISC01); //external interrupt on falling edge
    EICRA &= ~(1 << ISC00); //clear bit 0
    EIMSK |= (1 << INT0);   //enable external interrupt

    TCCR1A = 0;             
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); //set prescaler 64 and enable CTC
    OCR1A = 100;             //value for 1 ms interrupt
    TIMSK1 |= (1 << OCIE1A); //enable output compare interrupt, i.e. interrupt to timer1_compa_vect on ctc

    sei(); //enable global interrupts

 }

void loop(){

 }

ISR(INT0_vect) {
    if (countdown == 0) {   //when button is pressed - external interrupt
        TCNT1 = 0; //clear tcnt1 to have clean 1ms cycles
        PORTB |= (1 << PORTB0); //turn on LED
        countdown = 10;   //set countdown
    }
}

ISR(TIMER1_COMPA_vect) { //triggered every 1ms
    if (countdown > 0) { //countdown will be > 0 only if the button is pressed
        countdown--;         
        if (countdown == 0) { //default state if the button is not pressed
            PORTB &= ~(1 << PORTB0); //toggle LED
        }
    }
}
