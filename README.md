# Aegis Vault

A secure Arduino Uno micro-locker I am building with RFID/PIN authentication, a servo latch, LEDs, and a lockout alarm.

## Current build

The first goal is a reliable physical lock: an authorized RC522 RFID card or keypad PIN unlocks the servo for 10 seconds. Three failed attempts trigger a 30-second flashing red LED/buzzer lockout. Serial Monitor at 9600 baud records access events and prints card UIDs for enrollment.

## Exact hardware

- Arduino Uno R3
- RC522 RFID reader: 3.3V, GND, RST D9, SDA/SS D10, MOSI D11, MISO D12, SCK D13
- SG90 servo: signal D3, 5V, GND
- Piezo buzzer: D4 and GND
- Red LED + resistor: D5; green LED + resistor: D6
- 4x4 keypad: rows D2/D7/D8/A0; columns A1/A2/A3/A4

Important: RC522 is 3.3V only. Do not use the 9V battery directly with the servo.

## Continue from here

1. Create `src/aegis_vault.ino` with the firmware in the initial Codex workspace.
2. In Arduino IDE install MFRC522 and Keypad from Library Manager. Servo and SPI are built in.
3. Upload, open Serial Monitor at 9600, scan the intended card, then paste its uppercase UID into `AUTHORIZED_UID`. Change the starter PIN before a demo.
4. Build a cardboard/foam-board door; test servo angles before attaching the horn.
5. Next upgrades: HC-SR04 item detection, LCD through a 74HC595 shift register, RTC audit log, browser dashboard.

## Project voice

I describe it as: “I built Aegis Vault, a secure Arduino-powered micro-locker with RFID/PIN authentication, a servo-actuated latch, automatic re-locking, and a failed-access lockout system.”
