#include <stdio.h>

// Define the MIDINote structure with variables
typedef struct {
    int pitch;
    int velocity;
    int channel;
} MIDINote;

// Print the details of a MIDINote
void print_note(MIDINote note) {
    printf("The MIDI note has pitch %d, velocity %d, and channel %d.\n", note.pitch, note.velocity, note.channel);
}

int main() {
    MIDINote note;

    // Uuser inputs pitch, velocity, and channel values
    printf("Enter pitch, velocity, and channel: ");
    scanf("%d %d %d", &note.pitch, &note.velocity, &note.channel);

    // Print the details of the entered note
    print_note(note);

    return 0;
}