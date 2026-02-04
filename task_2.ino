void setup() {
  Serial.begin(9600);
  Serial.println("Enter two numbers separated by a space or comma:");
}

void loop() {
  if (Serial.available() > 0) {
    int input1 = Serial.parseInt();
    int input2 = Serial.parseInt();

    while(Serial.available() > 0) { Serial.read(); }

    uint8_t a = (uint8_t)input1;
    uint8_t b = (uint8_t)input2;
    uint8_t res;
    uint8_t sreg;

    asm volatile(
      "mov r16, %[val1] \n\t"   // move a to r16
      "mov r17, %[val2] \n\t"   // move b to r17
      "sub r16, r17     \n\t"   // r16 - r17
      "in  %[sreg], __SREG__ \n\t" // capture status flags
      "mov %[res], r16  \n\t"   // move result to output variable
      : [res] "=r" (res), [sreg] "=r" (sreg)
      : [val1] "r" (a), [val2] "r" (b)
      : "r16", "r17"
    );

    Serial.print("Operation: "); Serial.print(a); Serial.print(" - "); Serial.println(b);
    Serial.print("Result: "); Serial.println(res);
    bool Z = (sreg >> 1) & 1; 
    bool C = (sreg >> 0) & 1;

    Serial.print("Z flag: "); Serial.println(Z);
    Serial.print("C flag: "); Serial.println(C);
    Serial.println("-----------------------");
    Serial.println("Enter two more numbers:");
  }
}
