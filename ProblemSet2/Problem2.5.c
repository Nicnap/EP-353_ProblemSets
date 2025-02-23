#include <stdio.h>

int main() {
    // Variable to store the input MIDI pitch
    int pitch;

    // User inputs MIDI pitches continuously
    printf("Enter MIDI pitches (0-127), or -1 to exit:\n");

    // Continuously accept MIDI pitch values until the user enters -1
    while (1) {
        // User to inputs a MIDI pitch
        printf("Enter a MIDI pitch: ");
        scanf("%d", &pitch);

        // Check for sentinel value (-1 to exit)
        if (pitch == -1) {
            printf("Exiting program.\n");
            break;
        }

        // Check if the pitch is within the valid range
        if (pitch < 0 || pitch > 127) {
            printf("Invalid MIDI pitch. The pitch must be between 0 and 127.\n");
        } else {
            // Define an array of note names corresponding to pitch % 12
            const char *noteNames[] = {
                "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
            };

            // Calculate the octave using the formula;
            // octave = pitch / 12 - 1
            int octave = pitch / 12 - 1;

            // Calculate the note using the formula;
            // pitch % 12
            int note = pitch % 12;

            // Array calls note name
            const char *noteName = noteNames[note];

            // Print 
            printf("The MIDI pitch %d is %s%d.\n", pitch, noteName, octave);
        }
    }

    return 0;
}
