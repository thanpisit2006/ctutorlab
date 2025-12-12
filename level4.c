// level4
// arrays
//ming

#include <stdio.h>
#include <string.h>
#include "levels.h"
#include "player.h"

/* These helper functions already exist in your project */
extern int ask_code_step(const char *instruction, const char *expected);
extern void read_line(char *buffer, int size);
extern void trim_spaces(char *s);

#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED   "\x1b[31m"
#define COLOR_RESET "\x1b[0m"


/* Mini exercise: test basic array concepts */
static int run_mini_exercise_L4(void) {
    char answer[128];
    int test_score = 0;

    printf("\n=== Mini Exercise: Array Concepts ===\n");

    /* Test Case 1 */
    printf("\nTest Case 1:\n");
    printf("Question: Given int a[3] = {4,5,6}; what is the value of a[0]? (Answer with the number)\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "4") == 0) {
        printf(COLOR_GREEN "Test Case 1 passed." COLOR_RESET "\n");
        test_score++;
    } else {
        printf(COLOR_RED "Test Case 1 failed. Expected: 4" COLOR_RESET "\n");
    }

    /* Test Case 2 */
    printf("\nTest Case 2:\n");
    printf("Question: What index represents the third element of an array? (Answer with the index number)\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "2") == 0) {
        printf(COLOR_GREEN "Test Case 2 passed." COLOR_RESET "\n");
        test_score++;
    } else {
        printf(COLOR_RED "Test Case 2 failed. Expected: 2" COLOR_RESET "\n");
    }

    /* Test Case 3 */
    printf("\nTest Case 3:\n");
    printf("Question: Write the correct expression to access the third element in array 'arr' (include semicolon).\n");
    printf("Expected example: arr[2];\n");
    printf("Your answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "arr[2];") == 0) {
        printf(COLOR_GREEN "Test Case 3 passed." COLOR_RESET "\n");
        test_score++;
    } else {
        printf(COLOR_RED "Test Case 3 failed. Expected: arr[2];" COLOR_RESET "\n");
    }

    printf("\nMini exercise finished. You passed %d / 3 test cases.\n", test_score);
    return test_score * 3;   // 0, 3, 6, or 9 points
}


/* Main Level 4 gameplay */
void play_level4(Player *player) {
    int level_score = 0;
    char buffer[64];

    printf("\n=== Level 4: Arrays ===\n");
    printf("In this level, you will practice declaring arrays, accessing elements,\n");
    printf("and using loops to compute the sum of array values.\n\n");

    printf("Are you ready to start? (y/n): ");
    read_line(buffer, sizeof(buffer));

    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Returning to main menu.\n");
        return;
    }

    printf("\n--- Guided Coding Steps ---\n");

    /* Step 1 */
    level_score += ask_code_step(
        "Step 1: Declare an integer array named 'arr' initialized with values 1, 2, and 3.",
        "int arr[3] = {1, 2, 3};"
    ) ? 2 : 0;

    /* Step 2 */
    level_score += ask_code_step(
        "Step 2: Declare an integer variable named 'sum' and initialize it to 0.",
        "int sum = 0;"
    ) ? 2 : 0;

    /* Step 3 */
    level_score += ask_code_step(
        "Step 3: Write a for-loop header to iterate i from 0 up to but not including 3 (include '{').",
        "for (int i = 0; i < 3; i++) {"
    ) ? 2 : 0;

    /* Step 4 */
    level_score += ask_code_step(
        "Step 4: Inside the loop, add the current array element to sum.",
        "sum += arr[i];"
    ) ? 3 : 0;

    /* Step 5 */
    level_score += ask_code_step(
        "Step 5: Close the for-loop block.",
        "}"
    ) ? 2 : 0;

    /* Step 6 */
    level_score += ask_code_step(
        "Step 6: Print the result using printf with the format \"Sum = %d\\n\".",
        "printf(\"Sum = %d\\n\", sum);"
    ) ? 3 : 0;


    printf("\nGreat! You finished the guided coding steps.\n");
    printf("Now let's check your understanding with a mini test.\n");

    level_score += run_mini_exercise_L4();

    printf("\nLevel 4 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    /* Update score in file/database */
    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update score.\n");
        }
    } else {
        printf("No points earned. Review array concepts and try again.\n");
    }
}
