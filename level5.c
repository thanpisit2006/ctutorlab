// level5
// cat

#include <stdio.h>
#include <string.h>
#include "levels.h"

/* Ask the player to type a specific line of C code exactly.
 * (Assumed to be defined elsewhere, e.g., in level1.c or a shared file)
 */
extern int ask_code_step(const char *instruction, const char *expected);
extern void read_line(char *buffer, int size);
extern void trim_spaces(char *s);

#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED   "\x1b[31m"
#define COLOR_RESET "\x1b[0m"


/* Mini exercise with simple "test cases" about switch case concepts */
static int run_mini_exercise_L5(void) {
    char answer[128];
    int test_score = 0;

    printf("\n=== Mini Exercise: switch case Concepts ===\n");

    /* Test case 1: The 'default' keyword */
    printf("\nTest Case 1:\n");
    printf("Question: If no case matches the value in the switch statement, which command block is executed? (Answer with the specific C keyword)\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "default") == 0) {
        printf(COLOR_GREEN "Test Case 1 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "Test Case 1 failed. Expected: default" COLOR_RESET "\n");
    }

    /* Test case 2: Preventing fall-through */
    printf("\nTest Case 2:\n");
    printf("Question: Which statement must be placed at the end of each case block (unless fall-through is desired) to prevent the program from executing the next case? (Answer with the specific C keyword)\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "break") == 0) {
        printf(COLOR_GREEN "Test Case 2 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "Test Case 2 failed. Expected: break" COLOR_RESET "\n");
    }

    /* Test case 3: Writing a correct case for a character */
    printf("\nTest Case 3:\n");
    printf("Question: Write the correct C code line to start a case for the value 'A' (a character) in a switch statement (include the colon ':').\n");
    printf("Your answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "case 'A':") == 0) {
        printf(COLOR_GREEN "Test Case 3 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "Test Case 3 failed. Expected: case 'A':" COLOR_RESET "\n");
    }

    printf("\nMini exercise finished. You passed %d / 3 test cases.\n", test_score);
    /* Convert to points, for example 3 points max */
    return test_score * 3;  /* 0, 3, 6, or 9 points */
}

void play_level5(Player *player) {
    int level_score = 0; // Initialize score for this level
    char buffer[64];     // Buffer for reading user input (e.g., readiness check)

    printf("\n=== Level 5: The 'switch case' Statement ===\n");
    printf("In this level, you will practice using the 'switch case'\n");
    printf("control structure to handle multiple discrete values.\n\n");

    // Ask if the player is ready
    printf("\nAre you ready to start? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Okay, returning to the main menu.\n");
        return;
    }

    printf("\n--- Step-by-step Coding Practice ---\n");

    /* Step 1: int choice = 1; */
    level_score += ask_code_step(
        "Step 1: Declare an integer variable named 'choice' and initialize it to 1.",
        "int choice = 1;"
    ) ? 2 : 0; // Add points if correct

    /* Step 2: switch (choice) { */
    level_score += ask_code_step(
        "Step 2: Start the 'switch' control structure using the 'choice' variable.",
        "switch (choice) {"
    ) ? 2 : 0;

    /* Step 3: case 1: printf("Option 1 selected.\n"); break; */
    level_score += ask_code_step(
        "Step 3: Create a 'case' for the value 1, print the message 'Option 1 selected.', and include the 'break;' statement.",
        "case 1: printf(\"Option 1 selected.\\n\"); break;"
    ) ? 3 : 0;

    /* Step 4: case 2: printf("Option 2 selected.\n"); break; */
    level_score += ask_code_step(
        "Step 4: Create a 'case' for the value 2, print the message 'Option 2 selected.', and include the 'break;' statement.",
        "case 2: printf(\"Option 2 selected.\\n\"); break;"
    ) ? 3 : 0;

    /* Step 5: default: printf("Invalid choice.\n"); break; */
    level_score += ask_code_step(
        "Step 5: Create the 'default' case to print 'Invalid choice.', and include the 'break;' statement.",
        "default: printf(\"Invalid choice.\\n\"); break;"
    ) ? 3 : 0;

    /* Step 6: } */
    level_score += ask_code_step(
        "Step 6: Close the 'switch' statement block.",
        "}"
    ) ? 2 : 0;

    printf("\nYou have finished the guided coding steps.\n");
    printf("Now we will move on to a small exercise with test cases to check your understanding of switch concepts.\n");

    /* Run mini exercise and add its score */
    level_score += run_mini_exercise_L5();

    printf("\nLevel 5 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    // Update the player's total score
    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update your score in the player database.\n");
        }
    } else {
        printf("No points earned this time. Review the switch case concepts and try again!\n");
    }
}
