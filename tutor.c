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

#define MAX_USERS     1000
#define MAX_NAME_LEN  30
#define MAX_PASS_LEN  30
#define USER_FILE     "userdata.txt"

/* player data structure */
struct Player {
    char username[MAX_NAME_LEN + 1];
    char password[MAX_PASS_LEN + 1];
    int  score;
};

/* global data for all players */
struct Player players[MAX_USERS];
int playerCount = 0;
int currentUserIndex = -1;   /* -1 means: no user logged in */

/* function prototypes */
void home(void);
void show_score_bar(void);

int  get_line(char *buffer, int size);
int  line_has_exit(const char *buffer);
int  has_space(const char *buffer);
void to_lowercase(char *s);

int  string_equal(const char *a, const char *b);
void copy_with_limit(char dest[], const char src[], int max_len);

int  register_user(void);
int  login_user(void);

void main_menu(void);

/* lesson functions (separate content areas) */
int  lesson1(void);  /* scanf */
int  lesson2(void);  /* printf */
int  lesson3(void);  /* data types */
int  lesson4(void);  /* loops */
int  lesson5(void);  /* arrays */

/* file functions */
void load_users_from_file(void);
void save_users_to_file(void);

/* ==================== HOME SCREEN ==================== */

/* display home banner */
void home(void) {
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

/* display current user name and score */
void show_score_bar(void) {
    if (currentUserIndex >= 0) {
        printf("[User: %s]  [Score: %d]\n",
               players[currentUserIndex].username,
               players[currentUserIndex].score);
    } else {
        printf("[No user logged in]\n");
    }
    printf("----------------------------------------\n");
}

/* ==================== SIMPLE STRING HELPERS ==================== */

/* read one line from stdin and remove the newline */
int get_line(char *buffer, int size) {
    int i;

    if (fgets(buffer, size, stdin) == NULL) {
        return 0;  /* cannot read */
    }

    /* remove '\n' by hand (no strcspn) */
    i = 0;
    while (buffer[i] != '\0') {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            break;
        }
        i++;
    }

    return 1;
}

/* check if the text contains the word "exit" (case-insensitive) */
int line_has_exit(const char *buffer) {
    char temp[128];
    int i;

    /* copy and convert to lowercase (no library functions) */
    i = 0;
    while (i < 127 && buffer[i] != '\0') {
        char c = buffer[i];
        if (c >= 'A' && c <= 'Z') {
            c = (char)(c - 'A' + 'a');
        }
        temp[i] = c;
        i++;
    }
    temp[i] = '\0';

    /* manual search for "exit" inside temp (no strstr) */
    for (i = 0; temp[i] != '\0'; i++) {
        if (temp[i] == 'e') {
            if (temp[i + 1] == 'x' &&
                temp[i + 2] == 'i' &&
                temp[i + 3] == 't') {
                return 1;
            }
        }
    }
    return 0;
}

/* check if the text has any space character */
int has_space(const char *buffer) {
    int i = 0;
    while (buffer[i] != '\0') {
        if (buffer[i] == ' ') {
            return 1;
        }
        i++;
    }
    return 0;
}

/* convert a string to lowercase (in-place) */
void to_lowercase(char *s) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = (char)(s[i] - 'A' + 'a');
        }
        i++;
    }
}

/* compare two strings for equality (like strcmp == 0) */
int string_equal(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return 0;   /* not equal */
        }
        i++;
    }
    if (a[i] == '\0' && b[i] == '\0') {
        return 1;       /* equal */
    }
    return 0;           /* different length */
}

/* copy src into dest with max length limit (no strncpy) */
void copy_with_limit(char dest[], const char src[], int max_len) {
    int i = 0;
    while (i < max_len && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

/* ==================== FILE FUNCTIONS ==================== */

/* load all users from USER_FILE into players[] */
void load_users_from_file(void) {
    FILE *fp;
    char username[MAX_NAME_LEN + 1];
    char password[MAX_PASS_LEN + 1];
    int score;

    fp = fopen(USER_FILE, "r");
    if (fp == NULL) {
        /* file does not exist yet. this is OK. */
        return;
    }

    playerCount = 0;

    while (playerCount < MAX_USERS &&
           fscanf(fp, "%30s %30s %d", username, password, &score) == 3) {

        /* make sure username is stored in lowercase */
        to_lowercase(username);

        copy_with_limit(players[playerCount].username,
                        username, MAX_NAME_LEN);
        copy_with_limit(players[playerCount].password,
                        password, MAX_PASS_LEN);
        players[playerCount].score = score;

        playerCount++;
    }

    fclose(fp);
}

/* save all users from players[] into USER_FILE */
void save_users_to_file(void) {
    FILE *fp;
    int i;

    fp = fopen(USER_FILE, "w");
    if (fp == NULL) {
        printf("Error: cannot open user file to save.\n");
        return;
    }

    for (i = 0; i < playerCount; i++) {
        fprintf(fp, "%s %s %d\n",
                players[i].username,
                players[i].password,
                players[i].score);
    }

    fclose(fp);
}

/* ==================== REGISTER / LOGIN ==================== */

int register_user(void) {
    char username[64];
    char password[64];
    char confirm[64];
    int i;

    while (1) {
        show_score_bar();
        printf("=== Register ===\n");
        printf("Note:\n");
        printf("- Username will be stored in lowercase only.\n");
        printf("- Username and password cannot contain spaces.\n\n");

        printf("Enter new username (or type 'exit' to go home): ");
        if (!get_line(username, sizeof(username))) return -1;

        if (line_has_exit(username)) {
            return 0;
        }

        if (username[0] == '\0') {
            printf("Username cannot be empty.\n\n");
            continue;
        }

        if (has_space(username)) {
            printf("Username cannot contain spaces.\n\n");
            continue;
        }

        to_lowercase(username);

        /* check duplicate username */
        for (i = 0; i < playerCount; i++) {
            if (string_equal(players[i].username, username)) {
                printf("This username is already used. Please try another.\n\n");
                username[0] = '\0';
                break;
            }
        }
        if (username[0] == '\0') {
            continue;
        }

        printf("Enter new password (or type 'exit' to go home): ");
        if (!get_line(password, sizeof(password))) return -1;
        if (line_has_exit(password)) {
            return 0;
        }

        if (password[0] == '\0') {
            printf("Password cannot be empty.\n\n");
            continue;
        }

        if (has_space(password)) {
            printf("Password cannot contain spaces.\n\n");
            continue;
        }

        printf("Confirm password (or type 'exit' to go home): ");
        if (!get_line(confirm, sizeof(confirm))) return -1;
        if (line_has_exit(confirm)) {
            return 0;
        }

        if (confirm[0] == '\0') {
            printf("Confirm password cannot be empty.\n\n");
            continue;
        }

        if (has_space(confirm)) {
            printf("Confirm password cannot contain spaces.\n\n");
            continue;
        }

        if (!string_equal(password, confirm)) {
            printf("Passwords do not match. Please try again.\n\n");
            continue;
        }

        if (playerCount >= MAX_USERS) {
            printf("User limit reached. Cannot register more users.\n");
            return -1;
        }

        copy_with_limit(players[playerCount].username,
                        username, MAX_NAME_LEN);
        copy_with_limit(players[playerCount].password,
                        password, MAX_PASS_LEN);
        players[playerCount].score = 0;

        currentUserIndex = playerCount;
        playerCount++;

        save_users_to_file();

        printf("\nRegister complete! You are now logged in as %s.\n\n",
               players[currentUserIndex].username);
        return 1;
    }
}

int login_user(void) {
    char username[64];
    char password[64];
    int i;

    while (1) {
        show_score_bar();
        printf("=== Login ===\n");
        printf("Enter username (or type 'exit' to go home): ");
        if (!get_line(username, sizeof(username))) return -1;

        if (line_has_exit(username)) {
            return 0;
        }

        to_lowercase(username);

        printf("Enter password (or type 'exit' to go home): ");
        if (!get_line(password, sizeof(password))) return -1;

        if (line_has_exit(password)) {
            return 0;
        }

        for (i = 0; i < playerCount; i++) {
            if (string_equal(players[i].username, username) &&
                string_equal(players[i].password, password)) {
                currentUserIndex = i;
                printf("\nLogin successful! Welcome, %s.\n\n",
                       players[currentUserIndex].username);
                return 1;
            }
        }

        printf("Incorrect username or password. Please try again.\n\n");
    }
}

/* ==================== LESSON FUNCTIONS ==================== */
/* Each lesson has its own function.
 * Later, you can add real teaching content inside each function.
 * Now, each lesson still gives +5 score when visited.
 */

/* Lesson 1: scanf */
int lesson1(void) {
    char buffer[64];

    show_score_bar();
    printf("=== Lesson 1: scanf ===\n");
    printf("This lesson is still under development.\n");
    printf("In the future, this lesson will teach how to read input using scanf.\n");
    printf("You get +5 points for visiting this lesson.\n\n");

    if (currentUserIndex >= 0) {
        players[currentUserIndex].score += 5;
        save_users_to_file();
    }

    show_score_bar();
    printf("Press Enter to go back to main menu,\n");
    printf("or type 'exit' to go back to home: ");
    if (!get_line(buffer, sizeof(buffer))) return 1;
    if (line_has_exit(buffer)) return 0;
    return 1;
}

/* Lesson 2: printf */
int lesson2(void) {
    char buffer[64];

    show_score_bar();
    printf("=== Lesson 2: printf ===\n");
    printf("This lesson is still under development.\n");
    printf("In the future, this lesson will teach how to print output using printf.\n");
    printf("You get +5 points for visiting this lesson.\n\n");

    if (currentUserIndex >= 0) {
        players[currentUserIndex].score += 5;
        save_users_to_file();
    }

    show_score_bar();
    printf("Press Enter to go back to main menu,\n");
    printf("or type 'exit' to go back to home: ");
    if (!get_line(buffer, sizeof(buffer))) return 1;
    if (line_has_exit(buffer)) return 0;
    return 1;
}

/* Lesson 3: Data Types */
int lesson3(void) {
    char buffer[64];

    show_score_bar();
    printf("=== Lesson 3: Data Types ===\n");
    printf("This lesson is still under development.\n");
    printf("In the future, this lesson will explain basic data types in C.\n");
    printf("You get +5 points for visiting this lesson.\n\n");

    if (currentUserIndex >= 0) {
        players[currentUserIndex].score += 5;
        save_users_to_file();
    }

    show_score_bar();
    printf("Press Enter to go back to main menu,\n");
    printf("or type 'exit' to go back to home: ");
    if (!get_line(buffer, sizeof(buffer))) return 1;
    if (line_has_exit(buffer)) return 0;
    return 1;
}

/* Lesson 4: Loops */
int lesson4(void) {
    char buffer[64];

    show_score_bar();
    printf("=== Lesson 4: Loops ===\n");
    printf("This lesson is still under development.\n");
    printf("In the future, this lesson will cover for, while, and do-while loops.\n");
    printf("You get +5 points for visiting this lesson.\n\n");

    if (currentUserIndex >= 0) {
        players[currentUserIndex].score += 5;
        save_users_to_file();
    }

    show_score_bar();
    printf("Press Enter to go back to main menu,\n");
    printf("or type 'exit' to go back to home: ");
    if (!get_line(buffer, sizeof(buffer))) return 1;
    if (line_has_exit(buffer)) return 0;
    return 1;
}

/* Lesson 5: Arrays */
int lesson5(void) {
    char buffer[64];

    show_score_bar();
    printf("=== Lesson 5: Arrays ===\n");
    printf("This lesson is still under development.\n");
    printf("In the future, this lesson will teach how to use arrays.\n");
    printf("You get +5 points for visiting this lesson.\n\n");

    if (currentUserIndex >= 0) {
        players[currentUserIndex].score += 5;
        save_users_to_file();
    }

    show_score_bar();
    printf("Press Enter to go back to main menu,\n");
    printf("or type 'exit' to go back to home: ");
    if (!get_line(buffer, sizeof(buffer))) return 1;
    if (line_has_exit(buffer)) return 0;
    return 1;
}

/* ==================== MAIN MENU ==================== */

void main_menu(void) {
    char choice[16];
    int running = 1;

    while (running && currentUserIndex >= 0) {
        show_score_bar();
        printf("=== MAIN MENU ===\n");
        printf("1. Lesson 1: scanf\n");
        printf("2. Lesson 2: printf\n");
        printf("3. Lesson 3: Data Types\n");
        printf("4. Lesson 4: Loops\n");
        printf("5. Lesson 5: Arrays\n");
        printf("6. View my score\n");
        printf("7. Logout to Home\n");
        printf("----------------------------------------\n");
        printf("Select menu (1-7) or type 'exit' to go home: ");

        if (!get_line(choice, sizeof(choice))) {
            break;
        }

        if (line_has_exit(choice)) {
            currentUserIndex = -1;
            break;
        }

        if (choice[0] == '1') {
            if (!lesson1()) {
                currentUserIndex = -1;
                break;
            }
        } else if (choice[0] == '2') {
            if (!lesson2()) {
                currentUserIndex = -1;
                break;
            }
        } else if (choice[0] == '3') {
            if (!lesson3()) {
                currentUserIndex = -1;
                break;
            }
        } else if (choice[0] == '4') {
            if (!lesson4()) {
                currentUserIndex = -1;
                break;
            }
        } else if (choice[0] == '5') {
            if (!lesson5()) {
                currentUserIndex = -1;
                break;
            }
        } else if (choice[0] == '6') {
            show_score_bar();
            printf("=== My Score ===\n");
            if (currentUserIndex >= 0) {
                printf("Current score: %d points\n",
                       players[currentUserIndex].score);
            }
            printf("\nPress Enter to go back to main menu,\n");
            printf("or type 'exit' to go back to home: ");

            if (!get_line(choice, sizeof(choice))) continue;
            if (line_has_exit(choice)) {
                currentUserIndex = -1;
                break;
            }
        } else if (choice[0] == '7') {
            printf("Logging out...\n\n");
            currentUserIndex = -1;
            running = 0;
        } else {
            printf("Invalid menu. Please choose 1-7.\n\n");
        }
    }
}

/* ==================== MAIN FUNCTION ==================== */

int main(void) {
    char choice[16];
    int running = 1;

    load_users_from_file();

    while (running) {
        home();
        show_score_bar();

        printf("=== WELCOME ===\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit program\n");
        printf("----------------------------------------\n");
        printf("Select menu (1-3) or type 'exit' to exit program: ");

        if (!get_line(choice, sizeof(choice))) {
            break;
        }

        if (line_has_exit(choice)) {
            printf("\nGoodbye!\n");
            break;
        }

        if (choice[0] == '1') {
            int result = login_user();
            if (result == 1) {
                main_menu();
            } else if (result == 0) {
                continue;
            } else {
                break;
            }
        } else if (choice[0] == '2') {
            int result = register_user();
            if (result == 1) {
                main_menu();
            } else if (result == 0) {
                continue;
            } else {
                break;
            }
        } else if (choice[0] == '3') {
            printf("\nGoodbye!\n");
            running = 0;
        } else {
            printf("Invalid menu. Please choose 1-3.\n\n");
        }
    }

    return 0;
}