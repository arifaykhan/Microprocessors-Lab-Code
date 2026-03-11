#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

// --- FORWARD DECLARATIONS ---
extern "C" {
  void __attribute__((used, noinline, noreturn)) modeA_entry();
  void __attribute__((used, noinline, noreturn)) modeB_entry();
  void __attribute__((used, noinline, noreturn)) modeC_entry();
}

// --- GLOBALS ---
volatile uint8_t action_state = 0; 
volatile uint8_t action_idx = 0;   
bool led_target = false;           

void update_led() {
  if (action_state == 1) { 
    if (led_target) PORTB &= ~(1 << 5); else PORTB |= (1 << 5);
  } else if (action_state == 2) { 
    if (led_target) {
      PORTB |= (1 << 5); _delay_us(200);
      PORTB &= ~(1 << 5); _delay_us(800);
    } else {
      PORTB &= ~(1 << 5); _delay_us(1000);
    }
  } else { 
    if (led_target) PORTB |= (1 << 5); else PORTB &= ~(1 << 5);
  }
}

void run_delay(uint16_t ms) {
  for (uint16_t i = 0; i < ms; i++) {
    static bool last_btn = false;
    bool current_btn = (PINB & (1 << 0)) == 0; 

    if (current_btn && !last_btn) {
      asm volatile(
        "ldi r30, lo8(pm(JumpTable_%=)) \n\t"
        "ldi r31, hi8(pm(JumpTable_%=)) \n\t"
        "add r30, %[idx]               \n\t"
        "adc r31, __zero_reg__         \n\t" 
        "ijmp                          \n\t"
        "JumpTable_%=:                 \n\t"
        "rjmp Action0_%=               \n\t" 
        "rjmp Action1_%=               \n\t" 
        "rjmp Action2_%=               \n\t" 
        "rjmp Action3_%=               \n\t" 
        "Action0_%=: ldi %[state], 1   \n\t ldi %[idx], 1 \n\t rjmp Done_%= \n\t"
        "Action1_%=: ldi %[state], 2   \n\t ldi %[idx], 2 \n\t rjmp Done_%= \n\t"
        "Action2_%=: ldi %[state], 0   \n\t ldi %[idx], 3 \n\t rjmp Done_%= \n\t"
        "Action3_%=: ldi %[state], 0   \n\t ldi %[idx], 0 \n\t rjmp Done_%= \n\t"
        "Done_%=:                      \n\t"
        : [idx] "+r" (action_idx), [state] "+r" (action_state)
        : : "r30", "r31"
      );
      _delay_ms(50); 
    }
    last_btn = current_btn;
    update_led();
    if (action_state != 2) _delay_ms(1);
  }
}

extern "C" {
  void modeA_entry() { while (1) { led_target = 1; run_delay(800); led_target = 0; run_delay(800); } }
  void modeB_entry() { while (1) { led_target = 1; run_delay(150); led_target = 0; run_delay(150); led_target = 1; run_delay(150); led_target = 0; run_delay(800); } }
  void modeC_entry() { while (1) { led_target = 1; run_delay(50);  led_target = 0; run_delay(50); } }
}

void setup() {
  DDRB |= (1 << 5); DDRB &= ~(1 << 0); PORTB |= (1 << 0); 

  // Phase 1: Wait for press
  asm volatile(
    "1: sbic %0, 0 \n\t"
    "rjmp 1b       \n\t"
    : : "I" (_SFR_IO_ADDR(PINB))
  );

  PORTB |= (1 << 5); _delay_ms(200); PORTB &= ~(1 << 5);

  // Phase 2: Selection
  uint8_t presses = 1; 
  uint32_t start_time = millis();
  bool last_btn_state = false;
  while (millis() - start_time < 2000) {
    bool current_btn_state = (PINB & (1 << 0)) == 0; 
    if (current_btn_state && !last_btn_state) { presses++; _delay_ms(50); }
    last_btn_state = current_btn_state;
  }

  // Phase 3: The Jump (Revised to prevent garbage error)
  if (presses == 1)      asm volatile("jmp modeA_entry");
  else if (presses == 2) asm volatile("jmp modeB_entry");
  else                   asm volatile("jmp modeC_entry");
}

void loop() {}
