#include <utils.h>

void setupInputs(int pins[], int numPins) {
    for (int i=0; i < numPins; i++) {
        pinMode(pins[i], INPUT);
    }
}