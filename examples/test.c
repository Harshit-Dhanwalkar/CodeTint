// This is a single-line C comment
#include <stdio.h>
#include <stdlib.h>

/*
 * This is a
 * multi-line C comment.
 */

#define MAX_ATTEMPTS 3
const int GLOBAL_COUNT = 10;

// Function prototype
int calculate_sum(int a, int b);

int main() {
    int x = 5; // variable declaration and initialization
    int y = 10;
    int sum = calculate_sum(x, y);

    printf("The sum of %d and %d is: %d\n", x, y, sum);

    if (sum > 10) {
        for (int i = 0; i < MAX_ATTEMPTS; i++) {
            printf("Attempt %d\n", i + 1);
        }
    } else {
        puts("Sum is not greater than 10.");
    }

    // A simple string literal
    char *message = "Hello, C language!";
    printf("%s\n", message);

    return 0;
}

// Function definition
int calculate_sum(int a, int b) {
    return a + b; // keyword return
}
