# Build and setup

1. Build the circuit on the breadboard using [wiring.md](wiring.md).
2. In Arduino IDE, install `MFRC522` and `Keypad` via **Tools → Manage Libraries**.
3. Open `src/rfid_keypad_lock.ino`, choose **Arduino Uno**, and upload.
4. Open Serial Monitor at `9600` baud. Scan the RFID card I want to authorize; the sketch prints its UID.
5. Replace `REPLACE_WITH_YOUR_CARD_UID` with that UID. I also replace the starter `2048` PIN before demonstrating the project.
6. Upload again. Scan the authorized card, or type the PIN and press `#`. Press `A` at any time to lock immediately.

For the enclosure, I first confirm the servo's two angles without attaching the horn. Then I make a small cardboard door and position the horn so a tab catches the door edge while locked. If the servo strains, I adjust `LOCKED_ANGLE` and `UNLOCKED_ANGLE` in the sketch—never force it mechanically.

## Troubleshooting

- **RFID does not scan:** double-check 3.3V, `D10` for `SDA/SS`, and SPI pins `D11–D13`.
- **Keypad is scrambled:** the ribbon ordering differs between models. Update `rowPins`/`colPins` in the sketch instead of redoing the entire circuit.
- **Servo moves backward:** swap or tune its two angle constants.
- **Servo resets Uno:** use regulated 5V servo power with a common ground.
