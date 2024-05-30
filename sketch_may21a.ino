#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD to 0x27 for a 16 chars and 2 line display

// Morse code table
const int MAX_SIZE = 36;

const char* morseTable[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",  // Letters A-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."                                                                                                  // Numbers 0-9
};
// Decryption
String keys[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",  // Letters A-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."                                                                                                   // Numbers 0-9
};

String values[36] = {
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",  // Letters A-Z
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"                                                                                   // Numbers 0-9
};

String getValue(String key) {
  for (int i = 0; i < MAX_SIZE; ++i) {
    if (keys[i] == key) {
      return values[i];
    }
  }
  return "";  // Return an empty string if key not found
}

// Morse code parameters
const int dotDuration = 1200;  // Duration of a dot (milliseconds)
const int wpm = 5;             // Words per minute

// Pin definitions
const int ledPin = 13;     // Pin for the LED
const int speakerPin = 9;  // Pin for the piezo speaker
const int dotPin = 4;      // Pin for dot button
const int dashPin = 3;     // Pin for dash button

unsigned long startTime;
int started;
String myString;
int relay = 7 ;
String cmd = "___" ;
int flag = 0 ;

int lcdCursor = 0;  // Cursor position on the LCD

void setup() {
  Serial.begin(9600);       // Serial communication for both Serial Monitor and Bluetooth
  pinMode(ledPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(dotPin, INPUT_PULLUP);
  pinMode(dashPin, INPUT_PULLUP);
  pinMode(relay , OUTPUT) ;
  digitalWrite(relay , 1) ;

  // Display setup
  lcd.init();      // Initializing the display
  lcd.clear();     // Clearing the display
  lcd.backlight(); // Turn the backlight on
}

void loop() {
  // Handle button presses for dot and dash
  handleButtonPresses();

  // Handle Bluetooth/Serial input
  handleSerialInput();

  // Check for Morse code entry timeout
  checkMorseTimeout();
}

void handleButtonPresses() {
  if (digitalRead(dotPin) == LOW) {  // Check if dot button is pressed
    if (!started) {
      myString = "";
      started = 1;
      startTime = millis();
    }
    myString.concat('.');
    encodeMorse('.');
    delay(500);  // Debounce delay
  }

  if (digitalRead(dashPin) == LOW) {  // Check if dash button is pressed
    if (!started) {
      myString = "";
      started = 1;
      startTime = millis();
    }
    myString.concat('-');
    encodeMorse('-');
    delay(500);  // Debounce delay
  }
}

void handleSerialInput() {
  while (Serial.available() > 0) {  // Data received from Serial Monitor or Bluetooth
    char receivedChar = Serial.read();
    if (receivedChar == '.' || receivedChar == '-') {
      if (!started) {
        myString = "";
        started = 1;
        startTime = millis();
      }
      myString.concat(receivedChar);
      encodeMorse(receivedChar);
    } else if (receivedChar == '\n') {  // End of Morse code input
      String decodedValue = getValue(myString);
      Serial.println(decodedValue);  // Print decoded message to Serial Monitor
      displayCharacter(decodedValue);
      myString = "";  // Reset the string
      started = 0;
    }
  }
}

void checkMorseTimeout() {
  if (started) {
    double elapsedTime = (double)(millis() - startTime) / 1000.0;
    if (elapsedTime >= 4) {
      String decodedValue = getValue(myString);
      Serial.println(decodedValue);  // Print to Serial Monitor
      displayCharacter(decodedValue);
      myString = "";  // Reset the string
      started = 0;
    }
  }
}

void encodeMorse(char symbol) {
  digitalWrite(ledPin, HIGH);  // Turn on LED to indicate keying

  if (symbol == '.') {  // Dot
    tone(speakerPin, 500, dotDuration / wpm);  // Beep for dot
    delay(dotDuration / wpm);
  } else if (symbol == '-') {  // Dash
    tone(speakerPin, 500, dotDuration * 3 / wpm);  // Beep for dash
    delay(dotDuration * 3 / wpm);
  }

  digitalWrite(ledPin, LOW);  // Turn off LED
  noTone(speakerPin);
  delay(dotDuration / wpm);  // Pause between Morse code characters
}

void displayCharacter(String character) {
  if(character == "O"){
    cmd[0] = 'O' ;
  }
  else if(character == "N"){
    cmd[1] = 'N' ;
  }
  else if(character == "F"){
    cmd[1] = 'F' ;
  }
  else if(character == "0"){
    cmd[2] = '0' ;
  }
  else{
    cmd = "___" ;
  }

  Serial.println(cmd) ;

  if(cmd == "ON0"){
    Serial.println("ON") ;
    flag = 2 ;
    digitalWrite(relay , 0) ;
  }
  else if(cmd == "OF0"){
    Serial.println("OF") ;
    flag = 0 ;
    digitalWrite(relay , 1) ;
  }
  else if(flag == 2) {
    flag = 1 ;
    lcd.clear() ;
  }

  if(flag == 1){
      if (lcdCursor >= 32) {  // Both lines are full
        lcd.clear();
        lcdCursor = 0;  // Reset cursor to start writing from the beginning
      }

      int row = lcdCursor / 16;  // Calculate row
      int col = lcdCursor % 16;  // Calculate column

      lcd.setCursor(col, row);
      lcd.print(character);


      lcdCursor++;
  }
  else if(flag == 0){
    lcd.clear() ;
  }
}
