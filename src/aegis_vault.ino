/* AEGIS VAULT — Arduino Uno secure micro-locker
   Libraries: MFRC522 by GithubCommunity; Keypad by Mark Stanley/Alexander Brevig */
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Servo.h>

// RC522: SPI pins are fixed on an Uno (SS=10, MOSI=11, MISO=12, SCK=13).
constexpr byte RFID_RST = 9, RFID_SS = 10;
MFRC522 rfid(RFID_SS, RFID_RST);

constexpr byte SERVO_PIN = 3, BUZZER_PIN = 4, RED_LED = 5, GREEN_LED = 6;
constexpr byte LOCKED_ANGLE = 10, UNLOCKED_ANGLE = 105;
constexpr unsigned long UNLOCK_MS = 10000, LOCKOUT_MS = 30000;
Servo latch;

const byte ROWS = 4, COLS = 4;
char keymap[ROWS][COLS] = {
  {'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 7, 8, A0};
byte colPins[COLS] = {A1, A2, A3, A4};
Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

// Change these before considering the prototype secure.
const char ACCESS_PIN[] = "2048";
// Scan a card with Serial Monitor open, then paste its upper-case UID here.
const char AUTHORIZED_UID[] = "9DBCC101";

String enteredPin;
bool unlocked = false, alarmOn = false;
byte failures = 0;
unsigned long unlockedAt = 0, lockoutUntil = 0, lastAlarmToggle = 0;

void setLocked(bool shouldLock, const __FlashStringHelper* reason) {
  unlocked = !shouldLock;
  latch.write(shouldLock ? LOCKED_ANGLE : UNLOCKED_ANGLE);
  digitalWrite(RED_LED, shouldLock ? HIGH : LOW);
  digitalWrite(GREEN_LED, shouldLock ? LOW : HIGH);
  digitalWrite(BUZZER_PIN, LOW);
  alarmOn = false;
  Serial.print(shouldLock ? F("LOCKED: ") : F("UNLOCKED: "));
  Serial.println(reason);
}

void grantAccess(const __FlashStringHelper* reason) {
  if (millis() < lockoutUntil) return;
  failures = 0;
  setLocked(false, reason);
  unlockedAt = millis();
  tone(BUZZER_PIN, 1200, 100);
}

void rejectAccess(const __FlashStringHelper* reason) {
  failures++;
  Serial.print(F("ACCESS DENIED: "));
  Serial.println(reason);
  tone(BUZZER_PIN, 330, 180);
  delay(200); // brief feedback only
  if (failures >= 3) {
    setLocked(true, F("security lockout"));
    lockoutUntil = millis() + LOCKOUT_MS;
    failures = 0;
    Serial.println(F("ALARM: three failed attempts; locked out for 30 seconds."));
  }
}

String scannedUID() {
  String uid;
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += '0';
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

void readRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;
  String uid = scannedUID();
  Serial.print(F("RFID scanned: "));
  Serial.println(uid);
  if (uid == AUTHORIZED_UID) grantAccess(F("authorized RFID card"));
  else rejectAccess(F("unknown RFID card"));
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void readKeypad() {
  char key = keypad.getKey();
  if (!key) return;
  if (key >= '0' && key <= '9') {
    if (enteredPin.length() < 8) enteredPin += key;
    Serial.print('*'); // do not print a real PIN to the log
  } else if (key == '*') {
    enteredPin = "";
    Serial.println(F("\nPIN cleared"));
  } else if (key == '#') {
    Serial.println();
    if (enteredPin == ACCESS_PIN) grantAccess(F("correct keypad PIN"));
    else rejectAccess(F("incorrect keypad PIN"));
    enteredPin = "";
  } else if (key == 'A') {
    setLocked(true, F("manual keypad lock"));
  }
}

void updateAlarm() {
  if (millis() >= lockoutUntil) {
    if (lockoutUntil != 0) {
      lockoutUntil = 0;
      setLocked(true, F("lockout ended"));
    }
    return;
  }
  if (millis() - lastAlarmToggle >= 250) {
    lastAlarmToggle = millis();
    alarmOn = !alarmOn;
    digitalWrite(RED_LED, alarmOn);
    digitalWrite(BUZZER_PIN, alarmOn);
  }
}

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  latch.attach(SERVO_PIN);
  SPI.begin();
  rfid.PCD_Init();
  setLocked(true, F("system boot"));
  Serial.println(F("AEGIS VAULT READY — scan card or enter PIN then #. Press A to lock."));
}

void loop() {
  updateAlarm();
  if (millis() < lockoutUntil) return;
  readRFID();
  readKeypad();
  if (unlocked && millis() - unlockedAt >= UNLOCK_MS)
    setLocked(true, F("automatic re-lock"));
}
