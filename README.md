# Aegis Vault

**Aegis Vault is a secure, Arduino-powered micro-locker I built to explore physical security, embedded software, and human-friendly hardware design.** It authenticates people with either an RFID card or a keypad PIN, physically unlocks with a servo latch, and enters a visible/audio lockout after repeated failed attempts.

I designed it as a small prototype for use cases such as equipment checkout, dorm-room storage, or controlled makerspace supplies. The goal was to make a security system that is tangible enough to demo: scan a card, hear the confirmation tone, watch the lock release, and see the system defend itself from repeated bad attempts.

## What it does

- Unlocks with an authorized RC522 RFID card or a configurable keypad PIN
- Drives a servo-powered latch and automatically re-locks after 10 seconds
- Uses red/green LEDs and a piezo buzzer for immediate feedback
- Locks down for 30 seconds after three failed attempts
- Prints security events and scanned card UIDs to Serial Monitor at 9600 baud

## Build highlights

- Arduino C++ firmware built around a simple non-blocking security state machine
- SPI-based RFID integration and 4×4 matrix keypad input
- Servo actuator control and physical cardboard/foam-board enclosure design
- Configurable authorization credentials, timing, and servo angles
- A deliberately modular architecture, so I can add occupancy sensing, an LCD, an RTC audit log, and a laptop dashboard next

## Hardware

This project uses components in the Elegoo Uno R3 starter kit:

- Arduino Uno R3
- RC522 / MFRC522 RFID reader plus card or key fob
- 4×4 membrane keypad
- SG90 micro servo
- Piezo buzzer, red LED, green LED, and two 220–330 Ω resistors
- Breadboard and jumper wires

Full pin-by-pin wiring is in [docs/wiring.md](docs/wiring.md). Assembly and setup instructions are in [docs/build.md](docs/build.md).

## Quick start

1. Install the Arduino IDE libraries **MFRC522** and **Keypad** from Library Manager.
2. Wire the hardware using [the wiring guide](docs/wiring.md).
3. Upload [src/aegis_vault.ino](src/aegis_vault.ino) to the Uno.
4. Open Serial Monitor at `9600` baud and scan the intended card once.
5. Copy the printed UID into `AUTHORIZED_UID` in the sketch and choose a new `ACCESS_PIN`.
6. Upload again and test the lock before mounting the servo horn to the enclosure.

## Demo

See [docs/demo-script.md](docs/demo-script.md) for the short demo sequence I use to show the system end-to-end.

## Safety note

This is a low-voltage prototype, not a certified security product. The RC522 must run at **3.3V**. Do not connect the included 9V battery directly to the servo or RFID reader; a servo that causes USB resets needs a regulated external 5V supply with a shared ground.

## Next steps

- Detect whether a stored item was collected with the HC-SR04 ultrasonic sensor
- Add RTC-backed audit events and a 1602 LCD status screen
- Stream serial events into a browser dashboard for inventory and access monitoring

## License

MIT © 2026 Ashton Jiang
