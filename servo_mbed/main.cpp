#include "Servo.h"
#include "mbed.h"

Servo myservo(p26);

int main() {
  // Sweep servo through its full range back and forth.
  while (1) {
    for (float p = 0; p <= 1.0; p += 0.1) {
      myservo = p;
      wait(0.2);
    }
    for (float p = 1.0; p >= 0; p -= 0.1) {
      myservo = p;
      wait(0.2);
    }
  }
}
