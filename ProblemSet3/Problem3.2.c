#include <stdio.h>
#include <math.h>

// Convert MIDI note to frequency
double midi_to_frequency(int midi_note) {
    return 440.0 * pow(2.0, (midi_note - 69) / 12.0);
}

// Print the frequencies of the notes in the chord
void print_chord_frequencies(int chord[], int size) {
    for (int i = 0; i < size; i++) {
        printf("The frequency of MIDI note %d is %.2f Hz.\n", chord[i], midi_to_frequency(chord[i]));
    }
}

int main() {
    int size;

    // User inputs number of MIDI notes
    printf("Enter the number of MIDI notes in the chord: ");
    scanf("%d", &size);

    int chord[size]; // Define array with user input size

    // User inputs MIDI note numbers
    printf("Enter the MIDI notes separated by spaces: ");
    for (int i = 0; i < size; i++) {
        scanf("%d", &chord[i]);
    }

    // Print chord frequencies
    print_chord_frequencies(chord, size);

    return 0;
}
