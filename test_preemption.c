#include "types.h"
#include "user.h"

void waste_time(int n) {
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 1000000; j++) ; // Waste some CPU cycles
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(2, "Usage: %s <num_iterations>\n", argv[0]);
        exit();
    }

    int num_iterations = atoi(argv[1]);
    printf(1, "Running for %d iterations...\n", num_iterations);

    int i;
    for (i = 0; i < num_iterations; i++) {
        waste_time(100); // Simulate some computation
        printf(1, "Iteration %d\n", i);
    }

    printf(1, "Test program completed.\n");
    exit();
}
