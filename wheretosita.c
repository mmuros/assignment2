#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 20
#define MAX_PEOPLE 10

int n; // Number of people attending the movie
int p; // Number of pairs of people who do not want to sit next to each other

char names[MAX_PEOPLE][MAX_NAME_LENGTH + 1]; // Array to store the names of the people
int popcorn[MAX_PEOPLE]; // Array to store whether each person has popcorn (1) or not (0)
int restrictions[MAX_PEOPLE][MAX_PEOPLE]; // 2D array to store the restrictions

int count = 0; // Variable to store the total number of valid orderings

// Function to check if a permutation is valid
int isValid(int* permutation) {
    for (int i = 0; i < n - 1; i++) {
        int person1 = permutation[i];
        int person2 = permutation[i + 1];

        // Check if the two people do not want to sit next to each other
        if (restrictions[person1][person2] || restrictions[person2][person1]) {
            return 0;
        }

        // Check if everyone has access to popcorn
        if (!popcorn[person1] && !popcorn[person2]) {
            return 0;
        }
    }

    return 1;
}

// Function to generate permutations using backtracking
void generatePermutations(int* permutation, int* used, int depth) {
    if (depth == n) {
        if (isValid(permutation)) {
            count++;
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            used[i] = 1;
            permutation[depth] = i;
            generatePermutations(permutation, used, depth + 1);
            used[i] = 0;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read the number of people and pairs of restrictions
    fscanf(fp, "%d %d", &n, &p);

    // Read the names and popcorn information for each person
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%s %d", names[i], &popcorn[i]);
    }

    // Initialize the restrictions array
    memset(restrictions, 0, sizeof(restrictions));

    // Read the pairs of restrictions
    for (int i = 0; i < p; i++) {
        char name1[MAX_NAME_LENGTH + 1];
        char name2[MAX_NAME_LENGTH + 1];
        fscanf(fp, "%s %s", name1, name2);

        int person1 = -1;
        int person2 = -1;

        // Find the indices of the two people in the names array
        for (int j = 0; j < n; j++) {
            if (strcmp(names[j], name1) == 0) {
                person1 = j;
            }
            if (strcmp(names[j], name2) == 0) {
                person2 = j;
            }
        }

        // Set the restriction in the restrictions array
        restrictions[person1][person2] = 1;
        restrictions[person2][person1] = 1;
    }

    fclose(fp);

    // Generate permutations and count the valid ones
    int permutation[MAX_PEOPLE];
    int used[MAX_PEOPLE] = {0};
    generatePermutations(permutation, used, 0);

    // Print the total number of valid orderings
    printf("%d\n", count);

    return 0;
}
