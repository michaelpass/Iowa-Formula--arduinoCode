#include <LiquidCrystal.h>

// User-entered byte corresponding to desired sensor.
// Will be X by default, and then set by user to A, B, or C.
char chipSelect = 'X';
int chipCount = 0;

// LCD constant values. Change these to chagne pins on Arduino.
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Array of sensor values tied to multiplexer 1.
int sensorValue = 0;
double humanreadableValue = 0.0;

// Memory storage of received bytes
byte receivedMessage = 0;


void setup() {

  // Initialize UART for receiving commands from computer
  Serial.begin(9600);

  // Initialize LED and write welcome message.
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Chip Select: X");
  lcd.setCursor(0, 1);
  lcd.print("Sensor:      V");
  lcd.setCursor(0, 0);


  // Initialize chip-select pins and set them to low.
  // (Note: All chip-selects low corresponds to 000
  //  or device number 0, meaning device 0 will be
  //  the device that is connected to the common output.)

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);

}



void loop() {


  // Step 1. See if user has input in a control byte over serial. Make changes accordingly.

  if (Serial.available() > 0) {

    receivedMessage = Serial.read();

    // If user has input a different command, check to see if different and valid.

    if (chipSelect != receivedMessage) {

      if (receivedMessage == 'A' || receivedMessage == 'B' || receivedMessage == 'C' || receivedMessage == 'a' || receivedMessage == 'b' || receivedMessage == 'c')

        // If both different and valid, update chip-select.

        chipSelect = receivedMessage;


      // Translate user-command to chip number.

      switch (chipSelect) {

        case 'A':

          chipCount = 0;
          break;

        case 'a':

          chipCount = 0;
          break;

        case 'B':
          chipCount = 1;
          break;

        case 'b':
          chipCount = 1;
          break;

        case 'C':
          chipCount = 2;
          break;

        case 'c':
          chipCount = 2;
          break;

      }

      // Translate chip number into chip-select voltages in hardware.
      bool * chipSelectArray = getChipSelectArray(chipCount);

      // Write these values to select pin.
      digitalWrite(8, chipSelectArray[0]);
      digitalWrite(9, chipSelectArray[1]);
      digitalWrite(10, chipSelectArray[2]);

      // Consider adding hardware delay of 5 ms or so to allow equalization of output pin.

      lcd.setCursor(13, 0);
      lcd.print(chipSelect);
      lcd.setCursor(0, 0);

    }

  }


  // Step 2. Read multiplexed sensor value. Calculate in volts.

  sensorValue = analogRead(0); // Note: analogRead samples values between 0V and 5V as an int between 0 and 1023.

  humanreadableValue = sensorValue * 5.0 / 1023.0;

  // Step 3. Output reading onto lcd.

  int digit1 = 0, digit2 = 0, digit3 = 0, digit4 = 0;

  digit1 = ((int)humanreadableValue) % 10;
  digit2 = ((int)(humanreadableValue * 10.0)) % 10;
  digit3 = ((int)(humanreadableValue * 100.0)) % 10;
  digit4 = ((int)(humanreadableValue * 1000.0)) % 10;


  lcd.setCursor(8, 1);
  lcd.print(digit1);
  lcd.print('.');
  lcd.print(digit2);
  lcd.print(digit3);
  lcd.print(digit4);
  lcd.setCursor(0, 0);

  delay(500);

}


// --------------- Helper functions -----------------------

bool * getChipSelectArray(int chipCount) {

  int maxSize = 3;

  static bool chipSelectArray[3] = {0};

  int remainder = 0;

  int intQuotient = chipCount;

  for (int i = 0; i < maxSize; i++) {

    remainder = intQuotient - ((intQuotient / 2) * 2);

    chipSelectArray[i] = remainder;

    intQuotient = intQuotient / 2;

  }

  return chipSelectArray;

}
