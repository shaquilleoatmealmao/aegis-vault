#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Servo.h>

// Install MFRC522 and Keypad from Arduino Library Manager.
const byte RFID_RST=9, RFID_SS=10, SERVO_PIN=3, BUZZER=4, RED=5, GREEN=6;
const byte LOCKED=10, UNLOCKED=105;
const unsigned long UNLOCK_MS=10000, LOCKOUT_MS=30000;
MFRC522 rfid(RFID_SS, RFID_RST);
Servo latch;

const byte ROWS=4, COLS=4;
char keys[ROWS][COLS]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
byte rowPins[ROWS]={2,7,8,A0};
byte colPins[COLS]={A1,A2,A3,A4};
Keypad keypad=Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);

// Change these before demoing the project.
const char ACCESS_PIN[]="2048";
const char AUTHORIZED_UID[]="REPLACE_WITH_YOUR_CARD_UID";
String pin;
bool unlocked=false, alarm=false;
byte failures=0;
unsigned long unlockedAt=0, lockoutUntil=0, lastBlink=0;

void lock(bool state, const __FlashStringHelper* why) {
  unlocked=!state;
  latch.write(state ? LOCKED : UNLOCKED);
  digitalWrite(RED,state); digitalWrite(GREEN,!state); digitalWrite(BUZZER,LOW);
  Serial.print(state?F("LOCKED: "):F("UNLOCKED: ")); Serial.println(why);
}
void grant(const __FlashStringHelper* why) {
  if(millis()<lockoutUntil) return;
  failures=0; lock(false,why); unlockedAt=millis(); tone(BUZZER,1200,100);
}
void deny(const __FlashStringHelper* why) {
  Serial.print(F("ACCESS DENIED: ")); Serial.println(why); tone(BUZZER,330,180); failures++;
  if(failures>=3) { lock(true,F("security lockout")); failures=0; lockoutUntil=millis()+LOCKOUT_MS; }
}
String uid() {
  String result;
  for(byte i=0;i<rfid.uid.size;i++) { if(rfid.uid.uidByte[i]<0x10) result+='0'; result+=String(rfid.uid.uidByte[i],HEX); }
  result.toUpperCase(); return result;
}
void scanRFID() {
  if(!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;
  String card=uid(); Serial.print(F("RFID scanned: ")); Serial.println(card);
  if(card==AUTHORIZED_UID) grant(F("authorized RFID card")); else deny(F("unknown RFID card"));
  rfid.PICC_HaltA(); rfid.PCD_StopCrypto1();
}
void scanKeypad() {
  char key=keypad.getKey(); if(!key) return;
  if(key>='0' && key<='9') { if(pin.length()<8) pin+=key; Serial.print('*'); }
  else if(key=='*') { pin=""; Serial.println(F("\nPIN cleared")); }
  else if(key=='#') { Serial.println(); if(pin==ACCESS_PIN) grant(F("correct keypad PIN")); else deny(F("incorrect keypad PIN")); pin=""; }
  else if(key=='A') lock(true,F("manual keypad lock"));
}
void updateAlarm() {
  if(millis()>=lockoutUntil) { if(lockoutUntil) { lockoutUntil=0; lock(true,F("lockout ended")); } return; }
  if(millis()-lastBlink>=250) { lastBlink=millis(); alarm=!alarm; digitalWrite(RED,alarm); digitalWrite(BUZZER,alarm); }
}
void setup() {
  pinMode(RED,OUTPUT); pinMode(GREEN,OUTPUT); pinMode(BUZZER,OUTPUT);
  Serial.begin(9600); latch.attach(SERVO_PIN); SPI.begin(); rfid.PCD_Init();
  lock(true,F("system boot"));
  Serial.println(F("AEGIS VAULT READY — scan card or enter PIN then #. Press A to lock."));
}
void loop() {
  updateAlarm();
  if(millis()<lockoutUntil) return;
  scanRFID(); scanKeypad();
  if(unlocked && millis()-unlockedAt>=UNLOCK_MS) lock(true,F("automatic re-lock"));
}
