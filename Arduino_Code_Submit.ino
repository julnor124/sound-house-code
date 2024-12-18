// Pin assignments
const int buttonMusic = 13;   // Music button.
const int buttonBuzzer = 12;  // Buzzer button.
const int buttonDogBark = 11; // Dog barking button.

const int ledMusic = 3;        // LED controlled by music button.
const int ledBuzzer = 7;       // LED controlled by buzzer button.
const int ledForceSensor = 4;  // LED for force sensor.
const int ledDog = 5;          // LED controlled by dog barking button.

const int green6 = 6;   // Used for the working out and music scenarios.
const int green8 = 8;   // Used for the working out, music and barking scenarios.
const int green9 = 9;   // Used for the working out scenario.
const int green10 = 10; // Used for the default scenario.
const int greenA1 = A1; // Used for the default scenario.

const int buzzerPin = 2;       // Buzzer pin.
const int forceSensorPin = A0; // Force sensor pin.

// Variables to store button states and toggle status.
int stateMusicButton = 0;      // State of music button.
int stateBuzzerButton = 0;     // State of buzzer button.
int stateDogBarkButton = 0;    // State of dog barking button.

int lastStateMusicButton = HIGH; // Last state of music button.
int lastStateBuzzerButton = HIGH; // Last state of buzzer button.
int lastStateDogBarkButton = HIGH; // Last state of dog barking button.

bool isMusicOn = false;        // Track state for music.
bool isBuzzerOn = false;       // Track state for buzzer.
bool isDogBarkingOn = false;   // Track state for dog barking.

// Debounce variables.
const int debounceDelay = 50;  // debounce delay set to 50.
unsigned long lastDebounceTimeMusic = 0;  // debounce time for music button.
unsigned long lastDebounceTimeBuzzer = 0; // debounce time for buzzer button.
unsigned long lastDebounceTimeDogBark = 0; // debounce time for dog barking button.

void setup() {
    // Initialize button pins as input with internal pull-up resistor.
    pinMode(buttonMusic, INPUT_PULLUP);   
    pinMode(buttonBuzzer, INPUT_PULLUP);  
    pinMode(buttonDogBark, INPUT_PULLUP); 
    pinMode(forceSensorPin, INPUT_PULLUP); 

    // Initialize LED pins as output.
    pinMode(ledMusic, OUTPUT);        
    pinMode(ledBuzzer, OUTPUT);       
    pinMode(ledForceSensor, OUTPUT);  
    pinMode(ledDog, OUTPUT);          

    pinMode(green6, OUTPUT);
    pinMode(greenA1, OUTPUT);
    pinMode(green10, OUTPUT);
    pinMode(green8, OUTPUT);
    pinMode(green9, OUTPUT);

    // Initialize buzzer pin as output.
    pinMode(buzzerPin, OUTPUT);

    // Set initial states to LOW.
    digitalWrite(ledMusic, LOW);
    digitalWrite(ledBuzzer, LOW);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledForceSensor, LOW);
    digitalWrite(ledDog, LOW);

    // Default: Turn all green lights on.
    digitalWrite(green6, HIGH);
    digitalWrite(greenA1, HIGH);
    digitalWrite(green10, HIGH);
    digitalWrite(green8, HIGH);
    digitalWrite(green9, HIGH);

    Serial.begin(9600); // Start serial communication for debugging.
}

void loop() {
    // Read the state of buttons with debouncing.
    int readingMusic = digitalRead(buttonMusic);
    int readingBuzzer = digitalRead(buttonBuzzer);
    int readingDogBark = digitalRead(buttonDogBark);
    int forceSensorValue = analogRead(forceSensorPin); 

    // Flags to check if any scenario is active.
    bool scenarioActive = false;

    // Music button debounce and control.
    if (readingMusic != lastStateMusicButton) {
        lastDebounceTimeMusic = millis(); // Record the time when a state change is detected.
    }
    if ((millis() - lastDebounceTimeMusic) > debounceDelay) {  // Calculates the time difference since the last detected state change and checks if valid.
        if (readingMusic != stateMusicButton) {
            stateMusicButton = readingMusic; // Updating stateMusicButton to the new state.
            if (stateMusicButton == LOW) { // Button press detected.
                if (!isBuzzerOn && !isDogBarkingOn) { // Activate music only if others are off.
                    isMusicOn = !isMusicOn;  // Toggle music state, turn it on or off.
                    if (isMusicOn) {
                        scenarioActive = true;
                        Serial.println("BUTTON_PRESSED"); // Connected to pyhton script.
                    } else {
                        Serial.println("StopMusic"); // Connected to pyhton script.
                    }
                }
            }
        }
    }
    lastStateMusicButton = readingMusic; // Updates lastStateMusicButton to the current reading, so the code can detect future state changes.

    if (isMusicOn) {
        // Turn off default LEDs and activate only music scenario LEDs.
        scenarioActive = true;
        deactivateAllGreenLeds(); 
        digitalWrite(ledForceSensor, LOW); 
        digitalWrite(ledMusic, HIGH);  
        digitalWrite(green6, HIGH);    
        digitalWrite(green8, HIGH);    
    } else {
        digitalWrite(ledMusic, LOW);   
    }

    // Dog barking button debounce and control.
    if (readingDogBark != lastStateDogBarkButton) {
        lastDebounceTimeDogBark = millis(); // Record the time when a state change is detected.
    }
    if ((millis() - lastDebounceTimeDogBark) > debounceDelay) { // Calculates the time difference since the last detected state change and checks if valid.
        if (readingDogBark != stateDogBarkButton) {
            stateDogBarkButton = readingDogBark;  // Updating stateDogBarkButton to the new state.
            if (stateDogBarkButton == LOW) { // Button press detected.
                if (!isBuzzerOn && !isMusicOn) { // Activate barking only if others are off.
                    isDogBarkingOn = !isDogBarkingOn;  // Toggle dog barking state, turn it on or off.
                    if (isDogBarkingOn) {
                        scenarioActive = true;
                        Serial.println("DOG_BARK_BUTTON_PRESSED"); // Connected to pyhton script.
                    } else {
                        Serial.println("StopBarking"); // Connected to pyhton script.
                    }
                }
            }
        }
    }
    lastStateDogBarkButton = readingDogBark; // Updates lastStateDogBarkButton to the current reading, so the code can detect future state changes.

    if (isDogBarkingOn) {
        scenarioActive = true;
        deactivateAllGreenLeds();
        digitalWrite(ledForceSensor, LOW);
        digitalWrite(ledDog, HIGH);   
        digitalWrite(green8, HIGH);   
    } else {
        digitalWrite(ledDog, LOW);     
    }

    // Buzzer button debounce and control.
    if (readingBuzzer != lastStateBuzzerButton) {
        lastDebounceTimeBuzzer = millis(); // Record the time when a state change is detected.
    }
    if ((millis() - lastDebounceTimeBuzzer) > debounceDelay) {
        if (readingBuzzer != stateBuzzerButton) {
            stateBuzzerButton = readingBuzzer; // Updating stateBuzzerButton to the new state.
            if (stateBuzzerButton == LOW) { // Button press detected.
                if (!isMusicOn && !isDogBarkingOn) { // Activate buzzer only if others are off.
                    isBuzzerOn = !isBuzzerOn;  // Toggle buzzer state.
                }
            }
        }
    }
    lastStateBuzzerButton = readingBuzzer; // Updates lastStateBuzzerButton to the current reading, so the code can detect future state changes.

    // Buzzer state control
    if (isBuzzerOn) {
        scenarioActive = true;
        deactivateAllGreenLeds();
        digitalWrite(ledForceSensor, LOW); 
        digitalWrite(ledBuzzer, HIGH);    
        digitalWrite(buzzerPin, HIGH);   
    } else {
        digitalWrite(ledBuzzer, LOW);     
        digitalWrite(buzzerPin, LOW);     
    }

    // Force sensor control logic (only active if music, barking and buzzer are off).
    if (forceSensorValue <= 500 && !isMusicOn && !isDogBarkingOn && !isBuzzerOn) {
        scenarioActive = true;
        deactivateAllGreenLeds();
        analogWrite(ledForceSensor, 255);
        analogWrite(green6, 255);        
        analogWrite(green8, 255);        
        analogWrite(green9, 255);         
    } else {
        analogWrite(ledForceSensor, 0);   // Turn off force sensor LED when threshold is not met.
    }

    // Default state (when no scenario is active).
    if (!scenarioActive) {
        turnOnDefaultGreenLeds();
    }
}

// Function to deactivate all green LEDs.
void deactivateAllGreenLeds() {
    digitalWrite(green6, LOW);
    digitalWrite(green8, LOW);
    digitalWrite(green9, LOW);
    digitalWrite(greenA1, LOW);
    digitalWrite(green10, LOW);
}

// Function to turn on the default green LEDs.
void turnOnDefaultGreenLeds() {
    digitalWrite(green6, HIGH);
    digitalWrite(green8, HIGH);
    digitalWrite(green9, HIGH);
    digitalWrite(greenA1, HIGH);
    digitalWrite(green10, HIGH);
}
