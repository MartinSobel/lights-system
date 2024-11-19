#include "MIDIUSB.h"

const byte pinArray[] = {2, 3, 4, 5, 6, 7, 8, 9, 21, 20, 19, 18, 15, 14, 16, 10};
const byte numPins = sizeof(pinArray) / sizeof(pinArray[0]);
const byte startNote = 24;

void setup() {
  Serial.begin(115200);
  for (byte i = 0; i < numPins; i++) {
    pinMode(pinArray[i], OUTPUT);
    digitalWrite(pinArray[i], LOW); // Inicializar en LOW
  }
}

void handleNoteEvent(byte pitch, bool isNoteOn) {
  if (pitch >= startNote && pitch < startNote + numPins) {
    byte pinIndex = pitch - startNote;
    digitalWrite(pinArray[pinIndex], isNoteOn ? HIGH : LOW);
  }
}

void loop() {
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      if (rx.byte2 >= startNote && rx.byte2 < startNote + numPins) {
        if (rx.header == 8) {        // Note Off
          handleNoteEvent(rx.byte2, false);
        } else if (rx.header == 9) { // Note On
          handleNoteEvent(rx.byte2, true);
        }
      }
    }
  } while (rx.header != 0);
}
