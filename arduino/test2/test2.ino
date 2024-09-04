#include <SoftwareSerial.h>

// LEFT
// 2 STATE
// 3 TRANSMIT

int state = 0;
int step = 0;

//Array of int bits
int message[10] = {1, 1, 1, 0, 1, 0, 1, 1, 1, 1};

int index = 0;
void setup() {
  // Initialize software serial communication at 9600 baud
  state = 0;
  step = 0;
  index = 0;
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {

    Serial.println("Step next bit");
  if(step == 0){
    Serial.print("Sending next bit: ");
    Serial.print(message[index]);

    Serial.println();
    Serial.print("State: ");
    Serial.print(state);
    Serial.println();
    Serial.print("Step: ");
    Serial.print(step);
    Serial.println();
    Serial.print("Index: ");
    Serial.print(index);
    Serial.println();

  }
  digitalWrite(2, state);

  digitalWrite(3, message[index]);
  step = step + 1;

  if(step > 3)
  {
    if(state == 0)
    {
      state = 1;
    }
    else
    {
      state = 0;
    }
    step = 0;
    index = index + 1;
  }
  if(index >= 10)
  {
    index = 0;
  }


  delay(500); // Increased delay to ensure stability
}
