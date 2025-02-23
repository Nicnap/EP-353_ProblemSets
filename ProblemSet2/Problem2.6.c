#include <stdio.h>

int main() {
    // Array of intervals
    const char *intervals[] = {
        "unison", "minor second", "major second", "minor third", "major third", 
        "perfect fourth", "tritone", "perfect fifth", "minor sixth", 
        "major sixth", "minor seventh", "major seventh"
    };

    // Outer loop for the first note
    for (int note1 = 0; note1 < 12; note1++) {
        // Inner loop for the second note
        for (int note2 = 0; note2 < 12; note2++) {
            // Calculate the interval
            int interval = (note2 - note1 + 12) % 12;

            // Print
            printf("The interval from note %d to note %d is a %s.\n", 
                   note1, note2, intervals[interval]);
        }
        // Print a blank line between the rows
        printf("\n");
    }

    return 0;
}
