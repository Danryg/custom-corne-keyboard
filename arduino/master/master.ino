#include "Keyboard.h"
#include <SoftwareSerial.h>

#define TX_PIN 8  // Transmit pin connected to RX of the receiver
#define RX_PIN 9  // Receive pin connected to TX of the receiver

const int rowPins[2] = { 15, 5 };   // Rows connected to pins 16 and 5
const int colPins[2] = { A0, 10 };  // Columns connected to pins 8 and 9



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

SoftwareSerial mySerial(TX_PIN, RX_PIN);
// the setup function runs once when you press reset or power the board
void setup() {
  Keyboard.begin();
  mySerial.begin(57600);


  for (int i = 0; i < 2; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);     // Disable all rows
    pinMode(colPins[i], INPUT_PULLUP);  // Enable pull-up resistors for columns
  }
  delay(1000);
}

ArrayList pressed;
ArrayList slave_pressed;


int incomingAdd = -1;
// the loop function runs over and over again forever
void loop() {
  ArrayList temp;
  if (mySerial.available() > 0) {
    //Read the incoming byte
    char incomingByte = mySerial.read();
    if (incomingAdd == -1) {
      if (incomingByte == 'p') {
        incomingAdd = 1;
      } else if (incomingByte == 'r') {
        incomingAdd = 0;
      }
    } else {
      if (incomingAdd == 1) {
        slave_pressed.add(incomingByte);
      } else if (incomingAdd == 0) {
        slave_pressed.remove(incomingByte);
      }
      incomingAdd = -1;
    }
  }




  for (int row = 0; row < 2; row++) {
    digitalWrite(rowPins[row], LOW);  // Activate row
    for (int col = 0; col < 2; col++) {
      if (digitalRead(colPins[col]) == LOW) {
        // Button at (row, col) is pressed
        // Add your code here to handle the button press
        if (row == 0 && col == 0) {
          temp.add('e');
        } else if (row == 1 && col == 0) {
          temp.add('a');
        } else if (row == 0 && col == 1) {
          temp.add('b');
        } else if (row == 1 && col == 1) {
          temp.add('c');
        }
      }
    }
    digitalWrite(rowPins[row], HIGH);  // Deactivate row
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
