#include <stdio.h>

int main() {
    // Variable to store the input MIDI pitch
    int pitch;

    // User inputs a MIDI pitch
    printf("Enter a MIDI pitch (0-127): ");
    scanf("%d", &pitch);

    // Check if the pitch is within the valid range
    if (pitch < 0 || pitch > 127) {
        printf("Invalid MIDI pitch. The pitch must be between 0 and 127.\n");
    } else {
        // Calculate the octave using the formula;
        // octave = pitch / 12 - 1
        int octave = pitch / 12 - 1;

        // Print
        printf("MIDI pitch %d belongs to octave %d.\n", pitch, octave);
    }

    return 0;
}
