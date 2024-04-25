#include "Motor.h"
#include "XNucleo53L0A1.h"
#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// LiDAR Sensor, I2C sensor pins
Serial pc(USBTX, USBRX);
DigitalOut shdn(p20);
#define VL53L0_I2C_SDA p28
#define VL53L0_I2C_SCL p27
static XNucleo53L0A1 *board = NULL;

// Initialize variables to use in threads.
volatile int status;        // Status of LiDAR reported in read_distance()
volatile uint32_t distance; // Distance updated in read_distance()
volatile int mode = 1;      // 1-manual, 2-autonomous
float speed_set = 1.0f;     // Default multiplier retains original value.
volatile bool forward =
    false; // Used for autonomous safety stopping in manual mode.

// The robot is configured to have the long side facing forward, m1 is the left
// wheel and m2 is the right wheel:
Motor m1(p21, p6, p5); // pwm, fwd, rev
Motor m2(p22, p8, p7);

// MBED leds to display mode of robot:
BusOut myled(LED1, LED2, LED3, LED4);

// Bluetooth module:
Serial blue(p13, p14);

// Class for RGBLed to setup easy color configuration rate using the
// (255,255,255) -> (R,G,B).
class RGBLed {
public:
  RGBLed(PinName redPin, PinName greenPin, PinName bluePin)
      : _redPin(redPin), _greenPin(greenPin), _bluePin(bluePin) {
    _redPin.period(0.0005); // Setting PWM period for all pins might be needed
    _greenPin.period(0.0005);
    _bluePin.period(0.0005);
  }

  void write(float red, float green, float blue) {
    _redPin = red / 255.0;
    _greenPin = green / 255.0;
    _bluePin = blue / 255.0;
  }

private:
  PwmOut _redPin, _greenPin, _bluePin;
};

// RGB PWM pins
RGBLed led(p23, p24, p25);

void bluetooth(void const *argument) {
  // Motor: full-speed forward is -1.0, full-speed backward is 1.0
  char bnum = 0;
  char bhit = 0;
  while (1) {
    while (!blue.readable())
      Thread::yield();
    if (blue.getc() == '!') {
      if (blue.getc() == 'B') { // button data
        bnum = blue.getc();     // button number
        bhit = blue.getc();     // 1=hit, 0=release
        if (blue.getc() == char(~('!' + 'B' + bnum + bhit))) {
          switch (bnum) {
          case '1': // Sets mode=1 for manual driving
            if (bhit == '1') {
              m1.speed(0);
              m2.speed(0);
              mode = 1;
              myled = bnum - '0';
            }
            break;
          case '2': // Sets mode=2 for autonomous driving
            if (bhit == '1') {
              mode = 2;
              myled = bnum - '0';
            }
            break;
          case '3': // Button 3 toggles speed between slow to fast configuration
            if (bhit == '1' && mode == 1) {
              if (speed_set == 1.0f) {
                speed_set = 2.0f;
              } else {
                speed_set = 1.0f;
              }
            }
            break;
          case '4': // Sets mode=4, does nothing
            if (bhit == '1') {
              mode = 4;
              myled = bnum - '0';
            }
            break;
          case '5': // Button 5 moves robot forward
            if (bhit == '1' && mode == 1 && distance >= 125) {
              m1.speed(-0.5 * speed_set);
              m2.speed(-0.5 * speed_set);
              forward = true;
            } else {
              forward = false;
              m1.speed(0);
              m2.speed(0);
            }
            break;
          case '6': // Button 6 moves robot backward
            if (bhit == '1' && mode == 1) {
              m1.speed(0.5 * speed_set);
              m2.speed(0.5 * speed_set);
            } else {
              m1.speed(0);
              m2.speed(0);
            }
            break;
          case '7': // Button 7 rotates robot left
            if (bhit == '1' && mode == 1) {
              m1.speed(0.5 * speed_set);
              m2.speed(-0.5 * speed_set);
            } else {
              m1.speed(0);
              m2.speed(0);
            }
            break;
          case '8': // Button 8 rotates robot right
            if (bhit == '1' && mode == 1) {
              m1.speed(-0.5 * speed_set);
              m2.speed(0.5 * speed_set);
            } else {
              m1.speed(0);
              m2.speed(0);
            }
            break;
          default:
            break;
          }
        }
      }
    }
  }
}

int generateRandom(int t, int inc) { return t + (rand() % (inc - t + 1)); }

// Based upon the mode, autonomous either stops the robot when unsafe (mode=1)
// or moves robot around independently from the user with random turns.
void autonomous(void const *argument) {
  while (1) {
    if (mode == 1) {
      if (distance < 125 && forward) {
        m1.speed(0);
        m2.speed(0);
      }
    }
    if (mode == 2) {
      forward = false;
      while (distance < 125) {
        pc.printf("too close");
        m1.speed(0);
        m2.speed(0);
        wait(0.3);

        int rand_num = generateRandom(-1, 1);
        m1.speed(0.5 * rand_num);
        m2.speed(-0.5 * rand_num);

        // Determines how big a turn is.
        rand_num = generateRandom(5, 10);
        wait(0.1 * rand_num);

        m1.speed(0);
        m2.speed(0);
        wait(0.3);
      }
      m1.speed(-0.5);
      m2.speed(-0.5);
    }
  }
}

void read_distance(void const *argument) {
  DevI2C *device_i2c = new DevI2C(VL53L0_I2C_SDA, VL53L0_I2C_SCL);

  board = XNucleo53L0A1::instance(device_i2c, A2, D8, D2);
  shdn = 0; // Must reset sensor for an mbed reset to work
  wait(0.1);
  shdn = 1;
  wait(0.1);

  status = board->init_board();
  while (status) {
    pc.printf("Failed to init board! \r\n");
    status = board->init_board();
  }
  uint32_t temp_distance;
  while (1) {
    status = board->sensor_centre->get_distance(&temp_distance);
    if (status == VL53L0X_ERROR_NONE) {
      distance = temp_distance;
      pc.printf("D=%ld mm\r\n", distance);
    }
  }
}

void light_show(void const *argument) {
  while (1) {
    if (distance < 125) {
      // Various shades of green
      led.write(153, 0, 0);
      wait(0.1);
      led.write(204, 0, 0);
      wait(0.1);
      led.write(255, 51, 51);
      wait(0.1);
    } else {
      // Various shades of red
      led.write(0, 102, 0);
      wait(0.1);
      led.write(0, 204, 0);
      wait(0.1);
      led.write(51, 255, 153);
      wait(0.1);
    }
  }
}

int main() {
  myled = '1' - '0';
  Thread read_distance_thread(read_distance);
  Thread bluetooth_thread(bluetooth);
  Thread autonomous_thread(autonomous);
  Thread rgb_thread(light_show);
  while (1) {
    wait(0.1);
  }
}
