#include <stdio.h>
#include <string.h> // Include for strcmp
#include "levels.h" // Assuming this contains Player struct and update_player_score

// External functions (assuming they exist in your environment)
extern int ask_code_step(const char *instruction, const char *expected);
extern void read_line(char *buffer, int size);
extern void trim_spaces(char *s);

#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED   "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

/**
 * @brief Mini-exercise to test understanding of basic 'for' loop components.
 * @return int The points earned in this mini-exercise.
 */
static int run_mini_exercise_L2(void) {
    char answer[128];
    int test_score = 0;

    printf("\n=== Mini Exercise: For Loop Concepts ===\n");


    /* Test case 1: The correct syntax for initialization */
    printf("\nTest Case 1:\n");
    printf("Question: Write the correct C code line to initialize an integer variable 'i' to 0 for a loop (include the semicolon ';').\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "int i = 0;") == 0) {
        printf(COLOR_GREEN "Test Case 1 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "Test Case 1 failed. Expected: int i = 0;" COLOR_RESET "\n");
    }

    /* Test case 2: The correct syntax for the loop condition */
    printf("\nTest Case 2:\n");
    printf("Question: Write the condition that checks if 'i' is less than 5 (include the semicolon ';').\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "i < 5;") == 0) {
        printf(COLOR_GREEN "Test Case 2 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "Test Case 2 failed. Expected: i < 5;" COLOR_RESET "\n");
    }

    /* Test case 3: Writing the correct increment statement */
    printf("\nTest Case 3:\n");
    printf("Question: Write the statement to increment the variable 'i' by 1 using the shorthand post-increment operator.\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    // Accept both i++ and i=i+1 as common forms, but i++ is the target
    if (strcmp(answer, "i++") == 0 || strcmp(answer, "i=i+1") == 0) {
    printf(COLOR_GREEN "Test Case 3 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "Test Case 3 failed. Expected: i++" COLOR_RESET "\n");
    }

    printf("\nMini exercise finished. You passed %d / 3 test cases.\n", test_score);
    /* Convert to points, for example 3 points max per correct answer */
    return test_score * 3;  /* 0, 3, 6, or 9 points */
}

/**
 * @brief Main function for Level 2, focusing on 'for' loop practice.
 * @param player Pointer to the Player structure to update the score.
 */
void play_level2(Player *player) {
    int level_score = 0; // Initialize score for this level
    char buffer[64];     // Buffer for reading user input (e.g., readiness check)

    printf("\n=== Level 2: The 'For Loop' Statement ===\n");
    printf("In this level, you will practice using the fundamental C 'for' loop.\n");
    printf("A 'for' loop has three parts: Initialization, Condition, and Iteration.\n\n");

    // Ask if the player is ready
    printf("\nAre you ready to start? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
    printf("Okay, returning to the main menu.\n");
    return;
    }

    printf("\n--- Step-by-step Coding Practice ---\n");
    printf("Goal: Create a 'for' loop that prints the numbers from 0 to 4.\n");

    /* Step 1: for (int i = 0; */
    level_score += ask_code_step(
        "Step 1: Start the 'for' loop. Initialize an integer variable 'i' to 0 (including the opening parenthesis and first semicolon).",
        "for (int i = 0;"
    ) ? 3 : 0; // Added points if correct

    /* Step 2: i < 5; */
    level_score += ask_code_step(
        "Step 2: Add the loop condition so it continues as long as 'i' is less than 5 (include the second semicolon).",
        "i < 5;"
    ) ? 3 : 0;

    /* Step 3: i++) { */
    level_score += ask_code_step(
        "Step 3: Add the iteration statement to increment 'i' by 1, and open the loop block with '{'.",
        "i++) {"
    ) ? 3 : 0;

    /* Step 4: printf("%d\\n", i); */
    level_score += ask_code_step(
        "Step 4: Inside the loop, print the current value of 'i' followed by a newline.",
        "printf(\"%d\\n\", i);"
    ) ? 3 : 0;

    /* Step 5: } */
    level_score += ask_code_step(
        "Step 5: Close the 'for' loop block.",
        "}"
    ) ? 2 : 0;

    printf("\nYou have finished the guided coding steps. The full code would be:\n");
    printf("```c\nfor (int i = 0; i < 5; i++) {\n    printf(\"%%d\\n\", i);\n}\n```\n");

    printf("\nNow we will move on to a small exercise with test cases to check your understanding of 'for' loop concepts.\n");

    /* Run mini exercise and add its score */
    level_score += run_mini_exercise_L2();

    printf("\nLevel 2 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    // Update the player's total score
    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update your score in the player database.\n");
        }
        else {
            printf("No points earned this time. Review the 'for' loop concepts and try again!\n");
        }
    }
}