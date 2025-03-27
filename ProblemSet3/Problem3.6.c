#include <stdio.h>

// Define the MIDINote structure
typedef struct {
    int pitch;
    int velocity;
    int channel;
} MIDINote;

// Print the details of a single MIDINote
void print_note(MIDINote note) {
    printf("The MIDI note has pitch %d, velocity %d, and channel %d.\n", note.pitch, note.velocity, note.channel);
}

// Function to transpose the notes
void transpose_notes(MIDINote* notes, int size, int steps) {
    for (int i = 0; i < size; i++) {
        notes[i].pitch += steps;
    }
}

int main() {
    int size, steps;

    // User inputs number of MIDI notes
    printf("Enter the number of MIDI notes: ");
    scanf("%d", &size);

    // Create an array of MIDINote structures based on the input size
    MIDINote notes[size];

    // User inputs each MIDI note's details
    printf("Enter the MIDI notes (pitch velocity channel) each on a new line:\n");
    for (int i = 0; i < size; i++) {
        scanf("%d %d %d", &notes[i].pitch, &notes[i].velocity, &notes[i].channel);
    }

    // User inputs number of steps to transpose the notes
    printf("Enter the number of steps to transpose (0 ~ 11): ");
    scanf("%d", &steps);

    // Transpose the notes by the specified number of steps
    transpose_notes(notes, size, steps);

    // Print the details of the transposed MIDI notes
    for (int i = 0; i < size; i++) {
        print_note(notes[i]);
    }

    return 0;
}