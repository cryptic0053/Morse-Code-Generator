const char* morseTable[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", // Letters A-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", // Numbers 0-9
};


const int dotDuration = 1200; // Duration of a dot (milliseconds)
const int wpm = 5; // Words per minute

// Pin definitions
const int ledPin = 13;   // Pin for the LED
const int speakerPin = 9; // Pin for the piezo speaker
const int dotPin = 4;  // Pin for dot button
const int dashPin = 3; // Pin for dash button

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(dotPin, INPUT_PULLUP);
  pinMode(dashPin, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(dotPin) == LOW) { // Check if dot button is pressed
    encodeMorse('.');
  }

  if (digitalRead(dashPin) == LOW) { // Check if dash button is pressed
    encodeMorse('-');
  }
  
  if (Serial.available() > 0) { // Text entered in Serial Monitor, decode Morse code
    decodeMorse();
  }
}

void encodeMorse(char symbol) {
  digitalWrite(ledPin, HIGH); // Turn on LED to indicate keying
  
  if (symbol == '.') { // Dot
    Serial.print(".");
    tone(speakerPin, 500, dotDuration / wpm); // Beep for dot
    delay(dotDuration / wpm);
  } else if (symbol == '-') { // Dash
    Serial.print("-");
    tone(speakerPin, 500, dotDuration * 3 / wpm); // Beep for dash
    delay(dotDuration * 3 / wpm);
  }
  
  digitalWrite(ledPin, LOW); // Turn off LED
  noTone(speakerPin);
  delay(dotDuration / wpm); // Pause between Morse code characters
}

void decodeMorse() {
  String inputText = Serial.readStringUntil('\n'); // Read text from Serial Monitor until newline
  Serial.println("Decoded Message: " + inputText); // Print the decoded message to Serial Monitor
  
  for (int i = 0; i < inputText.length(); i++) {
    char c = inputText.charAt(i);
    if (c == ' ') { // Space between words
      delay(dotDuration * 7 / wpm); // Pause between words
    } else if (c >= 'A' && c <= 'Z') { // Letter
      int index = c - 'A';
      printMorse(morseTable[index]);
    } else if (c >= '0' && c <= '9') { // Number
      int index = c - '0' + 26;
      printMorse(morseTable[index]);
    }
  }
}

void printMorse(const char* morseCode) {
  for (int i = 0; morseCode[i] != '\0'; i++) {
    if (morseCode[i] == '.') {
      digitalWrite(ledPin, HIGH);
      tone(speakerPin, 500, dotDuration / wpm); // Beep for dot
      delay(dotDuration / wpm);
    } else if (morseCode[i] == '-') {
      digitalWrite(ledPin, HIGH);
      tone(speakerPin, 500, dotDuration * 3 / wpm); // Beep for dash
      delay(dotDuration * 3 / wpm);
    }
    digitalWrite(ledPin, LOW);
    noTone(speakerPin);
    delay(dotDuration / wpm); // Pause between Morse code characters
  }
  delay(dotDuration * 3 / wpm); // Pause between Morse code words
}
