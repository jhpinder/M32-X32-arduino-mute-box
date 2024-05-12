#include <MIDI.h>
#define MUTE_CHANNEL 2
#define MUTE_ON 127
#define MUTE_OFF 0
#define MIN_MUTE_VAL 64

MIDI_CREATE_DEFAULT_INSTANCE();

int inputPins[8] = {38, 40, 42, 44, 46, 48, 50, 52};
int outputPins[8] = {39, 41, 43, 45, 47, 49, 51, 53};
int faderLookup[17] = {0, 1, 2, 3, 4, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7}; // indexes to the corresponding local mute numbers
int outputFaderNumberLookup[8] = {0, 1, 2, 3, 4, 5, 6, 16};

byte localLedState[8] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
byte currentSwitchState[8] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
byte lastSwitchState[8] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

void setup() {
  MIDI.setHandleControlChange(handleCC);
  MIDI.begin(MUTE_CHANNEL);
  MIDI.turnThruOff();

  for (int i = 0; i < 8; i++) {
    pinMode(inputPins[i], INPUT_PULLUP);
    pinMode(outputPins[i], OUTPUT);
    digitalWrite(outputPins[i], localLedState[i]);
  }

  pinMode(13, INPUT_PULLUP);
  if (digitalRead(13) == LOW) { // if the button is on, send mutes after init

    // turn on and off LEDs one by one on startup
    digitalWrite(outputPins[0], HIGH);
    for (int i = 1; i < 8; i++) {
      delay(1200);
      digitalWrite(outputPins[i - 1], LOW);
      delay(50);
      digitalWrite(outputPins[i], HIGH);
    }
    delay(1200);
    digitalWrite(outputPins[7], LOW);
    delay(50);

    // set all LEDs to on
    for (int i = 0; i < 8; i++) {
      localLedState[i] = HIGH;
    }
    updateLEDs();

    // send mute to all channels that the box controls to sync
    sendInitialMutes();
  } else {
    digitalWrite(outputPins[0], HIGH);
    for (int i = 1; i < 8; i++) {
      delay(60);
      digitalWrite(outputPins[i], HIGH);
      digitalWrite(outputPins[i - 1], LOW);
    }
    delay(60);
    digitalWrite(outputPins[7], LOW);
  }
}

void loop() {
  MIDI.read();
  checkSwitches();
  updateLEDs();
  delay(5);
}

void updateLEDs() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(outputPins[i], localLedState[i]);
  }
}

void checkSwitches() {
  for (int i = 0; i < 8; i++) {
    currentSwitchState[i] = digitalRead(inputPins[i]);
    if (currentSwitchState[i] != lastSwitchState[i]  && currentSwitchState[i] == LOW) {
      if (localLedState[i] == LOW) {
        MIDI.sendControlChange(outputFaderNumberLookup[i], MUTE_ON, MUTE_CHANNEL);
      } else {
        MIDI.sendControlChange(outputFaderNumberLookup[i], MUTE_OFF, MUTE_CHANNEL);
      }
    }
    lastSwitchState[i] = currentSwitchState[i];
  }
}

void handleCC(byte channel, byte faderNumber, byte muteState) {
  if (channel != MUTE_CHANNEL) {
    return;
  }
  if (faderNumber < 0 || faderNumber > 16 || faderLookup[faderNumber] < 0) {
    return;
  }
  if (muteState > 127 || muteState < 0) {
    return;
  }
  localLedState[faderLookup[faderNumber]] = muteState < MIN_MUTE_VAL ? LOW : HIGH;
}

void sendInitialMutes() {
  for (int i = 0; i < 8; i++) {
    MIDI.sendControlChange(outputFaderNumberLookup[i], MUTE_ON, MUTE_CHANNEL);
  }
}