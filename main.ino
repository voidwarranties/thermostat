// constants
const int clients = 2;                         // number of clients
const int led[clients] = { 8, 9 };             // pins on which leds are mounted, ordered by client
const int button[clients] = { 3, 4 };          // pins on which buttons are mounted, ordered by client
const int relay_pin = 12;                      // pins on which buttons are mounted, ordered by client
const int blink_interval = 800;                // how fast to blink (ms)
const int debounce_delay = 5;                  // the debounce time; increase if the output flickers (ms)
const long request_duration = 2 * 3600 * 1000; // for how long to turn on the heating when requested (ms)

//  variables
boolean blink_state;                        // current blink state
unsigned long blink_previousToggle;         // last time blink state was changed

boolean debounce_lastReading[clients];      // previous reading from the input pin
unsigned long debounce_lastToggle[clients]; // time since the output pin was last toggled

boolean button_state[clients];              // current state of the buttons
boolean button_lastState[clients];          // previous state of the buttons (after debounce)

unsigned long request[clients];             // requests for heating (time of request if active, else 0)
boolean relay = false;                      // relay state

void setup() {
  for (int i = 0; i < clients; i++) {
    pinMode(led[i], OUTPUT);
    pinMode(button[i], INPUT);
  }
  pinMode(relay_pin, OUTPUT);
}

void loop() {
  // get current time and blink state
  unsigned long currentMillis = millis();
  blink_state = update_blinkState(currentMillis);

  // for each button: 
  // * check button state
  // * if state changed debounce and set request time
  // * check if a request expired
  // * update led.  
  for (int i = 0; i < clients; i++) { 
    button_state[i] = digitalRead(button[i]);
    boolean changed = debounce(i, currentMillis);
 
    if (changed == true) {
        if (button_state[i] == HIGH) {
          if (request[i] == 0) {
            request[i] = currentMillis;
          } else {
            request[i] = 0;
          }
          update_relay();
        }
    }
    
    if (request[i] != 0) {
      if ((unsigned long)(currentMillis - request[i]) >= request_duration){
        request[i] = 0;
        update_relay();
      }
    }
    
    update_led(i);
  }
}

int update_blinkState(long currentMillis) {
  if((unsigned long)(currentMillis - blink_previousToggle) >= blink_interval) {   
    blink_state = !blink_state;
    blink_previousToggle = currentMillis;
  }
  return blink_state;
}

boolean debounce(int i, long currentMillis) {
  if (button_state[i] != debounce_lastReading[i]) {
    debounce_lastToggle[i] = currentMillis;
  }
  debounce_lastReading[i] = button_state[i];
  if ((unsigned long)(currentMillis - debounce_lastToggle[i]) >= debounce_delay) {
    boolean lastState = button_lastState[i];
    button_lastState[i] = button_state[i];
    if (button_state[i] != lastState) {
      return true;
    }
  }
  return false;
}

boolean update_relay() {
  relay = false;
  for (int i = 0; i < clients; i++) { 
    if (request[i] != 0) {
      relay = true;
    }
  }
  digitalWrite(relay_pin, relay);
}

int update_led(int i) {
  if (request[i] != 0) {
    digitalWrite(led[i], HIGH);
  } else if (relay == true){
    digitalWrite(led[i], blink_state);
  } else {
    digitalWrite(led[i], LOW);
  }
}
