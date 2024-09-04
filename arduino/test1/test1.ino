#include <SoftwareSerial.h>

// RIGHT

// 2 RECEIVE
// 3 STATE

int lastState = 0;

int code[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int codeIndex = 0;

void setup() {
  // Initialize software serial communication at 9600 baud
  lastState = 0;
  codeIndex = 0;

  pinMode(2, INPUT);
  pinMode(3, INPUT);
}


void loop() {

  int state = digitalRead(3);
  if(state != lastState){
    int biten = digitalRead(2);
    if(biten == HIGH){
      code[codeIndex] = 1;
    } else {
      code[codeIndex] = 0;
    }
    codeIndex++;
  }

  if(codeIndex == 10){
    for(int i = 0; i < 10; i++){
      Serial.print(code[i]);
    }
    Serial.println();
    codeIndex = 0;
  }

  delay(500);  // Wait for 1 second between sending
}
