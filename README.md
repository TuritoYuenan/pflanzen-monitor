# PflanzenMonitor

This is an IoT project for setting up an automatic plant monitoring system. The project consists of three parts:

- Hardware: ESP32-C3 microcontroller, DHT sensor, Light sensor (LDR), Soil moisture sensor and LCD screen.
- Software (in C++) to operate the hardware reading, displaying and sending the sensor values to Blynk.
- User Interface using Blynk (private).

> See [HARDWARE.md](HARDWARE.md) for list of hardware components

## Features

- Read temperature, humidity, lightness and soil moisture values
- Display the values on an LCD screen
- Send the values to Blynk
- Power efficient: Sleep after 60 seconds, then wake up 60 seconds later

## Usage

1. Create a new Blynk device with 4 datastreams for temperature, humidity, lightness and soil moisture.
2. Create a secret file by copying `secrets.example.h`, rename as `secrets.h` then fill in Blynk and Wifi info.
3. Load necessary files to Wokwi and run to simulate the hardware. Alternatively, you can use the Arduino IDE/CLI to compile and load to real hardware (similar to Wokwi's visual).

## Credit

- Author: N.T. Minh-Triet (me) - K5 Computer Science student
- Supervisor: Dr P.V. Dai - Lecturer @ Swinburne University, Vietnam
