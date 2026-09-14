# Wiring

Disconnect the USB cable while building. Every module ground connects to Uno `GND`.

| Component | Module pin | Uno pin | Notes |
|---|---|---:|---|
| RC522 RFID | `3.3V` | `3.3V` | Never use 5V for this module. |
| RC522 RFID | `GND` | `GND` | |
| RC522 RFID | `RST` | `D9` | |
| RC522 RFID | `SDA` / `SS` | `D10` | This is SPI select, not Uno I²C SDA. |
| RC522 RFID | `MOSI` | `D11` | |
| RC522 RFID | `MISO` | `D12` | |
| RC522 RFID | `SCK` | `D13` | |
| Servo | signal (orange/yellow) | `D3` | |
| Servo | power (red) | `5V` | Start on USB power. See note below. |
| Servo | ground (brown/black) | `GND` | |
| Piezo buzzer | `+` | `D4` | Other pin to `GND`. |
| Red LED | anode via resistor | `D5` | Cathode to `GND`. |
| Green LED | anode via resistor | `D6` | Cathode to `GND`. |
| Keypad | `R1 R2 R3 R4` | `D2 D7 D8 A0` | Follow the ribbon order printed on the module. |
| Keypad | `C1 C2 C3 C4` | `A1 A2 A3 A4` | Uno analog pins also work as digital I/O. |

If the Uno resets while the servo moves, power the servo from a stable regulated **5V** source and connect that supply's ground to Uno ground. Do not use a 9V battery directly.
