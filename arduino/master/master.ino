#include "Keyboard.h"
#include <SoftwareSerial.h>


const int rowPins[3] = {2, 9, 15 };   // Rows connected to pins 16 and 5
const int colPins[6] = { 3, 4, 5, 6, 7, 8};  // Columns connected to pins 8 and 9
SoftwareSerial mySerial(A3, A2);

char array[3][6] = {
    {'t', 'r', 'e', 'w', 'q', KEY_TAB},
    {'g', 'f', 'd', 's', 'a', KEY_ESC},
    {'b', 'v', 'c', 'x', 'z', KEY_LEFT_SHIFT}
};
char layer_2[3][6] = {
    {'5', '4', '3', '2', '1', KEY_TAB},
    {'0', '9', '8', '7', '6', KEY_LEFT_GUI},
    {'b', 'v', 'c', 'x', 'z', KEY_LEFT_CTRL}
};

class ArrayList {
private:
  char* items;
  int capacity;
  int count;

  void expandCapacity() {
    char* newItems = new char[capacity * 2];
    for (int i = 0; i < count; ++i) {
      newItems[i] = items[i];
    }
    delete[] items;
    items = newItems;
    capacity *= 2;
  }

public:
  ArrayList(int initialCapacity = 10)
    : capacity(initialCapacity), count(0) {
    items = new char[capacity];
  }
  ~ArrayList() {
    delete[] items;
  }

  void add(char item) {
    if (count == capacity) {
      expandCapacity();
    }
    items[count++] = item;
  }

  bool contains(char item) {
    for (int i = 0; i < count; ++i) {
      if (items[i] == item) {
        return true;
      }
    }
    return false;
  }

  int size() {
    return count;
  }

  char get(int index) {
    if (index < 0 || index >= count) {
      // Simple error handling: return the first item.
      return items[0];
    }
    return items[index];
  }
  void remove(char item) {
    for (int i = 0; i < count; ++i) {
      if (items[i] == item) {
        // Shift all elements to the left to fill the gap.
        for (int j = i; j < count - 1; ++j) {
          items[j] = items[j + 1];
        }
        count--;  // Decrease the element count.
        return;   // Exit after removing the item.
      }
    }
  }

  // This method compares the current list with another list and returns a new list
  // with items that are in the current list but not in the other list.
  ArrayList compareAndReturnNew(ArrayList& other) {
    ArrayList resultList;
    for (int i = 0; i < this->count; ++i) {
      if (!other.contains(this->items[i])) {
        resultList.add(this->items[i]);
      }
    }
    return resultList;
  }
};

// the setup function runs once when you press reset or power the board
void setup() {
  Keyboard.begin();

  Serial.begin(9600);
  mySerial.begin(9600);

  for (int i = 0; i < 3; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);     // Disable all rows
  }
  for (int i = 0; i < 6; i++) {
    pinMode(colPins[i], INPUT_PULLUP);  // Enable pull-up resistors for columns
  }

    pinMode(10, INPUT_PULLUP);  // Enable pull-up resistors for columns
    pinMode(14, INPUT_PULLUP);  // Enable pull-up resistors for columns
    pinMode(16, INPUT_PULLUP);  // Enable pull-up resistors for columns
  delay(1000);
}

ArrayList pressed;
ArrayList slave_pressed;


int incomingAdd = -1;
// the loop function runs over and over again forever
void loop() {
  ArrayList temp;
  for (int row = 0; row < 3; row++) {
    digitalWrite(rowPins[row], LOW);  // Activate row
    for (int col = 0; col < 6; col++) {
      if (digitalRead(colPins[col]) == LOW) {
        // Button at (row, col) is pressed
        // Add your code here to handle the button press

        if(digitalRead(16) == LOW){
          temp.add(layer_2[row][col]);
        } else {
          temp.add(array[row][col]);
        }
      }
    }
    digitalWrite(rowPins[row], HIGH);  // Deactivate row
  }

  if(digitalRead(10) == LOW){

    temp.add(' ');
  }

  if(digitalRead(14) == LOW){

    temp.add(KEY_LEFT_ALT);
  }

  for (int i = 0; i < slave_pressed.size(); i++) {
    temp.add(slave_pressed.get(i));
  }

  ArrayList compres = temp.compareAndReturnNew(pressed);
  for (int i = 0; i < compres.size(); i++) {
    Keyboard.press(compres.get(i));
    pressed.add(compres.get(i));

  }

  ArrayList compres2 = pressed.compareAndReturnNew(temp);

  for (int i = 0; i < compres2.size(); i++) {
    Keyboard.release(compres2.get(i));
    pressed.remove(compres2.get(i));
  }


  delay(10);
}
