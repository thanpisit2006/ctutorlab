/* =========================================================
   C Tutor Lab (Single File Version)
   - merged: player.h, player.c, levels.h, level1-5.c, main.c
   ========================================================= */

/*  AUTHOR: [Group 14]
        68070503415  Chadathong  Tangjitpakorn
        68070503422  Nichapa     Piromying
        68070503427  Thanpisit   Ritpetchnil
        68070503445  Peerawat    Boonyaprasit
        68070503457  Supakorn    Polphichai
    PROJECT: C TUTOR LAB
    PURPOSE: Term Project
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ===========================
   "player.h" (merged)
   =========================== */

#define MAX_NAME_LEN 31                 /* maximum length of a player name */
#define MAX_PLAYERS  100                /* maximum number of players */
#define PLAYER_DATA_FILE "players.txt"  /* file to store player data */

typedef struct {
    char name[MAX_NAME_LEN + 1];
    int  score;
} Player;

/* player function prototypes */
int  load_all_players(Player players[], int max);
int  find_player_index(const char *name, Player players[], int count);
int  save_all_players(Player players[], int count);
int  register_player(Player *out_player);
int  login_player(Player *out_player);
int  update_player_score(Player *player, int delta);
void print_player_info(const Player *player);

/* ===========================
   "levels.h" (merged)
   =========================== */

void play_level1(Player *player);
void play_level2(Player *player);
void play_level3(Player *player);
void play_level4(Player *player);
void play_level5(Player *player);

/* ===========================
   SCORING SYSTEM (UPDATED)
   =========================== */
/*
   NEW RULES:
   - Any normal correct answer = 5 points
   - Difficulty-based questions:
       EASY   = 5 points
       MEDIUM = 10 points
       HARD   = 15 points
*/

/* ===========================
   player.c (merged)
   =========================== */

static void clear_stdin_line(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

int load_all_players(Player players[], int max) {
    FILE *fp;
    int count = 0;

    fp = fopen(PLAYER_DATA_FILE, "r");
    if (fp == NULL) {
        return 0; /* no file -> no players */
    }

    while (count < max &&
           fscanf(fp, "%31s %d", players[count].name, &players[count].score) == 2) {
        count++;
    }

    fclose(fp);
    return count;
}

int save_all_players(Player players[], int count) {
    FILE *fp;
    int i;

    fp = fopen(PLAYER_DATA_FILE, "w");
    if (fp == NULL) {
        printf("Error: cannot open player data file for writing.\n");
        return 0;
    }

    for (i = 0; i < count; i++) {
        fprintf(fp, "%s %d\n", players[i].name, players[i].score);
    }

    fclose(fp);
    return 1;
}

int find_player_index(const char *name, Player players[], int count) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(name, players[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

int register_player(Player *out_player) {
    Player players[MAX_PLAYERS];
    int count;
    char name[MAX_NAME_LEN + 1];

    count = load_all_players(players, MAX_PLAYERS);

    printf("\n=== Register ===\n");
    printf("Enter new player name (no spaces): ");
    if (scanf("%31s", name) != 1) {
        printf("Invalid input.\n");
        clear_stdin_line();
        return 0;
    }
    clear_stdin_line();

    if (find_player_index(name, players, count) != -1) {
        printf("This name is already taken. Please try again.\n");
        return 0;
    }

    if (count >= MAX_PLAYERS) {
        printf("Player list is full. Cannot register a new player.\n");
        return 0;
    }

    strcpy(players[count].name, name);
    players[count].score = 0;
    count++;

    if (!save_all_players(players, count)) {
        printf("Failed to save player data.\n");
        return 0;
    }

    strcpy(out_player->name, name);
    out_player->score = 0;

    printf("Registration successful. Welcome, %s!\n", out_player->name);
    return 1;
}

int login_player(Player *out_player) {
    Player players[MAX_PLAYERS];
    int count;
    char name[MAX_NAME_LEN + 1];
    int idx;

    count = load_all_players(players, MAX_PLAYERS);

    printf("\n=== Login ===\n");
    printf("Enter player name: ");
    if (scanf("%31s", name) != 1) {
        printf("Invalid input.\n");
        clear_stdin_line();
        return 0;
    }
    clear_stdin_line();

    if (count == 0) {
        printf("No players found. Please register first.\n");
        return 0;
    }

    idx = find_player_index(name, players, count);
    if (idx == -1) {
        printf("Player not found. Please register first.\n");
        return 0;
    }

    strcpy(out_player->name, players[idx].name);
    out_player->score = players[idx].score;

    printf("Login successful. Welcome back, %s! (Score: %d)\n",
           out_player->name, out_player->score);
    return 1;
}

int update_player_score(Player *player, int delta) {
    Player players[MAX_PLAYERS];
    int count;
    int idx;

    count = load_all_players(players, MAX_PLAYERS);
    if (count == 0) {
        printf("Error: no player data found.\n");
        return 0;
    }

    idx = find_player_index(player->name, players, count);
    if (idx == -1) {
        printf("Error: current player not found in data file.\n");
        return 0;
    }

    players[idx].score += delta;
    if (players[idx].score < 0) {
        players[idx].score = 0;
    }

    player->score = players[idx].score;

    if (!save_all_players(players, count)) {
        printf("Failed to update player data file.\n");
        return 0;
    }

    return 1;
}

void print_player_info(const Player *player) {
    printf("\n=== Player Info ===\n");
    printf("Name : %s\n", player->name);
    printf("Score: %d\n", player->score);
}

/* ===========================
   shared helpers + level1.c (merged)
   =========================== */

#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED   "\x1b[31m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"

#define MAX_RETRY 3  /* for NON Easy/Medium/Hard parts */

static void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
}

static void trim_spaces(char *s) {
    char *start = s;
    char *end;

    while (*start == ' ' || *start == '\t') start++;

    if (*start == '\0') {
        s[0] = '\0';
        return;
    }

    end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t')) end--;

    *(end + 1) = '\0';

    if (start != s) {
        memmove(s, start, strlen(start) + 1);
    }
}

static void remove_spaces_tabs(char *s) {
    char *src = s, *dst = s;
    while (*src) {
        if (*src != ' ' && *src != '\t') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

/* =========================================================
   strict step checker -> retry with hint (NOT E/M/H)
   ========================================================= */
static int ask_code_step_retry(const char *instruction,
                               const char *expected,
                               const char *hint) {
    char answer[256];
    int attempt;

    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("\n%s\n", instruction);
        printf("Your answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);

        if (strcmp(answer, expected) == 0) {
            printf(COLOR_GREEN "✓ Correct!\n" COLOR_RESET);
            return 1;
        }

        printf(COLOR_RED "✗ Incorrect." COLOR_RESET "\n");
        printf(COLOR_YELLOW "Hint:" COLOR_RESET " %s\n", hint);

        if (attempt < MAX_RETRY) {
            printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
        } else {
            printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " %s\n", expected);
        }
    }

    return 0;
}

/* keep old name used by many levels */
int ask_code_step(const char *instruction, const char *expected) {
    const char *generic_hint =
        "Check spaces/symbols carefully. Common mistakes: missing ;, missing quotes, missing \\n, wrong brackets.";
    return ask_code_step_retry(instruction, expected, generic_hint);
}

/* ---------- L1 Explanation helpers ---------- */
static void explain_mistake_L1(const char *expected, int test_case) {
    (void)expected;

    printf(COLOR_YELLOW "\n💡 Explanation: " COLOR_RESET);

    switch (test_case) {
        case 1: /* %d */
            printf("%%d is the format specifier for an integer (int).\n");
            printf("Example: printf(\"Age: %%d\\n\", age);\n");
            break;

        case 2: /* %s */
            printf("%%s is the format specifier for a string (char array).\n");
            printf("Example: printf(\"Name: %%s\\n\", name);\n");
            break;

        case 3: /* printf statement */
            printf("A complete printf statement needs:\n");
            printf("- printf( ... );\n");
            printf("- Double quotes \"...\" around the message\n");
            printf("- \\n for a newline\n");
            printf("- A semicolon ; at the end\n");
            printf("Example: printf(\"%%d\\n\", 42);\n");
            break;

        case 4: /* scanf int */
            printf("scanf for int uses %%d and MUST use & to store into the variable.\n");
            printf("Example: scanf(\"%%d\", &age);\n");
            break;

        case 5: /* float %f */
            printf("%%f is used to print floating-point numbers (float/double) with printf.\n");
            printf("Example: printf(\"%%f\\n\", price);\n");
            break;

        default:
            printf("Check the syntax carefully (symbols, quotes, spaces, semicolon).\n");
            break;
    }

    printf("\n");
}

/* ---------- L1 Mini Exercise (syntax check) ---------- */
static int run_mini_exercise_L1(void) {
    char answer[256];
    int test_passed = 0;
    int attempt;

    printf("\n" COLOR_CYAN "=== Mini Exercise (Syntax): Format Specifiers & printf ===" COLOR_RESET "\n");
    printf("Scoring: 5 points per correct answer\n");
    printf("(You can retry up to %d times per question)\n", MAX_RETRY);

    /* Test Case 1 */
    printf("\nTest Case 1:\n");
    printf("Question: What is the format specifier for an integer (int)?\n");

    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);

        if (strcmp(answer, "%d") == 0) {
            printf(COLOR_GREEN "✓ Test Case 1 passed. (+5)\n" COLOR_RESET);
            test_passed += 1;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " It's %%d (percent + d)\n");
            explain_mistake_L1("%d", 1);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " %%d\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    /* Test Case 2 */
    printf("\nTest Case 2:\n");
    printf("Question: What is the format specifier for a string (char array)?\n");

    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);

        if (strcmp(answer, "%s") == 0) {
            printf(COLOR_GREEN "✓ Test Case 2 passed. (+5)\n" COLOR_RESET);
            test_passed += 1;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " It's %%s (percent + s)\n");
            explain_mistake_L1("%s", 2);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " %%s\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    /* Test Case 3 */
    printf("\nTest Case 3:\n");
    printf("Question: Write a complete printf statement that prints the integer 42 with a newline.\n");
    printf("One valid answer: printf(\"%%d\\n\", 42);\n");

    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);

        if (strcmp(answer, "printf(\"%d\\n\", 42);") == 0) {
            printf(COLOR_GREEN "✓ Test Case 3 passed. (+5)\n" COLOR_RESET);
            test_passed += 1;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " Need printf(\"%%d\\n\", 42); (quotes, \\n, semicolon)\n");
            explain_mistake_L1("printf(\"%d\\n\", 42);", 3);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " printf(\"%%d\\n\", 42);\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    printf("\nMini exercise finished. Passed %d / 3.\n", test_passed);
    return test_passed * 5;
}

/* ---------- L1 Additional Exercise: Easy/Medium/Hard ---------- */
static int run_additional_exercise_L1(void) {
    char answer[256];
    int score = 0;

    printf("\n" COLOR_CYAN "=== Additional Exercise: Easy / Medium / Hard ===" COLOR_RESET "\n");
    printf("Scoring: EASY=5, MEDIUM=10, HARD=15\n");

    /* EASY */
    printf("\n" COLOR_CYAN "[EASY]" COLOR_RESET " Question 1:\n");
    printf("Question: What is the printf format specifier for a floating-point number?\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "%f") == 0) {
        printf(COLOR_GREEN "✓ Correct (+5 points)\n" COLOR_RESET);
        score += 5;
    } else {
        printf(COLOR_RED "✗ Incorrect. Expected: %%f\n" COLOR_RESET);
        explain_mistake_L1("%f", 5);
    }

    /* MEDIUM */
    printf("\n" COLOR_CYAN "[MEDIUM]" COLOR_RESET " Question 2:\n");
    printf("Question: Write a printf statement that prints a string variable name with newline.\n");
    printf("Expected pattern: printf(\"Hello, %%s\\n\", name);\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "printf(\"Hello, %s\\n\", name);") == 0) {
        printf(COLOR_GREEN "✓ Correct (+10 points)\n" COLOR_RESET);
        score += 10;
    } else {
        printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
        printf("One valid answer: printf(\"Hello, %%s\\n\", name);\n");
        printf(COLOR_YELLOW "💡 Explanation: Use %%s for strings, include \\n, and end with ;\n" COLOR_RESET);
    }

    /* HARD */
    printf("\n" COLOR_CYAN "[HARD]" COLOR_RESET " Question 3:\n");
    printf("Question: Write a scanf statement to read an integer into variable age.\n");
    printf("Requirement: Use %%d and include &age and semicolon ;\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "scanf(\"%d\", &age);") == 0) {
        printf(COLOR_GREEN "✓ Correct (+15 points)\n" COLOR_RESET);
        score += 15;
    } else {
        printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
        printf("Expected: scanf(\"%%d\", &age);\n");
        explain_mistake_L1("scanf(\"%d\", &age);", 4);
    }

    printf("\nAdditional exercise finished. You earned %d points.\n", score);
    return score;
}

/* ---------- Level 1 main ---------- */
void play_level1(Player *player) {
    int level_score = 0;
    char buffer[64];

    printf("\n=== Level 1: Data Types and Format Specifiers ===\n");

    /* 1) Teaching */
    printf("\n--- Teaching ---\n");
    printf("In C, printf() uses format specifiers to print values:\n");
    printf("  - %%d for int (integer)\n");
    printf("  - %%s for string (char array)\n");
    printf("  - %%f for float/double\n\n");
    printf("Example:\n");
    printf("  int age = 20;\n");
    printf("  char name[50] = \"Bank\";\n");
    printf("  printf(\"Age: %%d\\n\", age);\n");
    printf("  printf(\"Name: %%s\\n\", name);\n");
    printf("\nPress Enter to continue...");
    read_line(buffer, sizeof(buffer));

    printf("\nAre you ready to start? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Okay, returning to the main menu.\n");
        return;
    }

    /* 2) Step-by-step practice: retry with hint */
    printf("\n--- Step-by-step Coding Practice ---\n");
    printf("Scoring: 5 points per correct answer\n");
    printf("(Retry allowed up to %d times per step)\n", MAX_RETRY);

    level_score += ask_code_step_retry(
        "Step 1: Declare an integer variable named age.",
        "int age;",
        "Declare with type + name + semicolon. Example: int age;"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 2: Declare a char array named name that can hold 50 characters.",
        "char name[50];",
        "Arrays use brackets []. Example: char name[50];"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 3: Write a printf statement that prints the age using %d.",
        "printf(\"Age: %d\\n\", age);",
        "Need printf(\"...%d...\\n\", age); and end with ;"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 4: Write a printf statement that prints the name using %s.",
        "printf(\"Name: %s\\n\", name);",
        "Need printf(\"...%s...\\n\", name); and end with ;"
    ) ? 5 : 0;

    printf("\nYou finished the guided steps.\n");

    /* 3) Mini exercise (retry allowed) */
    level_score += run_mini_exercise_L1();

    /* 4) Additional exercise (E/M/H unchanged) */
    level_score += run_additional_exercise_L1();

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

/* ===========================
   level2-5.c (merged)
   =========================== */

static int read_int_in_range(const char *prompt, int min, int max) {
    char buf[64];
    int val;

    while (1) {
        printf("%s", prompt);
        read_line(buf, sizeof(buf));
        trim_spaces(buf);

        if (sscanf(buf, "%d", &val) == 1 && val >= min && val <= max) {
            return val;
        }
        printf(COLOR_RED "Please enter a number (%d-%d).\n" COLOR_RESET, min, max);
    }
}

/* ---------------------------
   LEVEL 2
   --------------------------- */
static void explain_mistake_L2(const char *user_answer, const char *expected, int test_case) {
    (void)user_answer;
    (void)expected;
    printf(COLOR_YELLOW "\n💡 Explanation: " COLOR_RESET);

    switch(test_case) {
        case 1:
            printf("The correct syntax is: 'int i = 0;'\n");
            printf("- 'int' declares the variable type\n");
            printf("- 'i' is the variable name\n");
            printf("- '= 0' initializes it to 0\n");
            printf("- ';' ends the statement\n");
            break;

        case 2:
            printf("The condition 'i < 5;' means:\n");
            printf("- Continue the loop while i is less than 5\n");
            printf("- This will run for i = 0, 1, 2, 3, 4 (5 times total)\n");
            printf("- Don't forget the semicolon!\n");
            break;

        case 3:
            printf("The increment operator 'i++' means:\n");
            printf("- Add 1 to the value of i after each loop iteration\n");
            printf("- It's shorthand for 'i = i + 1'\n");
            break;
    }
    printf("\n");
}

/* retry with hint (NOT E/M/H) */
static int run_mini_exercise_L2(void) {
    char answer[128];
    int passed = 0;
    int attempt;

    printf("\n" COLOR_CYAN "=== Mini Exercise: For Loop Concepts ===" COLOR_RESET "\n");
    printf("Scoring: 5 points per correct answer\n");
    printf("(You can retry up to %d times per question)\n", MAX_RETRY);

    printf("\nTest Case 1:\n");
    printf("Question: Write the correct C code line to initialize an integer variable 'i' to 0 for a loop (include ';').\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);
        if (strcmp(answer, "int i = 0;") == 0) {
            printf(COLOR_GREEN "✓ Test Case 1 passed. (+5)\n" COLOR_RESET);
            passed += 1;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " Need type + name + = 0 + ;\n");
            explain_mistake_L2(answer, "int i = 0;", 1);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " int i = 0;\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    printf("\nTest Case 2:\n");
    printf("Question: Write the condition that checks if 'i' is less than 5 (include ';').\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);
        if (strcmp(answer, "i < 5;") == 0) {
            printf(COLOR_GREEN "✓ Test Case 2 passed. (+5)\n" COLOR_RESET);
            passed += 1;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " Condition is i < 5; (with semicolon)\n");
            explain_mistake_L2(answer, "i < 5;", 2);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " i < 5;\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    printf("\nTest Case 3:\n");
    printf("Question: Write the statement to increment the variable 'i' by 1 using the shorthand post-increment operator.\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);
        if (strcmp(answer, "i++") == 0 || strcmp(answer, "i=i+1") == 0) {
            printf(COLOR_GREEN "✓ Test Case 3 passed. (+5)\n" COLOR_RESET);
            passed += 1;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " Use i++ (no semicolon required here)\n");
            explain_mistake_L2(answer, "i++", 3);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " i++\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    printf("\nMini exercise finished. Passed %d / 3.\n", passed);
    return passed * 5;
}

static int run_additional_exercise_L2(void) {
    char answer[128];
    int score = 0;

    printf("\n" COLOR_CYAN "=== Additional Exercise: Advanced For Loop Questions ===" COLOR_RESET "\n");
    printf("Scoring: EASY=5, MEDIUM=10, HARD=15\n");

    /* EASY */
    printf("\n" COLOR_CYAN "[EASY]" COLOR_RESET " Question 1:\n");
    printf("Question: Write a for loop initialization to start counting from 1 (include ';').\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "int i = 1;") == 0) {
        printf(COLOR_GREEN "✓ Correct (+5)\n" COLOR_RESET);
        score += 5;
    } else {
        printf(COLOR_RED "✗ Incorrect. Expected: int i = 1;\n" COLOR_RESET);
        printf(COLOR_YELLOW "💡 To start from 1, initialize i with value 1 instead of 0.\n" COLOR_RESET);
    }

    /* MEDIUM */
    printf("\n" COLOR_CYAN "[MEDIUM]" COLOR_RESET " Question 2:\n");
    printf("Question: Write a for loop that counts DOWN from 10 to 1.\n");
    printf("Write the complete for loop header (without the body).\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "for (int i = 10; i >= 1; i--)") == 0 ||
        strcmp(answer, "for (int i = 10; i > 0; i--)") == 0) {
        printf(COLOR_GREEN "✓ Correct (+10)\n" COLOR_RESET);
        score += 10;
    } else {
        printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
        printf("Expected: for (int i = 10; i >= 1; i--)\n");
        printf(COLOR_YELLOW "💡 Start at 10, keep i >= 1, and decrement with i--.\n" COLOR_RESET);
    }

    /* HARD */
    printf("\n" COLOR_CYAN "[HARD]" COLOR_RESET " Question 3:\n");
    printf("Question: Write a for loop that prints only EVEN numbers from 0 to 10 (inclusive).\n");
    printf("Write the complete for loop header (without the body).\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "for (int i = 0; i <= 10; i += 2)") == 0 ||
        strcmp(answer, "for (int i = 0; i <= 10; i=i+2)") == 0) {
        printf(COLOR_GREEN "✓ Correct (+15)\n" COLOR_RESET);
        score += 15;
    } else {
        printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
        printf("Expected: for (int i = 0; i <= 10; i += 2)\n");
        printf(COLOR_YELLOW "💡 Even numbers: start 0, go to 10, step +2.\n" COLOR_RESET);
    }

    printf("\nAdditional exercise finished. You earned %d points.\n", score);
    return score;
}

void play_level2(Player *player) {
    int level_score = 0;
    char buffer[64];

    printf("\n=== Level 2: The 'For Loop' Statement ===\n");
    printf("In this level, you will practice using the fundamental C 'for' loop.\n");
    printf("A 'for' loop has three parts: Initialization, Condition, and Iteration.\n\n");

    printf("\nAre you ready to start? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Okay, returning to the main menu.\n");
        return;
    }

    printf("\n--- Step-by-step Coding Practice ---\n");
    printf("Goal: Create a 'for' loop that prints the numbers from 0 to 4.\n");
    printf("Scoring: 5 points per correct answer\n");
    printf("(Retry allowed up to %d times per step)\n", MAX_RETRY);

    level_score += ask_code_step_retry(
        "Step 1: Start the 'for' loop. Initialize an integer variable 'i' to 0 (include the opening parenthesis and first semicolon).",
        "for (int i = 0;",
        "The first part is initialization: for (int i = 0;  (include the semicolon)"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 2: Add the loop condition so it continues as long as 'i' is less than 5 (include the second semicolon).",
        "i < 5;",
        "Condition goes in the middle: i < 5;  (include semicolon)"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 3: Add the iteration statement to increment 'i' by 1, and open the loop block with '{'.",
        "i++) {",
        "Iteration is i++ then close ) and open block: i++) {"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 4: Inside the loop, print the current value of 'i' followed by a newline.",
        "printf(\"%d\\n\", i);",
        "Use printf(\"%d\\n\", i); (quotes + \\n + semicolon)"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 5: Close the 'for' loop block.",
        "}",
        "Just a closing brace: }"
    ) ? 5 : 0;

    printf("\nYou have finished the guided coding steps. The full code would be:\n");
    printf("```c\nfor (int i = 0; i < 5; i++) {\n    printf(\"%%d\\n\", i);\n}\n```\n");

    printf("\nNow we will move on to the mini exercise with test cases.\n");
    level_score += run_mini_exercise_L2();

    printf("\nGreat! Now let's try some more challenging questions.\n");
    level_score += run_additional_exercise_L2();

    printf("\nLevel 2 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update your score in the player database.\n");
        }
    } else {
        printf("No points earned this time. Review the 'for' loop concepts and try again!\n");
    }
}

/* ---------------------------
   LEVEL 3: if-else  (UPDATED to match Level 2 structure)
   --------------------------- */

static void explain_mistake_L3(int test_case) {
    printf(COLOR_YELLOW "\n💡 Explanation: " COLOR_RESET);
    switch (test_case) {
        case 1:
            printf("An if statement starts with: if (condition) {\n");
            printf("Make sure you have parentheses () and an opening brace {.\n");
            break;
        case 2:
            printf("An else block starts with: else {\n");
            printf("It runs when the if condition is false.\n");
            break;
        case 3:
            printf("A full if-else usually looks like:\n");
            printf("if (condition) { ... } else { ... }\n");
            printf("Don’t forget braces {} and semicolons ; inside.\n");
            break;
    }
    printf("\n");
}

/* Mini exercise for L3 (NOT E/M/H) -> retry with hint */
static int run_mini_exercise_L3(void) {
    char answer[256];
    int passed = 0;
    int attempt;

    printf("\n" COLOR_CYAN "=== Mini Exercise: if–else Basics ===" COLOR_RESET "\n");
    printf("Scoring: 5 points per correct answer\n");
    printf("(You can retry up to %d times per question)\n", MAX_RETRY);

    /* Test 1 */
    printf("\nTest Case 1:\n");
    printf("Question: Write an if statement header that checks if x is greater than 0 (include '{').\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);

        if (strcmp(answer, "if (x > 0) {") == 0) {
            printf(COLOR_GREEN "✓ Test Case 1 passed. (+5)\n" COLOR_RESET);
            passed++;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " Use: if (x > 0) {\n");
            explain_mistake_L3(1);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " if (x > 0) {\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    /* Test 2 */
    printf("\nTest Case 2:\n");
    printf("Question: Write the else block header (include '{').\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);

        if (strcmp(answer, "else {") == 0) {
            printf(COLOR_GREEN "✓ Test Case 2 passed. (+5)\n" COLOR_RESET);
            passed++;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " It is exactly: else {\n");
            explain_mistake_L3(2);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " else {\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    /* Test 3 */
    printf("\nTest Case 3:\n");
    printf("Question: Write a complete one-line if-else that prints \"Yes\\n\" if a==1 else prints \"No\\n\".\n");
    printf("Answer must be exactly one line.\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);

        /* compare after removing spaces/tabs to reduce formatting issues */
        {
            char tmp[256];
            strncpy(tmp, answer, sizeof(tmp) - 1);
            tmp[sizeof(tmp) - 1] = '\0';
            remove_spaces_tabs(tmp);

            if (strcmp(tmp, "if(a==1){printf(\"Yes\\n\");}else{printf(\"No\\n\");}") == 0) {
                printf(COLOR_GREEN "✓ Test Case 3 passed. (+5)\n" COLOR_RESET);
                passed++;
                break;
            }
        }

        printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
        printf(COLOR_YELLOW "Hint:" COLOR_RESET " Pattern: if (a==1) { ... } else { ... }\n");
        explain_mistake_L3(3);
        if (attempt == MAX_RETRY) {
            printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " if (a == 1) { printf(\"Yes\\n\"); } else { printf(\"No\\n\"); }\n");
        } else {
            printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
        }
    }

    printf("\nMini exercise finished. Passed %d / 3.\n", passed);
    return passed * 5;
}

/* Additional exercise for L3 (E/M/H) */
static int run_additional_exercise_L3(void) {
    char answer[256];
    int score = 0;

    printf("\n" COLOR_CYAN "=== Additional Exercise: if–else (Easy / Medium / Hard) ===" COLOR_RESET "\n");
    printf("Scoring: EASY=5, MEDIUM=10, HARD=15\n");

    /* EASY */
    printf("\n" COLOR_CYAN "[EASY]" COLOR_RESET " Question 1:\n");
    printf("Question: Write an if statement header that checks if score is equal to 100 (include '{').\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "if (score == 100) {") == 0) {
        printf(COLOR_GREEN "✓ Correct (+5)\n" COLOR_RESET);
        score += 5;
    } else {
        printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
        printf("Expected: if (score == 100) {\n");
    }

    /* MEDIUM */
    printf("\n" COLOR_CYAN "[MEDIUM]" COLOR_RESET " Question 2:\n");
    printf("Question: Write a complete one-line if-else:\n");
    printf("- If score >= 50 print \"Pass\\n\"\n");
    printf("- Else print \"Fail\\n\"\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    {
        char tmp[256];
        strncpy(tmp, answer, sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';
        remove_spaces_tabs(tmp);

        if (strcmp(tmp, "if(score>=50){printf(\"Pass\\n\");}else{printf(\"Fail\\n\");}") == 0) {
            printf(COLOR_GREEN "✓ Correct (+10)\n" COLOR_RESET);
            score += 10;
        } else {
            printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
            printf("One valid answer:\n");
            printf("if (score >= 50) { printf(\"Pass\\n\"); } else { printf(\"Fail\\n\"); }\n");
        }
    }

    /* HARD */
    printf("\n" COLOR_CYAN "[HARD]" COLOR_RESET " Question 3:\n");
    printf("Question: Write one-line if / else if / else for age:\n");
    printf("- age < 13  -> print \"Child\\n\"\n");
    printf("- age < 20  -> print \"Teen\\n\"\n");
    printf("- else      -> print \"Adult\\n\"\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    {
        char tmp[256];
        strncpy(tmp, answer, sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';
        remove_spaces_tabs(tmp);

        if (strcmp(tmp, "if(age<13){printf(\"Child\\n\");}else if(age<20){printf(\"Teen\\n\");}else{printf(\"Adult\\n\");}") == 0) {
            printf(COLOR_GREEN "✓ Correct (+15)\n" COLOR_RESET);
            score += 15;
        } else {
            printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
            printf("One valid answer:\n");
            printf("if (age < 13) { printf(\"Child\\n\"); } else if (age < 20) { printf(\"Teen\\n\"); } else { printf(\"Adult\\n\"); }\n");
        }
    }

    printf("\nAdditional exercise finished. You earned %d points.\n", score);
    return score;
}

void play_level3(Player *player) {
    int level_score = 0;
    char buffer[64];

    printf("\n=== Level 3: IF–ELSE Decision Making ===\n");

    /* 1) Teaching */
    printf("\n--- Teaching ---\n");
    printf("if–else is used to make decisions in C.\n");
    printf("- if (condition) { ... } runs when condition is true\n");
    printf("- else { ... } runs when condition is false\n\n");
    printf("Example:\n");
    printf("int x = 5;\n");
    printf("if (x > 0) {\n");
    printf("    printf(\"Positive\\n\");\n");
    printf("} else {\n");
    printf("    printf(\"Not positive\\n\");\n");
    printf("}\n");
    printf("\nPress Enter to continue...");
    read_line(buffer, sizeof(buffer));

    printf("\nAre you ready to start? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Okay, returning to the main menu.\n");
        return;
    }

    /* 2) Step-by-step practice (like L2) */
    printf("\n--- Step-by-step Coding Practice ---\n");
    printf("Goal: Build an if-else that prints \"Positive\" when x > 0, otherwise prints \"Not Positive\".\n");
    printf("Scoring: 5 points per correct step\n");
    printf("(Retry allowed up to %d times per step)\n", MAX_RETRY);

    level_score += ask_code_step_retry(
        "Step 1: Write the if statement header that checks x > 0 (include '{').",
        "if (x > 0) {",
        "Pattern: if (condition) {"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 2: Inside the if block, print \"Positive\\n\".",
        "printf(\"Positive\\n\");",
        "Use printf(\"Positive\\n\"); (quotes + \\n + semicolon)"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 3: Close the if block.",
        "}",
        "Just a closing brace: }"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 4: Start the else block (include '{').",
        "else {",
        "Else header is: else {"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 5: Inside the else block, print \"Not Positive\\n\".",
        "printf(\"Not Positive\\n\");",
        "Use printf(\"Not Positive\\n\");"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 6: Close the else block.",
        "}",
        "Just }"
    ) ? 5 : 0;

    printf("\nYou have finished the guided coding steps. The full code would be:\n");
    printf("```c\nif (x > 0) {\n    printf(\"Positive\\n\");\n} else {\n    printf(\"Not Positive\\n\");\n}\n```\n");

    /* 3) Mini exercise (retry) */
    printf("\nNow we will move on to the mini exercise with test cases.\n");
    level_score += run_mini_exercise_L3();

    /* 4) Additional exercise (E/M/H) */
    printf("\nGreat! Now let's try some more challenging questions.\n");
    level_score += run_additional_exercise_L3();

    printf("\nLevel 3 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update your score.\n");
        }
    } else {
        printf("No points earned this time. Review if–else concepts and try again!\n");
    }
}

/* ---------------------------
   LEVEL 4: Arrays (UPDATED to match Level 2 structure)
   --------------------------- */

static void explain_mistake_L4(const char *expected, int test_case) {
    (void)expected;
    printf(COLOR_YELLOW "\n💡 Explanation: " COLOR_RESET);

    switch (test_case) {
        case 1:
            printf("Array indexing in C starts from 0.\n");
            printf("So a[0] refers to the first element.\n");
            break;
        case 2:
            printf("The third element has index 2.\n");
            printf("Indexing goes 0, 1, 2.\n");
            break;
        case 3:
            printf("To access the third element, use arr[2];\n");
            printf("Square brackets [] are required.\n");
            break;
    }
    printf("\n");
}

/* Mini exercise (retry) */
static int run_mini_exercise_L4(void) {
    char answer[128];
    int passed = 0;
    int attempt;

    printf("\n" COLOR_CYAN "=== Mini Exercise: Array Concepts ===" COLOR_RESET "\n");
    printf("Scoring: 5 points per correct answer\n");
    printf("(You can retry up to %d times per question)\n", MAX_RETRY);

    printf("\nTest Case 1:\n");
    printf("Question: Given int a[3] = {4, 5, 6}; what is the value of a[0]?\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);

        if (strcmp(answer, "4") == 0) {
            printf(COLOR_GREEN "✓ Test Case 1 passed. (+5)\n" COLOR_RESET);
            passed++;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " Index 0 is the FIRST element.\n");
            explain_mistake_L4("4", 1);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " 4\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    printf("\nTest Case 2:\n");
    printf("Question: What index represents the third element of an array?\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);

        if (strcmp(answer, "2") == 0) {
            printf(COLOR_GREEN "✓ Test Case 2 passed. (+5)\n" COLOR_RESET);
            passed++;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " Counting indices: 0, 1, 2 -> third is 2\n");
            explain_mistake_L4("2", 2);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " 2\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    printf("\nTest Case 3:\n");
    printf("Question: Write the correct C expression to access the third element of array 'arr' (include semicolon).\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);

        if (strcmp(answer, "arr[2];") == 0) {
            printf(COLOR_GREEN "✓ Test Case 3 passed. (+5)\n" COLOR_RESET);
            passed++;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " Use brackets: arr[2];\n");
            explain_mistake_L4("arr[2];", 3);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " arr[2];\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    printf("\nMini exercise finished. Passed %d / 3.\n", passed);
    return passed * 5;
}

/* Additional exercise for L4 (E/M/H) */
static int run_additional_exercise_L4(void) {
    char answer[256];
    int score = 0;

    printf("\n" COLOR_CYAN "=== Additional Exercise: Arrays (Easy / Medium / Hard) ===" COLOR_RESET "\n");
    printf("Scoring: EASY=5, MEDIUM=10, HARD=15\n");

    /* EASY */
    printf("\n" COLOR_CYAN "[EASY]" COLOR_RESET " Question 1:\n");
    printf("Given: int b[4] = {9, 8, 7, 6}; What is b[1]?\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "8") == 0) {
        printf(COLOR_GREEN "✓ Correct (+5)\n" COLOR_RESET);
        score += 5;
    } else {
        printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
        printf("Expected: 8\n");
        printf(COLOR_YELLOW "💡 Index 1 is the second element.\n" COLOR_RESET);
    }

    /* MEDIUM */
    printf("\n" COLOR_CYAN "[MEDIUM]" COLOR_RESET " Question 2:\n");
    printf("Question: Write a for-loop header that iterates i from 0 to n-1 (no body).\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "for (int i = 0; i < n; i++)") == 0 ||
        strcmp(answer, "for (int i=0; i<n; i++)") == 0) {
        printf(COLOR_GREEN "✓ Correct (+10)\n" COLOR_RESET);
        score += 10;
    } else {
        printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
        printf("Expected: for (int i = 0; i < n; i++)\n");
    }

    /* HARD */
    printf("\n" COLOR_CYAN "[HARD]" COLOR_RESET " Question 3:\n");
    printf("Question: Write the C expression to access the last element of array arr with size n (include semicolon).\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "arr[n-1];") == 0 || strcmp(answer, "arr[n - 1];") == 0) {
        printf(COLOR_GREEN "✓ Correct (+15)\n" COLOR_RESET);
        score += 15;
    } else {
        printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
        printf("Expected: arr[n-1];\n");
        printf(COLOR_YELLOW "💡 Last index is (n-1) because arrays start at 0.\n" COLOR_RESET);
    }

    printf("\nAdditional exercise finished. You earned %d points.\n", score);
    return score;
}

void play_level4(Player *player) {
    int level_score = 0;
    char buffer[64];

    printf("\n=== Level 4: Arrays ===\n");

    /* 1) Teaching */
    printf("\n--- Teaching ---\n");
    printf("Arrays store multiple values of the same type.\n");
    printf("- Index starts at 0\n");
    printf("- Example: int arr[3] = {1, 2, 3};\n");
    printf("  arr[0] = 1, arr[1] = 2, arr[2] = 3\n");
    printf("\nPress Enter to continue...");
    read_line(buffer, sizeof(buffer));

    printf("\nReady? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Returning to main menu.\n");
        return;
    }

    /* 2) Step-by-step (like L2) */
    printf("\n--- Step-by-step Coding Practice ---\n");
    printf("Goal: Calculate the sum of elements in an integer array.\n");
    printf("Scoring: 5 points per correct step\n");
    printf("(Retry allowed up to %d times per step)\n", MAX_RETRY);

    level_score += ask_code_step_retry(
        "Step 1: Declare an integer array named 'arr' with values 1, 2, and 3.",
        "int arr[3] = {1, 2, 3};",
        "Array init: int arr[3] = {1, 2, 3}; (brackets + braces + semicolon)"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 2: Declare an integer variable named 'sum' and initialize it to 0.",
        "int sum = 0;",
        "Initialize: int sum = 0; (needs semicolon)"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 3: Write a for loop that iterates from i = 0 while i < 3 (include '{').",
        "for (int i = 0; i < 3; i++) {",
        "Loop header: for (int i = 0; i < 3; i++) {"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 4: Inside the loop, add the current array element to sum.",
        "sum += arr[i];",
        "Use sum += arr[i]; (index with i)"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 5: Close the for loop block.",
        "}",
        "Just }"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 6: Print the sum using printf with format \"Sum = %d\\n\".",
        "printf(\"Sum = %d\\n\", sum);",
        "printf(\"Sum = %d\\n\", sum); (quotes + \\n + semicolon)"
    ) ? 5 : 0;

    printf("\nYou have finished the guided coding steps. The full code would be:\n");
    printf("```c\nint arr[3] = {1, 2, 3};\nint sum = 0;\nfor (int i = 0; i < 3; i++) {\n    sum += arr[i];\n}\nprintf(\"Sum = %%d\\n\", sum);\n```\n");

    /* 3) Mini exercise */
    printf("\nNow we will move on to the mini exercise with test cases.\n");
    level_score += run_mini_exercise_L4();

    /* 4) Additional exercise */
    printf("\nGreat! Now let's try some more challenging questions.\n");
    level_score += run_additional_exercise_L4();

    printf("\nLevel 4 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update your score.\n");
        }
    } else {
        printf("No points earned this time. Review array concepts and try again!\n");
    }
}

/* ---------------------------
   LEVEL 5: switch case (UPDATED to match Level 2 structure)
   --------------------------- */

static void explain_mistake_L5(int test_case) {
    printf(COLOR_YELLOW "\n💡 Explanation: " COLOR_RESET);

    switch(test_case) {
        case 1:
            printf("The 'default' case runs when no 'case' matches.\n");
            break;
        case 2:
            printf("'break' stops execution from falling through to the next case.\n");
            break;
        case 3:
            printf("Character literals use single quotes: case 'A':\n");
            break;
        case 4:
            printf("A switch starts like: switch (variable) {\n");
            printf("Don’t forget the opening brace {.\n");
            break;
    }
    printf("\n");
}

/* Mini exercise (retry) */
static int run_mini_exercise_L5(void) {
    char answer[128];
    int passed = 0;
    int attempt;

    printf("\n" COLOR_CYAN "=== Mini Exercise: switch case Concepts ===" COLOR_RESET "\n");
    printf("Scoring: 5 points per correct answer\n");
    printf("(You can retry up to %d times per question)\n", MAX_RETRY);

    printf("\nTest Case 1:\n");
    printf("Question: If no case matches the value in the switch statement, which block is executed? (keyword)\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);
        if (strcmp(answer, "default") == 0) {
            printf(COLOR_GREEN "✓ Test Case 1 passed. (+5)\n" COLOR_RESET);
            passed += 1;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " It's the 'default' block.\n");
            explain_mistake_L5(1);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " default\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    printf("\nTest Case 2:\n");
    printf("Question: Which statement prevents fall-through to the next case? (keyword)\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);
        if (strcmp(answer, "break") == 0) {
            printf(COLOR_GREEN "✓ Test Case 2 passed. (+5)\n" COLOR_RESET);
            passed += 1;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " It stops the switch case: break\n");
            explain_mistake_L5(2);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " break\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    printf("\nTest Case 3:\n");
    printf("Question: Write the correct C code line to start a case for the value 'A' (include ':').\n");
    for (attempt = 1; attempt <= MAX_RETRY; attempt++) {
        printf("Your answer: ");
        read_line(answer, sizeof(answer));
        trim_spaces(answer);
        if (strcmp(answer, "case 'A':") == 0) {
            printf(COLOR_GREEN "✓ Test Case 3 passed. (+5)\n" COLOR_RESET);
            passed += 1;
            break;
        } else {
            printf(COLOR_RED "✗ Wrong.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Hint:" COLOR_RESET " Use single quotes: case 'A':\n");
            explain_mistake_L5(3);
            if (attempt == MAX_RETRY) {
                printf(COLOR_YELLOW "Answer revealed:" COLOR_RESET " case 'A':\n");
            } else {
                printf("Try again (%d/%d)\n", attempt, MAX_RETRY);
            }
        }
    }

    printf("\nMini exercise finished. Passed %d / 3.\n", passed);
    return passed * 5;
}

/* Additional exercise (E/M/H) */
static int run_additional_exercise_L5(void) {
    char answer[256];
    int score = 0;

    printf("\n" COLOR_CYAN "=== Additional Exercise: switch case (Easy / Medium / Hard) ===" COLOR_RESET "\n");
    printf("Scoring: EASY=5, MEDIUM=10, HARD=15\n");

    /* EASY */
    printf("\n" COLOR_CYAN "[EASY]" COLOR_RESET " Question 1:\n");
    printf("Question: Write the case label for integer 5 (include ':').\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "case 5:") == 0) {
        printf(COLOR_GREEN "✓ Correct (+5)\n" COLOR_RESET);
        score += 5;
    } else {
        printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
        printf("Expected: case 5:\n");
    }

    /* MEDIUM */
    printf("\n" COLOR_CYAN "[MEDIUM]" COLOR_RESET " Question 2:\n");
    printf("Question: Write ONE line that groups 'y' and 'Y' to print \"Yes\\n\" then break.\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "case 'y': case 'Y': printf(\"Yes\\n\"); break;") == 0) {
        printf(COLOR_GREEN "✓ Correct (+10)\n" COLOR_RESET);
        score += 10;
    } else {
        printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
        printf("Expected: case 'y': case 'Y': printf(\"Yes\\n\"); break;\n");
    }

    /* HARD */
    printf("\n" COLOR_CYAN "[HARD]" COLOR_RESET " Question 3:\n");
    printf("Question: Write ONE line that groups case 6 and case 7 to print \"Weekend\\n\" then break, and also includes default to print \"Error\\n\" then break.\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "case 6: case 7: printf(\"Weekend\\n\"); break; default: printf(\"Error\\n\"); break;") == 0) {
        printf(COLOR_GREEN "✓ Correct (+15)\n" COLOR_RESET);
        score += 15;
    } else {
        printf(COLOR_RED "✗ Incorrect.\n" COLOR_RESET);
        printf("Expected:\n");
        printf("case 6: case 7: printf(\"Weekend\\n\"); break; default: printf(\"Error\\n\"); break;\n");
    }

    printf("\nAdditional exercise finished. You earned %d points.\n", score);
    return score;
}

void play_level5(Player *player) {
    int level_score = 0;
    char buffer[64];

    printf("\n=== Level 5: The 'switch case' Statement ===\n");

    /* 1) Teaching */
    printf("\n--- Teaching ---\n");
    printf("switch is used when you want to compare ONE value against many options.\n");
    printf("Structure:\n");
    printf("switch (value) {\n");
    printf("  case 1: ... break;\n");
    printf("  case 2: ... break;\n");
    printf("  default: ... break;\n");
    printf("}\n");
    printf("\nPress Enter to continue...");
    read_line(buffer, sizeof(buffer));

    printf("\nReady? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Returning to main menu.\n");
        return;
    }

    /* 2) Step-by-step practice (like L2; NOT E/M/H) */
    printf("\n--- Step-by-step Coding Practice ---\n");
    printf("Goal: Build a switch on 'choice':\n");
    printf("- case 1 prints \"Play\\n\"\n");
    printf("- case 2 prints \"Exit\\n\"\n");
    printf("- default prints \"Invalid\\n\"\n");
    printf("Scoring: 5 points per correct step\n");
    printf("(Retry allowed up to %d times per step)\n", MAX_RETRY);

    level_score += ask_code_step_retry(
        "Step 1: Start the switch statement using variable choice (include '{').",
        "switch (choice) {",
        "Use: switch (choice) {"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 2: Start case 1 (include ':').",
        "case 1:",
        "Case label pattern: case 1:"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 3: In case 1, print \"Play\\n\" then break (ONE line).",
        "printf(\"Play\\n\"); break;",
        "Use printf(\"Play\\n\"); break;"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 4: Write case 2 that prints \"Exit\\n\" then break (ONE line).",
        "case 2: printf(\"Exit\\n\"); break;",
        "Pattern: case 2: printf(\"Exit\\n\"); break;"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 5: Write default that prints \"Invalid\\n\" then break (ONE line).",
        "default: printf(\"Invalid\\n\"); break;",
        "Pattern: default: printf(\"Invalid\\n\"); break;"
    ) ? 5 : 0;

    level_score += ask_code_step_retry(
        "Step 6: Close the switch block.",
        "}",
        "Just }"
    ) ? 5 : 0;

    printf("\nYou have finished the guided coding steps. The full code would be:\n");
    printf("```c\nswitch (choice) {\n");
    printf("    case 1:\n");
    printf("        printf(\"Play\\n\");\n");
    printf("        break;\n");
    printf("    case 2:\n");
    printf("        printf(\"Exit\\n\");\n");
    printf("        break;\n");
    printf("    default:\n");
    printf("        printf(\"Invalid\\n\");\n");
    printf("        break;\n");
    printf("}\n```\n");

    /* 3) Mini exercise */
    printf("\nNow we will move on to the mini exercise with test cases.\n");
    level_score += run_mini_exercise_L5();

    /* 4) Additional exercise */
    printf("\nGreat! Now let's try some more challenging questions.\n");
    level_score += run_additional_exercise_L5();

    printf("\nLevel 5 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update your score in the player database.\n");
        }
    } else {
        printf("No points earned this time. Review the 'switch case' concepts and try again!\n");
    }
}

/* ===========================
   main.c (merged)
   =========================== */

static void show_welcome_screen(void) {
    printf("========================================\n");
    printf("              C Tutor Lab\n");
    printf("          CPE100 Term Project\n");
    printf("----------------------------------------\n");
    printf("                MEMBERS\n");
    printf(" 68070503415  Chadathong  Tangjitpakorn\n");
    printf(" 68070503422  Nichapa     Piromying\n");
    printf(" 68070503427  Thanpisit   Ritpetchnil\n");
    printf(" 68070503445  Peerawat    Boonyaprasit\n");
    printf(" 68070503457  Supakorn    Polphichai\n");
    printf("========================================\n\n");
}

static void show_auth_menu(void) {
    printf("=== Player Menu ===\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("0. Exit\n");
    printf("-------------------\n");
}

static void show_game_menu(const Player *player) {
    printf("\n=== Main Game Menu ===\n");
    printf("Current player: %s (Score: %d)\n", player->name, player->score);
    printf("1. Play Level 1\n");
    printf("2. Play Level 2\n");
    printf("3. Play Level 3\n");
    printf("4. Play Level 4\n");
    printf("5. Play Level 5\n");
    printf("6. Show player info\n");
    printf("0. Logout and exit\n");
    printf("-------------------------\n");
}

int main(void) {
    Player currentPlayer;
    int logged_in = 0;
    int choice;
    int ch;

    show_welcome_screen();

    while (!logged_in) {
        show_auth_menu();
        printf("Select: ");

        if (scanf("%d", &choice) != 1) {
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("Please enter a number (0-2).\n\n");
            continue;
        }
        while ((ch = getchar()) != '\n' && ch != EOF) { }

        if (choice == 1) {
            if (register_player(&currentPlayer)) {
                logged_in = 1;
            }
        } else if (choice == 2) {
            if (login_player(&currentPlayer)) {
                logged_in = 1;
            }
        } else if (choice == 0) {
            printf("Goodbye!\n");
            return 0;
        } else {
            printf("Invalid choice. Please select 0, 1, or 2.\n\n");
        }
    }

    while (1) {
        show_game_menu(&currentPlayer);
        printf("Select: ");

        if (scanf("%d", &choice) != 1) {
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("Please enter a number (0-6).\n");
            continue;
        }
        while ((ch = getchar()) != '\n' && ch != EOF) { }

        switch (choice) {
            case 1: play_level1(&currentPlayer); break;
            case 2: play_level2(&currentPlayer); break;
            case 3: play_level3(&currentPlayer); break;
            case 4: play_level4(&currentPlayer); break;
            case 5: play_level5(&currentPlayer); break;
            case 6: print_player_info(&currentPlayer); break;
            case 0:
                printf("Logging out... Goodbye, %s!\n", currentPlayer.name);
                return 0;
            default:
                printf("Invalid choice. Please select 0-6.\n");
                break;
        }
    }

    return 0;
}