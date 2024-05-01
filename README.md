# ECE4180 Final Project - Dual-Mode Robot

## Team Members

- **Ivan Mix (Section A)**: imix3@gatech.edu
- **Nikola Rogers (Section A)**: nrogers39@gatech.edu

## Project Idea

In this project, our team has developed a versatile robot that operates under dual control modes: manual and autonomous. Equipped with a Bluetooth interface, the robot can be manually controlled through a dedicated app, allowing users to adjust its speed and direction. A real-time LiDAR sensor continuously monitors the distance to potential obstacles, enabling the robot to halt or navigate around barriers automatically. In its autonomous mode, the robot employs random directional changes to avoid collisions, enriching its capability to maneuver in complex environments. Additionally, the robot features an RGB LED system that indicates its operational status and proximity warnings.

## Video Demonstration

Please [click here](https://youtu.be/LskHHatkHdY) for our video demonstration.

## Pictures

### Top:

![Front](https://github.com/imix8/ECE4180_FinalProject/blob/main/Pictures/IMG_7470.jpg)

### Front:

![Front](https://github.com/imix8/ECE4180_FinalProject/blob/main/Pictures/IMG_7472.jpg)

### Back:

![Back](https://github.com/imix8/ECE4180_FinalProject/blob/main/Pictures/IMG_7471.jpg)
![Bottom](https://github.com/imix8/ECE4180_FinalProject/blob/main/Pictures/IMG_1810.jpg)

## Required Components

### Hardware:

- [2x MBED LPC1768](https://os.mbed.com/platforms/mbed-LPC1768/)
- [1x Breadboard - Full-Size](https://www.sparkfun.com/products/12615)
- [1x Breadboard - Half-Size](https://www.sparkfun.com/products/12002)
- [1x Servo - Hitec HS-422](https://www.sparkfun.com/products/11884)
- [1x Adafruit VL53L0X ToF sensor](https://www.adafruit.com/product/3317)
- [2x DC Barrel Jack Adapter](https://www.sparkfun.com/products/10811)
- [1x RGB LED](https://www.sparkfun.com/products/105)
- [3x 330 Ohm Resistor](https://www.sparkfun.com/products/8377)
- [1x Adafruit Bluefruit BLE UART friend module](https://os.mbed.com/users/4180_1/notebook/adafruit-bluefruit-le-uart-friend---bluetooth-low-/)
- [1x Shadow Chassis](https://www.sparkfun.com/products/13301)
- [1x Dual H-Bridge](https://www.sparkfun.com/products/14450)
- [2x Gearmotor - 140 RPM](https://www.sparkfun.com/products/13302)
- [2x Wheel - 65mm](https://www.sparkfun.com/products/13259)
- [Assorted Wires](https://www.sparkfun.com/products/124)
- [2x Battery Holder](https://www.sparkfun.com/products/12083)
- [8x AA Batteries](https://www.sparkfun.com/products/15201)

### Software:

- [arm KEIL Studio](https://studio.keil.arm.com/auth/login)
- Github
- Bluefruit Connect app for [iOS](https://apps.apple.com/app/id830125974) or [Android](https://play.google.com/store/apps/details?id=com.adafruit.bluefruit.le.connect&pcampaignid=web_share)

## Wiring Schematic

![Wiring Schematic](https://github.com/imix8/ECE4180_FinalProject/blob/main/Pictures/Wiring%20Schematic.png)

Please note that any pins not shown in the wiring schematic are unused and should not be connected to anything. Also please make sure to connect all ground rails in the breadboards together as failure to do so can cause issues.

## Code

As seen from the parts list and schematic above, we used two MBEDS in our project: the main MBED controls the driving and autonomous features of our robot, while the other controls the servo. Please click [here](https://github.com/imix8/ECE4180_FinalProject/blob/main/main_mbed/main.cpp) for the main MBED's code and [here](https://github.com/imix8/ECE4180_FinalProject/blob/main/servo_mbed/main.cpp) for the code used in the secondary MBED. Feel free to explore the folders as well which contain all of the libraries used, but the files linked previously in this section are the ones that we developed ourselves.

## Instructions

Now that you have acquired all of the parts you need, viewed our schematic, and read through our code you have everything you need to start building the robot! We reccomend that you start by connecting all of the components using our schematic, but don't worry too much about cable management at first. Then, once everything is wired you can take the bin files we have prepared (one for the [main MBED](https://github.com/imix8/ECE4180_FinalProject/blob/main/main_mbed.LPC1768.bin) and another for the [secondary MBED](https://github.com/imix8/ECE4180_FinalProject/blob/main/servo_mbed.LPC1768.bin)) and place them onto each of the MBEDs. Finally, install the Bluefruit Connect app on your phone in order to control the robot. You are now ready to use our dual-mode robot!
