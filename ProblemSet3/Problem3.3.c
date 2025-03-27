#include <stdio.h>
#include <math.h>

// Convert a MIDI note to its frequency
float midi_to_frequency(int note) {
    return 440.0 * pow(2.0, (note - 69) / 12.0);
}

// Calculate the frequencies for each note in the chord
void frequencies_of_chord(int* chord, int size, float* frequencies) {
    for (int i = 0; i < size; i++) {
        frequencies[i] = midi_to_frequency(chord[i]);
    }
}

int main() {
    int size;
    
    // User inputs number of MIDI notes in the chord
    printf("Enter the number of MIDI notes in the chord: ");
    scanf("%d", &size);
    
    // Create arrays to store the MIDI notes and the corresponding frequencies
    int chord[size];
    float frequencies[size];
    
    // User inputs the MIDI notes separated by spaces
    printf("Enter the MIDI notes separated by spaces: ");
    for (int i = 0; i < size; i++) {
        scanf("%d", &chord[i]);
    }
    
    // Calculate the frequencies for the chord
    frequencies_of_chord(chord, size, frequencies);
    
    // Print the frequencies
    for (int i = 0; i < size; i++) {
        printf("The frequency of MIDI note %d is %.2f Hz.\n", chord[i], frequencies[i]);
    }
    
    return 0;
}