# 45-second demo script

> This is Aegis Vault, a secure micro-locker I built with an Arduino Uno. It uses two forms of authentication: an RFID card and a keypad PIN.

1. Begin with the door closed, red LED on, and Serial Monitor visible.
2. Scan an unknown card or enter a wrong PIN three times. Point out the red flashing lockout and buzzer.
3. Wait for lockout to end, then scan the authorized card. Show the green LED, confirmation tone, servo latch release, and serial log.
4. Close the door and press `A` on the keypad to lock it again.
5. End with the enclosure and wiring in frame: “Next I’m adding item detection, an LCD status panel, and timestamped audit logs.”
