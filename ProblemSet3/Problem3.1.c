#include <stdio.h>
#include <math.h>

// Convert MIDI note to frequency
// Takes an integer MIDI note number and returns the corresponding frequency as a double
double midi_to_frequency(int note) {
    return 440.0 * pow(2.0, (note - 69) / 12.0);
}

int main() {
    int note;

    // User inputs a MIDI note
    printf("Enter a MIDI note: ");
    scanf("%d", &note);

    // Calculate frequency 
    double frequency = midi_to_frequency(note);

    // Print the frequency
    printf("The frequency of MIDI note %d is %.2f Hz.\n", note, frequency);

    return 0;
}