// level1
// bank

#include <stdio.h>
#include <string.h>
#include "levels.h"

#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED   "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

/* Helper: read one line from stdin and remove trailing newline */
static void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

/* Helper: trim leading and trailing spaces (in-place) */
static void trim_spaces(char *s) {
    char *start = s;
    char *end;

    /* skip leading spaces */
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    /* if string is all spaces */
    if (*start == '\0') {
        s[0] = '\0';
        return;
    }

    /* find end of string */
    end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t')) {
        end--;
    }

    /* write new terminator */
    *(end + 1) = '\0';

    /* move trimmed string back to s */
    if (start != s) {
        memmove(s, start, strlen(start) + 1);
    }
}

/* Ask the player to type a specific line of C code exactly.
 * If correct, show it in green and return 1.
 * If incorrect, show a red message and allow multiple retries.
 */
static int ask_code_step(const char *instruction, const char *expected) {
    char input[256];

    while (1) {
        printf("\n%s\n", instruction);
        printf("Your code: ");
        read_line(input, sizeof(input));
        trim_spaces(input);

        if (strcmp(input, expected) == 0) {
            printf(COLOR_GREEN "Correct: %s" COLOR_RESET "\n", input);
            return 1;
        } else {
            printf(COLOR_RED "Incorrect. Please try again." COLOR_RESET "\n");
        }
    }
}

/* Mini exercise with simple "test cases" about format specifiers */
static int run_mini_exercise(void) {
    char answer[128];
    int test_score = 0;

    printf("\n=== Mini Exercise: Format Specifiers ===\n");

    /* Test case 1 */
    printf("\nTest Case 1:\n");
    printf("Question: What is the format specifier for an integer (int)?\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer)); 
    trim_spaces(answer);               
    if (strcmp(answer, "%d") == 0) {
        printf(COLOR_GREEN "Test Case 1 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "Test Case 1 failed. Expected: %%d" COLOR_RESET "\n");
    }

    /* Test case 2 */
    printf("\nTest Case 2:\n");
    printf("Question: What is the format specifier for a string (char array)?\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "%s") == 0) {
        printf(COLOR_GREEN "Test Case 2 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "Test Case 2 failed. Expected: %%s" COLOR_RESET "\n");
    }

    /* Test case 3 */
    printf("\nTest Case 3:\n");
    printf("Write a complete printf statement that prints the integer 42\n");
    printf("with a newline. Use %%d as the format specifier.\n");
    printf("Example expected pattern: printf(\"Number: %%d\\n\", 42);\n");
    printf("Your answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "printf(\"%d\\n\", 42);") == 0) {
        printf(COLOR_GREEN "Test Case 3 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "Test Case 3 failed. One correct answer is:" COLOR_RESET "\n");
        printf("printf(\"%%d\\n\", 42);\n");
    }

    printf("\nMini exercise finished. You passed %d / 3 test cases.\n", test_score);
    /* Convert to points, for example 3 points max */
    return test_score * 3;  /* 0, 3, 6, or 9 points */
}

void play_level1(Player *player) {
    int level_score = 0;
    char buffer[64];

    printf("\n=== Level 1: Data Types and Format Specifiers ===\n");
    printf("In this level, you will practice basic C data types\n");
    printf("and format specifiers used with printf(), such as %%d and %%s.\n\n");

    printf("We will build a small piece of code step by step.\n");
    printf("You must type each line of code exactly as requested.\n");
    printf("If your code is correct, it will be shown in ");
    printf(COLOR_GREEN "green" COLOR_RESET ".\n");

    /* Ask if the player is ready */
    printf("\nAre you ready to start? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Okay, returning to the main menu.\n");
        return;
    }

    printf("\n--- Step-by-step Coding Practice ---\n");

    /* Step 1: int age; */
    level_score += ask_code_step(
        "Step 1: Declare an integer variable named age.",
        "int age;"
    ) ? 2 : 0;

    /* Step 2: char name[50]; */
    level_score += ask_code_step(
        "Step 2: Declare a char array named name that can hold 50 characters.",
        "char name[50];"
    ) ? 2 : 0;

    /* Step 3: printf line with %d */
    level_score += ask_code_step(
        "Step 3: Write a printf statement that prints the age using %d.\n"
        "Expected output: Age: 20 (assume age variable will be used).",
        "printf(\"Age: %d\\n\", age);"
    ) ? 3 : 0;

    /* Step 4: printf line with %s */
    level_score += ask_code_step(
        "Step 4: Write a printf statement that prints the name using %s.\n"
        "Expected output pattern: Name: <name>",
        "printf(\"Name: %s\\n\", name);"
    ) ? 3 : 0;

    printf("\nYou have finished the guided coding steps.\n");
    printf("Now we will move on to a small exercise with test cases.\n");

    /* Run mini exercise and add its score */
    level_score += run_mini_exercise();

    printf("\nLevel 1 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update your score in the player database.\n");
        }
    } else {
        printf("No points earned this time. Keep practicing!\n");
    }
}