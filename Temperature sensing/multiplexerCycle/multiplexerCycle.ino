int chipCounter = 0;

void setup() {
  
pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);

Serial.begin(9600);

}

void loop() {

// Physically select hardware pin.
bool * chipSelect = getChipSelectArray(chipCounter);

digitalWrite(2, chipSelect[0]);
digitalWrite(3, chipSelect[1]);
digitalWrite(4, chipSelect[2]);

chipCounter++;

chipCounter = chipCounter % 8;

Serial.println(chipCounter);

delay(100);

}



bool * getChipSelectArray(int chipCount){

 int maxSize = 3;

 static bool chipSelectArray[3] = {0};

 int remainder = 0;

 int intQuotient = chipCount;

  for(int i = 0; i < maxSize; i++){

    remainder = intQuotient - ((intQuotient / 2) * 2);

    chipSelectArray[i] = remainder;

    intQuotient = intQuotient / 2;
    
  }

 return chipSelectArray;

}
