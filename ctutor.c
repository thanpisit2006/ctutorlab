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
   level1.c (merged)
   =========================== */

#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED   "\x1b[31m"
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

static int run_mini_exercise(void) {
    char answer[128];
    int test_score = 0;

    printf("\n=== Mini Exercise: Format Specifiers ===\n");

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

    printf("\nTest Case 3:\n");
    printf("Write a complete printf statement that prints the integer 42\n");
    printf("with a newline. Use %%d as the format specifier.\n");
    printf("One correct answer: printf(\"%%d\\n\", 42);\n");
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
    return test_score * 3; /* 0, 3, 6, or 9 points */
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

    printf("\nAre you ready to start? (y/n): ");
    read_line(buffer, sizeof(buffer));
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf("Okay, returning to the main menu.\n");
        return;
    }

    printf("\n--- Step-by-step Coding Practice ---\n");

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

    printf("\nYou have finished the guided coding steps.\n");
    printf("Now we will move on to a small exercise with test cases.\n");

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
void play_level2(Player *player) {
    int level_score = 0;
    char buf[128];

    printf("\n=== Level 2: [PUT YOUR TOPIC HERE] ===\n");
    printf("Description: [Explain what this level teaches]\n");
    printf("Rules: [How to get points]\n");

    printf("\nReady? (y/n): ");
    read_line(buf, sizeof(buf));
    if (buf[0] != 'y' && buf[0] != 'Y') {
        printf("Returning to main menu.\n");
        return;
    }

    /* TODO (Team Level 2):
       1) Add guided steps OR your own style
       2) Add at least 1 mini exercise with 3 test cases (recommended)
       3) Update level_score accordingly
    */

    /* ตัวอย่างใช้ read_int_in_range (ลบทิ้งได้)
    int choice = read_int_in_range("Choose 1-3: ", 1, 3);
    printf("You chose: %d\n", choice);
    level_score += 1;
    */

    printf("\nLevel 2 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update your score.\n");
        }
    } else {
        printf("No points earned this time.\n");
    }
}

/* ---------------------------
   LEVEL 3 TEMPLATE
   --------------------------- */
void play_level3(Player *player) {
    int level_score = 0;
    char buf[128];

    printf("\n=== Level 3: [PUT YOUR TOPIC HERE] ===\n");
    printf("Description: [Explain what this level teaches]\n");

    printf("\nReady? (y/n): ");
    read_line(buf, sizeof(buf));
    if (buf[0] != 'y' && buf[0] != 'Y') {
        printf("Returning to main menu.\n");
        return;
    }

    /* TODO (Team Level 3): put your code here */

    printf("\nLevel 3 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update your score.\n");
        }
    } else {
        printf("No points earned this time.\n");
    }
}

/* ---------------------------
   LEVEL 4 TEMPLATE
   --------------------------- */
void play_level4(Player *player) {
    int level_score = 0;
    char buf[128];

    printf("\n=== Level 4: [PUT YOUR TOPIC HERE] ===\n");
    printf("Description: [Explain what this level teaches]\n");

    printf("\nReady? (y/n): ");
    read_line(buf, sizeof(buf));
    if (buf[0] != 'y' && buf[0] != 'Y') {
        printf("Returning to main menu.\n");
        return;
    }

    /* TODO (Team Level 4): put your code here */

    printf("\nLevel 4 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update your score.\n");
        }
    } else {
        printf("No points earned this time.\n");
    }
}

/* ---------------------------
   LEVEL 5 TEMPLATE
   --------------------------- */
void play_level5(Player *player) {
    int level_score = 0;
    char buf[128];

    printf("\n=== Level 5: [PUT YOUR TOPIC HERE] ===\n");
    printf("Description: [Explain what this level teaches]\n");

    printf("\nReady? (y/n): ");
    read_line(buf, sizeof(buf));
    if (buf[0] != 'y' && buf[0] != 'Y') {
        printf("Returning to main menu.\n");
        return;
    }

    /* TODO (Team Level 5): put your code here */

    printf("\nLevel 5 completed.\n");
    printf("You earned %d points in this level.\n", level_score);

    if (level_score > 0) {
        if (update_player_score(player, level_score)) {
            printf("Your new total score: %d\n", player->score);
        } else {
            printf("Failed to update your score.\n");
        }
    } else {
        printf("No points earned this time.\n");
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