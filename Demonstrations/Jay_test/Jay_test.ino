
int readVal = 0;

void setup() {
 pinMode(11, OUTPUT);

 Serial.begin(9600);
}

void loop() {
readVal = analogRead(A0);
analogWrite(11, ((readVal * 255) / 1023));
Serial.print(readVal);
delay(1200);

}
