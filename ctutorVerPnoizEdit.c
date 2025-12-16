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
   player.c (merged)
   =========================== */

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
        return 0;
    }

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
        return 0;
    }

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
   level1.c (merged)  ✅ FIXED: teaching -> mini(syntax) -> 3 Qs (E/M/H) + explanations
   =========================== */

#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED   "\x1b[31m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"

static void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
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

/* strict step checker (เหมือนเดิม) */
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

/* ---------- L1 Explanation helpers (ใหม่) ---------- */
static void explain_mistake_L1(const char *expected, int test_case) {
    (void)expected;

    printf(COLOR_YELLOW "\n💡 Explanation: " COLOR_RESET);

    switch (test_case) {
        case 1: /* %d */
            printf("%%d is the format specifier for an integer (int).\n");
            printf("- It always starts with '%%'\n");
            printf("- 'd' means decimal integer\n");
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

/* ---------- L1 Mini Exercise (syntax check) (แก้ใหม่ + มีอธิบาย) ---------- */
static int run_mini_exercise_L1(void) {
    char answer[256];
    int test_score = 0;

    printf("\n" COLOR_CYAN "=== Mini Exercise (Syntax): Format Specifiers & printf ===" COLOR_RESET "\n");
    printf("How to answer:\n");
    printf("- Type the EXACT token/line requested (include %% and ; when asked)\n");
    printf("- Example format: %%d   or   printf(\"%%d\\n\", 42);\n");

    /* Test Case 1 */
    printf("\nTest Case 1:\n");
    printf("Question: What is the format specifier for an integer (int)?\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "%d") == 0) {
        printf(COLOR_GREEN "✓ Test Case 1 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "✗ Test Case 1 failed. Expected: %%d" COLOR_RESET "\n");
        explain_mistake_L1("%d", 1);
    }

    /* Test Case 2 */
    printf("\nTest Case 2:\n");
    printf("Question: What is the format specifier for a string (char array)?\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "%s") == 0) {
        printf(COLOR_GREEN "✓ Test Case 2 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "✗ Test Case 2 failed. Expected: %%s" COLOR_RESET "\n");
        explain_mistake_L1("%s", 2);
    }

    /* Test Case 3 */
    printf("\nTest Case 3:\n");
    printf("Question: Write a complete printf statement that prints the integer 42 with a newline.\n");
    printf("Requirement: Use %%d and include the semicolon ;\n");
    printf("One valid answer: printf(\"%%d\\n\", 42);\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "printf(\"%d\\n\", 42);") == 0) {
        printf(COLOR_GREEN "✓ Test Case 3 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "✗ Test Case 3 failed." COLOR_RESET "\n");
        printf("Expected (one valid answer): printf(\"%%d\\n\", 42);\n");
        explain_mistake_L1("printf(\"%d\\n\", 42);", 3);
    }

    printf("\nMini exercise finished. You passed %d / 3 test cases.\n", test_score);
    return test_score * 3; /* 0, 3, 6, 9 points */
}

/* ---------- L1 Additional Exercise: 3 Questions (E/M/H) (ใหม่ + มีอธิบาย) ---------- */
static int run_additional_exercise_L1(void) {
    char answer[256];
    int score = 0;

    printf("\n" COLOR_CYAN "=== Additional Exercise: Easy / Medium / Hard ===" COLOR_RESET "\n");
    printf("Answer rules: type the exact code/token requested.\n");

    /* EASY */
    printf("\n" COLOR_CYAN "[EASY]" COLOR_RESET " Question 1:\n");
    printf("Question: What is the printf format specifier for a floating-point number?\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "%f") == 0) {
        printf(COLOR_GREEN "✓ Correct (+3 points)" COLOR_RESET "\n");
        score += 3;
    } else {
        printf(COLOR_RED "✗ Incorrect. Expected: %%f" COLOR_RESET "\n");
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
        printf(COLOR_GREEN "✓ Correct (+5 points)" COLOR_RESET "\n");
        score += 5;
    } else {
        printf(COLOR_RED "✗ Incorrect." COLOR_RESET "\n");
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
        printf(COLOR_GREEN "✓ Correct (+7 points)" COLOR_RESET "\n");
        score += 7;
    } else {
        printf(COLOR_RED "✗ Incorrect." COLOR_RESET "\n");
        printf("Expected: scanf(\"%%d\", &age);\n");
        explain_mistake_L1("scanf(\"%d\", &age);", 4);
    }

    printf("\nAdditional exercise finished. You earned %d points.\n", score);
    return score;
}

/* ---------- Level 1 main (ปรับ flow ตาม Level 2) ---------- */
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

    /* 2) Step-by-step practice (เหมือนเดิม) */
    printf("\n--- Step-by-step Coding Practice ---\n");
    printf("You must type each line exactly as requested.\n");

    level_score += ask_code_step(
        "Step 1: Declare an integer variable named age.",
        "int age;"
    ) ? 2 : 0;

    level_score += ask_code_step(
        "Step 2: Declare a char array named name that can hold 50 characters.",
        "char name[50];"
    ) ? 2 : 0;

    level_score += ask_code_step(
        "Step 3: Write a printf statement that prints the age using %d.\n"
        "Expected output: Age: 20 (assume age variable will be used).",
        "printf(\"Age: %d\\n\", age);"
    ) ? 3 : 0;

    level_score += ask_code_step(
        "Step 4: Write a printf statement that prints the name using %s.\n"
        "Expected output pattern: Name: <name>",
        "printf(\"Name: %s\\n\", name);"
    ) ? 3 : 0;

    printf("\nYou finished the guided steps.\n");

    /* 3) Mini exercise (syntax) + explanations (ใหม่) */
    level_score += run_mini_exercise_L1();

    /* 4) 3 Questions (Easy/Medium/Hard) + explanations (ใหม่) */
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
   level2-5.c (merged) : DEV AREA (เต็มไฟล์แล้ว)
   ===========================

   NOTE FOR TEAM (ใครทำเลเวล 2-5 อ่านก่อน!)
   -------------------------------------------------
   1) ห้ามเปลี่ยน signature ของฟังก์ชัน:
        void play_levelX(Player *player);

   2) การให้คะแนน:
        - เก็บคะแนนเลเวลไว้ในตัวแปร int level_score = 0;
        - เมื่อจบเลเวล ให้เรียก update_player_score(player, level_score);
        - อย่าแก้ player->score ตรง ๆ เอง

   3) การรับ input:
        - แนะนำให้ใช้ read_line() แทน scanf()
          เพราะ scanf มักติดปัญหา newline/เว้นวรรค
        - ถ้าต้องแปลงเป็นตัวเลข ให้ใช้ sscanf หรือ strtol (ถ้าอยากชัวร์)

   4) ฟังก์ชัน helper:
        - ถ้าจะสร้าง helper เพิ่ม ให้ใส่ static และตั้งชื่อขึ้นต้นด้วย level
          เช่น static int level2_check_answer(...) เพื่อไม่ชนชื่อกัน

   5) รูปแบบเลเวล (แนะนำ):
        - แสดงหัวข้อ/คำอธิบายสั้น ๆ
        - มี 2 ส่วน: (A) Guided Steps / (B) Mini Exercise
        - สรุปคะแนน + อัปเดตคะแนนในไฟล์ players.txt

   6) โทนเดียวกับ Level 1:
        - ใช้ COLOR_GREEN / COLOR_RED / COLOR_RESET ได้เลย
*/

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
   LEVEL 2 TEMPLATE
   --------------------------- */
static void explain_mistake_L2(const char *user_answer, const char *expected, int test_case) {
    printf(COLOR_YELLOW "\n💡 Explanation: " COLOR_RESET);

    switch(test_case) {
        case 1: // Initialization
            printf("The correct syntax is: 'int i = 0;'\n");
            printf("- 'int' declares the variable type\n");
            printf("- 'i' is the variable name\n");
            printf("- '= 0' initializes it to 0\n");
            printf("- ';' ends the statement\n");
            break;

        case 2: // Condition
            printf("The condition 'i < 5;' means:\n");
            printf("- Continue the loop while i is less than 5\n");
            printf("- This will run for i = 0, 1, 2, 3, 4 (5 times total)\n");
            printf("- Don't forget the semicolon!\n");
            break;

        case 3: // Increment
            printf("The increment operator 'i++' means:\n");
            printf("- Add 1 to the value of i after each loop iteration\n");
            printf("- It's shorthand for 'i = i + 1'\n");
            break;
    }
    printf("\n");
}

static int run_mini_exercise_L2(void) {
    char answer[128];
    int test_score = 0;

    printf("\n" COLOR_CYAN "=== Mini Exercise: For Loop Concepts ===" COLOR_RESET "\n");

    /* Test case 1: The correct syntax for initialization */
    printf("\nTest Case 1:\n");
    printf("Question: Write the correct C code line to initialize an integer variable 'i' to 0 for a loop (include the semicolon ';').\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "int i = 0;") == 0) {
        printf(COLOR_GREEN "✓ Test Case 1 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "✗ Test Case 1 failed. Expected: int i = 0;" COLOR_RESET "\n");
        explain_mistake_L2(answer, "int i = 0;", 1);
    }

    /* Test case 2: The correct syntax for the loop condition */
    printf("\nTest Case 2:\n");
    printf("Question: Write the condition that checks if 'i' is less than 5 (include the semicolon ';').\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "i < 5;") == 0) {
        printf(COLOR_GREEN "✓ Test Case 2 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "✗ Test Case 2 failed. Expected: i < 5;" COLOR_RESET "\n");
        explain_mistake_L2(answer, "i < 5;", 2);
    }

    /* Test case 3: Writing the correct increment statement */
    printf("\nTest Case 3:\n");
    printf("Question: Write the statement to increment the variable 'i' by 1 using the shorthand post-increment operator.\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "i++") == 0 || strcmp(answer, "i=i+1") == 0) {
        printf(COLOR_GREEN "✓ Test Case 3 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "✗ Test Case 3 failed. Expected: i++" COLOR_RESET "\n");
        explain_mistake_L2(answer, "i++", 3);
    }

    printf("\nMini exercise finished. You passed %d / 3 test cases.\n", test_score);
    return test_score * 3;  /* 0, 3, 6, or 9 points */
}

static int run_additional_exercise_L2(void) {
    char answer[128];
    int test_score = 0;

    printf("\n" COLOR_CYAN "=== Additional Exercise: Advanced For Loop Questions ===" COLOR_RESET "\n");
    printf("This section has 3 questions with increasing difficulty.\n");

    /* EASY Question */
    printf("\n" COLOR_CYAN "[EASY]" COLOR_RESET " Question 1:\n");
    printf("Question: Write a for loop initialization to start counting from 1 (not 0)(include the semicolon ';').\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "int i = 1;") == 0) {
        printf(COLOR_GREEN "✓ Question 1 passed. (+3 points)" COLOR_RESET "\n");
        test_score += 3;
    } else {
        printf(COLOR_RED "✗ Question 1 failed. Expected: int i = 1;" COLOR_RESET "\n");
        printf(COLOR_YELLOW "💡 To start from 1, initialize i with value 1 instead of 0.\n" COLOR_RESET);
    }

    /* MEDIUM Question */
    printf("\n" COLOR_CYAN "[MEDIUM]" COLOR_RESET " Question 2:\n");
    printf("Question: Write a for loop that counts DOWN from 10 to 1.\n");
    printf("Write the complete for loop header (without the body).\n");
    printf("Example format: for (initialization; condition; iteration)\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "for (int i = 10; i >= 1; i--)") == 0 || strcmp(answer, "for (int i = 10; i > 0; i--)") == 0) {
        printf(COLOR_GREEN "✓ Question 2 passed. (+5 points)" COLOR_RESET "\n");
        test_score += 5;
    } else {
        printf(COLOR_RED "✗ Question 2 failed. Expected: for (int i = 10; i >= 1; i--)" COLOR_RESET "\n");
        printf(COLOR_YELLOW "💡 To count down:\n");
        printf("   - Start from 10: int i = 10\n");
        printf("   - Continue while i >= 1 (to include 1)\n");
        printf("   - Decrement: i--\n" COLOR_RESET);
    }

    /* HARD Question */
    printf("\n" COLOR_CYAN "[HARD]" COLOR_RESET " Question 3:\n");
    printf("Question: Write a for loop that prints only EVEN numbers from 0 to 10 (inclusive).\n");
    printf("Write the complete for loop header (without the body).\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "for (int i = 0; i <= 10; i += 2)") == 0 ||
        strcmp(answer, "for (int i = 0; i <= 10; i=i+2)") == 0) {
        printf(COLOR_GREEN "✓ Question 3 passed. (+7 points)" COLOR_RESET "\n");
        test_score += 7;
    } else {
        printf(COLOR_RED "✗ Question 3 failed. Expected: for (int i = 0; i <= 10; i += 2)" COLOR_RESET "\n");
        printf(COLOR_YELLOW "💡 To print even numbers (0, 2, 4, 6, 8, 10):\n");
        printf("   - Start at 0 (first even number)\n");
        printf("   - Continue while i <= 10 (to include 10)\n");
        printf("   - Increment by 2: i += 2 (skips odd numbers)\n" COLOR_RESET);
    }

    printf("\nAdditional exercise finished. You earned %d points.\n", test_score);
    return test_score;
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

    level_score += ask_code_step(
        "Step 1: Start the 'for' loop. Initialize an integer variable 'i' to 0 (including the opening parenthesis and first semicolon).",
        "for (int i = 0;"
    ) ? 3 : 0;

    level_score += ask_code_step(
        "Step 2: Add the loop condition so it continues as long as 'i' is less than 5 (include the second semicolon).",
        "i < 5;"
    ) ? 3 : 0;

    level_score += ask_code_step(
        "Step 3: Add the iteration statement to increment 'i' by 1, and open the loop block with '{'.",
        "i++) {"
    ) ? 3 : 0;

    level_score += ask_code_step(
        "Step 4: Inside the loop, print the current value of 'i' followed by a newline.",
        "printf(\"%d\\n\", i);"
    ) ? 3 : 0;

    level_score += ask_code_step(
        "Step 5: Close the 'for' loop block.",
        "}"
    ) ? 2 : 0;

    printf("\nYou have finished the guided coding steps. The full code would be:\n");
    printf("```c\nfor (int i = 0; i < 5; i++) {\n printf(\"%%d\\n\", i);\n}\n```\n");

    printf("\nNow we will move on to the basic exercise with test cases.\n");

    /* Run the original mini exercise (3 basic questions) */
    level_score += run_mini_exercise_L2();

    printf("\nGreat! Now let's try some more challenging questions.\n");

    /* Run the additional exercise (1 easy, 1 medium, 1 hard) */
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
   LEVEL 3 TEMPLATE
   --------------------------- */
/* ===========================
   LEVEL 3: if-else Statement
   =========================== */

/* ใช้เฉพาะใน level 3 */
static void level3_remove_spaces(char *s) {
    char *src = s;
    char *dst = s;
    while (*src) {
        if (*src != ' ' && *src != '\t') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

/* ---------- Mini Exercise (syntax check) ---------- */
static int run_mini_exercise_L3(void) {
    char answer[256];
    int score = 0;

    printf("\n=== Mini Exercise: if–else Syntax ===\n");

    /* EASY */
    printf("\n[EASY] Write an if statement that checks if x is greater than 0.\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    level3_remove_spaces(answer);

    if (strcmp(answer, "if(x>0){") == 0) {
        printf(COLOR_GREEN "✓ Correct\n" COLOR_RESET);
        score += 2;
    } else {
        printf(COLOR_RED "✗ Incorrect\n" COLOR_RESET);
        printf("Expected example: if (x > 0) {\n");
    }

    /* MEDIUM */
    printf("\n[MEDIUM] Write an if–else that prints \"Safe\" if wind < 10, otherwise \"Danger\".\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    level3_remove_spaces(answer);

    if (strcmp(answer,
        "if(wind<10){printf(\"Safe\");}else{printf(\"Danger\");}") == 0) {
        printf(COLOR_GREEN "✓ Correct\n" COLOR_RESET);
        score += 4;
    } else {
        printf(COLOR_RED "✗ Incorrect\n" COLOR_RESET);
        printf("One valid answer:\n");
        printf("if (wind < 10) { printf(\"Safe\"); } else { printf(\"Danger\"); }\n");
    }

    /* HARD */
    printf("\n[HARD] Write if–else if–else for hunger:\n");
    printf("0–3 -> Eat 1 bowl, 4–6 -> Eat 2 bowls, else -> Eat 3 bowls\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    level3_remove_spaces(answer);

    if (strcmp(answer,
        "if(hunger<=3){printf(\"Eat1bowl\");}else if(hunger<=6){printf(\"Eat2bowls\");}else{printf(\"Eat3bowls\");}") == 0) {
        printf(COLOR_GREEN "✓ Correct\n" COLOR_RESET);
        score += 6;
    } else {
        printf(COLOR_RED "✗ Incorrect\n" COLOR_RESET);
        printf("Hint: Use if / else if / else\n");
    }

    printf("\nMini Exercise finished. Score: %d\n", score);
    return score;
}

/* ---------- MAIN LEVEL FUNCTION ---------- */
void play_level3(Player *player) {
    int level_score = 0;
    char buffer[64];

    printf("\n=== Level 3: IF–ELSE Decision Making ===\n");
    printf("Story: You are surviving on a deserted island.\n");
    printf("You must make decisions using if–else statements.\n");

    printf("\nAre you ready? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Returning to menu...\n");
        return;
    }
    printf("\n--- Teaching: if Statement ---\n");
    printf("An if statement is used to make decisions in a program.\n");
    printf("It runs code only when the condition is true.\n");

    printf("\nExample:\n");
    printf("int x = 5;\n");
    printf("if (x > 0) {\n");
    printf("    printf(\"Positive\");\n");
    printf("}\n");

    printf("\nExplanation:\n");
    printf("- if (x > 0) checks the condition\n");
    printf("- Code inside { } runs only if the condition is true\n");

    printf("\nPress Enter to continue to practice...");
    read_line(buffer, sizeof(buffer));

    printf("\n--- Step-by-step Practice ---\n");

    level_score += ask_code_step(
        "Step 1: Write an if statement that checks if x is greater than 0.",
        "if (x > 0) {"
    ) ? 2 : 0;

    level_score += ask_code_step(
        "Step 2: Inside the if block, print \"Positive\".",
        "printf(\"Positive\");"
    ) ? 2 : 0;

    level_score += ask_code_step(
        "Step 3: Close the if block.",
        "}"
    ) ? 1 : 0;

    printf("\nNow try the exercises.\n");

    level_score += run_mini_exercise_L3();

    printf("\nLevel 3 completed.\n");
    printf("You earned %d points.\n", level_score);

    if (level_score > 0) {
        update_player_score(player, level_score);
        printf("Total score: %d\n", player->score);
    }
}

/* ---------------------------
   LEVEL 4: Arrays
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

static int run_mini_exercise_L4(void) {
    char answer[128];
    int test_score = 0;

    printf("\n" COLOR_CYAN "=== Mini Exercise: Array Concepts ===" COLOR_RESET "\n");

    printf("\nTest Case 1:\n");
    printf("Question: Given int a[3] = {4, 5, 6}; what is the value of a[0]?\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "4") == 0) {
        printf(COLOR_GREEN "✓ Test Case 1 passed.\n" COLOR_RESET);
        test_score++;
    } else {
        printf(COLOR_RED "✗ Test Case 1 failed. Expected: 4\n" COLOR_RESET);
        explain_mistake_L4("4", 1);
    }

    printf("\nTest Case 2:\n");
    printf("Question: What index represents the third element of an array?\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "2") == 0) {
        printf(COLOR_GREEN "✓ Test Case 2 passed.\n" COLOR_RESET);
        test_score++;
    } else {
        printf(COLOR_RED "✗ Test Case 2 failed. Expected: 2\n" COLOR_RESET);
        explain_mistake_L4("2", 2);
    }

    printf("\nTest Case 3:\n");
    printf("Question: Write the correct C expression to access the third element of array 'arr' (include semicolon).\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);

    if (strcmp(answer, "arr[2];") == 0) {
        printf(COLOR_GREEN "✓ Test Case 3 passed.\n" COLOR_RESET);
        test_score++;
    } else {
        printf(COLOR_RED "✗ Test Case 3 failed. Expected: arr[2];\n" COLOR_RESET);
        explain_mistake_L4("arr[2];", 3);
    }

    printf("\nMini exercise finished. Passed %d / 3 cases.\n", test_score);
    return test_score * 3;
}

void play_level4(Player *player) {
    int level_score = 0;
    char buffer[64];

    printf("\n=== Level 4: Arrays ===\n");
    printf("Description: Learn how to store multiple values using arrays,\n");
    printf("access elements by index, and process data using loops.\n");

    printf("\nReady? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Returning to main menu.\n");
        return;
    }

    printf("\n--- Step-by-step Coding Practice ---\n");
    printf("Goal: Calculate the sum of elements in an integer array.\n");

    level_score += ask_code_step(
        "Step 1 (2 pts): Declare an integer array named 'arr' with values 1, 2, and 3.",
        "int arr[3] = {1, 2, 3};"
    ) ? 2 : 0;

    level_score += ask_code_step(
        "Step 2 (2 pts): Declare an integer variable named 'sum' and initialize it to 0.",
        "int sum = 0;"
    ) ? 2 : 0;

    level_score += ask_code_step(
        "Step 3 (3 pts): Write a for loop that iterates from i = 0 while i < 3 (include '{').",
        "for (int i = 0; i < 3; i++) {"
    ) ? 3 : 0;

    level_score += ask_code_step(
        "Step 4 (3 pts): Inside the loop, add the current array element to sum.",
        "sum += arr[i];"
    ) ? 3 : 0;

    level_score += ask_code_step(
        "Step 5 (2 pts): Close the for loop block.",
        "}"
    ) ? 2 : 0;

    level_score += ask_code_step(
        "Step 6 (3 pts): Print the sum using printf with format \"Sum = %d\\n\".",
        "printf(\"Sum = %d\\n\", sum);"
    ) ? 3 : 0;

    printf("\nYou have finished the guided coding steps.\n");
    printf("Now we will check your understanding with a mini exercise.\n");

    level_score += run_mini_exercise_L4();

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

/* =======================================
   Level 5: switch case (Helper Functions)
   ======================================= */

static int ask_code_step_with_explanation(const char *instruction,
                                           const char *expected,
                                           const char *explanation) {
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
            printf(COLOR_RED "Reasoning:" COLOR_RESET " %s\n", explanation);
            printf("Expected pattern: %s\n", expected);
        }
    }
}

static int run_mini_exercise_L5(void) {
    char answer[128];
    int test_score = 0;

    printf("\n" COLOR_CYAN "=== Mini Exercise: switch case Concepts ===" COLOR_RESET "\n");

    printf("\nTest Case 1:\n");
    printf("Question: If no case matches the value in the switch statement, which command block is executed? (Answer with the specific C keyword)\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "default") == 0) {
        printf(COLOR_GREEN "✓ Test Case 1 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "✗ Test Case 1 failed. Expected: default" COLOR_RESET "\n");
    }

    printf("\nTest Case 2:\n");
    printf("Question: Which statement must be placed at the end of each case block (unless fall-through is desired) to prevent the program from executing the next case? (Answer with the specific C keyword)\n");
    printf("Answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "break") == 0) {
        printf(COLOR_GREEN "✓ Test Case 2 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "✗ Test Case 2 failed. Expected: break" COLOR_RESET "\n");
    }

    printf("\nTest Case 3:\n");
    printf("Question: Write the correct C code line to start a case for the value 'A' (a character) in a switch statement (include the colon ':').\n");
    printf("Your answer: ");
    read_line(answer, sizeof(answer));
    trim_spaces(answer);
    if (strcmp(answer, "case 'A':") == 0) {
        printf(COLOR_GREEN "✓ Test Case 3 passed." COLOR_RESET "\n");
        test_score += 1;
    } else {
        printf(COLOR_RED "✗ Test Case 3 failed. Expected: case 'A':" COLOR_RESET "\n");
    }

    printf("\nMini exercise finished. You passed %d / 3 test cases.\n", test_score);
    return test_score * 3;
}

void play_level5(Player *player) {
    int level_score = 0;
    char buffer[64];

    printf("\n=== Level 5: The 'switch case' Statement ===\n");
    printf("Description: Practice using the 'switch' control structure, including 'case', 'break', 'default', and fall-through logic.\n");

    printf("\nReady? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Returning to main menu.\n");
        return;
    }

    printf("\n--- Step-by-step Coding Practice (Easy/Medium/Hard) ---\n");

    level_score += ask_code_step_with_explanation(
        "Easy (2 points): Declare an integer variable named 'day' and start a switch statement using it.",
        "switch (day) {",
        "You must declare the switch statement using the 'switch' keyword, followed by the variable in parentheses, and an opening brace '{' to start the block."
    ) ? 2 : 0;

    level_score += ask_code_step_with_explanation(
        "Medium (3 points): Inside the switch, create a case for the value 1. Print the string 'Monday' and ensure the flow stops after this case.",
        "case 1: printf(\"Monday\\n\"); break;",
        "Every case must end with a colon ':' and include a 'break;' statement to prevent 'fall-through' into the next case's code. Remember to use 'printf' correctly."
    ) ? 3 : 0;

    level_score += ask_code_step_with_explanation(
        "Hard (5 points): Write the code for case 6 and case 7 combined to print 'Weekend' (using fall-through), followed by the default case to print 'Error'.",
        "case 6: case 7: printf(\"Weekend\\n\"); break; default: printf(\"Error\\n\"); break;",
        "To combine multiple cases, you stack the 'case' labels without a 'break;'. The 'default' case handles all non-matching values, and it's good practice to include a final 'break;' for clarity, although not strictly necessary at the very end."
    ) ? 5 : 0;

    printf("\nYou have finished the guided coding steps.\n");
    printf("Now we will move on to a small exercise with test cases to check your understanding of switch concepts.\n");

    level_score += run_mini_exercise_L5();

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
