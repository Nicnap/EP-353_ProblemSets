#include <stdio.h>
#include <math.h>

int main() {
    // Initialize MIDI pitch values 
    int C4 = 60;
    int A4 = 69;

    // Calculate the frequency for C4 and A4 using the formula;
    // f = 2^((p - 69) / 12) * 440
    double frequencyC4 = pow(2, (C4 - 69) / 12.0) * 440.0;
    double frequencyA4 = pow(2, (A4 - 69) / 12.0) * 440.0;

    // Print the frequencies
    printf("C4 MIDI pitch: %d, Frequency: %.2f Hz\n", C4, frequencyC4);
    printf("A4 MIDI pitch: %d, Frequency: %.2f Hz\n", A4, frequencyA4);

    return 0;
}
