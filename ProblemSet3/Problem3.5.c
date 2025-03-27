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

// Print the details of an array of MIDINote structures
void print_notes(MIDINote notes[], int size) {
    for (int i = 0; i < size; i++) {
        print_note(notes[i]);
    }
}

int main() {
    int size;

    // User inputs number of MIDI notes
    printf("Enter the number of MIDI notes: ");
    scanf("%d", &size);

    MIDINote notes[size];

    // User inputs details for each MIDI note
    printf("Enter the MIDI notes (pitch velocity channel) each on a new line:\n");
    for (int i = 0; i < size; i++) {
        scanf("%d %d %d", &notes[i].pitch, &notes[i].velocity, &notes[i].channel);
    }

    // Print the details of all entered MIDI notes
    print_notes(notes, size);

    return 0;
}