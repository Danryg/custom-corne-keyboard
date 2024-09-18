#include "Keyboard.h"
#include <SoftwareSerial.h>

#define ROWS 3
#define COLS 6
#define FUNC_KEYS 3

const int rowPins[3] = { 2, 9, 15 };          // Rows connected to pins 16 and 5
const int colPins[6] = { 3, 4, 5, 6, 7, 8 };  // Columns connected to pins 8 and 9
const int funcPins[3] = { 14, 16, 10 };

char keys_default_layer[3][6] = {
  { 't', 'r', 'e', 'w', 'q', KEY_TAB },
  { 'g', 'f', 'd', 's', 'a', KEY_LEFT_CTRL },
  { 'b', 'v', 'c', 'x', 'z', KEY_LEFT_SHIFT }
};
char keys_spec_layer[3][6] = {
  { '5', '4', '3', '2', '1', KEY_ESC },
  { '0', '9', '8', '7', '6', KEY_LEFT_GUI },
  { KEY_F5, KEY_F5, '$', '@', '<', KEY_LEFT_SHIFT }
};

char keys_func_layer[FUNC_KEYS] = {
  KEY_LEFT_ALT, 1, ' '
};

// DEBOUNCE
const int debounceDelay = 10;                   // Debounce delay of 50 milliseconds
unsigned long lastDebounceTime[ROWS][COLS];     // Stores the last debounce time for each button
unsigned long lastDebounceTimeFunc[FUNC_KEYS];  // Stores the last debounce time for each button

// BUTTON STATE
int buttonState[ROWS][COLS];
int buttonStateFunc[FUNC_KEYS];
int currentLayer = 0;

// ENGINE
void setup() {
  //keyboard setup
  Keyboard.begin(KeyboardLayout_sv_SE);

  //initialize debounce and button state
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      lastDebounceTime[i][j] = 0;
      buttonState[i][j] = HIGH;
    }
  }
  currentLayer = 0;

  //initialize pins
  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);  // Disable all rows
  }
  for (int i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT_PULLUP);  // Enable pull-up resistors for columns
  }
  for (int i = 0; i < FUNC_KEYS; i++) {
    pinMode(funcPins[i], INPUT_PULLUP);  // Enable pull-up resistors for columns
  }

  //default delay
  delay(1000);
}


// the loop function runs over and over again forever
void loop() {
  unsigned long currentTime = millis();

  // Handle func keys
  for (int i = 0; i < FUNC_KEYS; i++) {
    bool currentButtonState = digitalRead(funcPins[i]) == LOW;

    if ((currentTime - lastDebounceTimeFunc[i]) > debounceDelay) {
      // Debounced button state change
      if (currentButtonState != buttonStateFunc[i]) {
        buttonStateFunc[i] = currentButtonState;
        if (buttonStateFunc[i] == LOW) {
          // Button is pressed
          if (i == 1) {
            currentLayer = 0;
            continue;
          }

          Keyboard.release(keys_func_layer[i]);
        } else {
          if (i == 1) {
            currentLayer = 1;
            continue;
          }
          Keyboard.press(keys_func_layer[i]);
        }
        lastDebounceTimeFunc[i] = currentTime;
      }
    }
  }

  for (int row = 0; row < ROWS; row++) {
    digitalWrite(rowPins[row], LOW);  // Activate row
    for (int col = 0; col < COLS; col++) {
      bool currentButtonState = digitalRead(colPins[col]) == LOW;

      if ((currentTime - lastDebounceTime[row][col]) > debounceDelay) {
        // Debounced button state change
        if (currentButtonState != buttonState[row][col]) {
          buttonState[row][col] = currentButtonState;

          if (buttonState[row][col] == LOW) {
            // Button is pressed
            if (currentLayer == 1) {
              Keyboard.release(keys_spec_layer[row][col]);

            } else {
              Keyboard.release(keys_default_layer[row][col]);
            }
          }
          if (buttonState[row][col] == HIGH) {
            // Button is pressed
            if (currentLayer == 1) {
              Keyboard.press(keys_spec_layer[row][col]);

            } else {
              Keyboard.press(keys_default_layer[row][col]);
            }
          }
          lastDebounceTime[row][col] = currentTime;
        }
      }
    }
    digitalWrite(rowPins[row], HIGH);  // Deactivate row
  }
}
