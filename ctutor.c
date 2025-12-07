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
#include <stdlib.h>
#include <string.h>

// player.h
// declares player data structure and functions to manage player info


#define MAX_NAME_LEN 31                 // maximum length of a player name
#define MAX_PLAYERS 100                 // maximum number of players supported in the file
#define PLAYER_DATA_FILE "players.txt"  // name of file used to store player data

typedef struct {
    char name[MAX_NAME_LEN + 1];  // player name
    int score;                    // player total score
} Player;

// read all players from file and send to array
// returns the number of players (= 0 if file does not exist or empty)
int load_all_players(Player players[], int max);

// search for player by name in the array
// returns the index if found, or -1 if not found
int find_player_index(const char *name, Player players[], int count);

// write all players from the array back into file (overwrites the entire file)
int save_all_players(Player players[], int count);

// register new player.
// success: writes the new player into *out_player and returns 1
// failure (duplicate name): returns 0
int register_player(Player *out_player);

// log in an existing player
// success: writes the player's data into *out_player and returns 1
// if name is not found: returns 0
int login_player(Player *out_player);

// update a player score and write to file
// delta > 0 increase the score; delta < 0 decrease score
// returns 1 on success or 0 on failure
int update_player_score(Player *player, int delta);

// display player information (name, score)
void print_player_info(const Player *player);

// player.c
// managing player data: read/write from file,
// registering new players, logging in, updating scores


int load_all_players(Player players[], int max) {
    FILE *fp;
    int count = 0;

    // open the file. if it does not exist, treat it as no player in system
    fp = fopen(PLAYER_DATA_FILE, "r");
    if (fp == NULL) {
        return 0; //no file
    }

    // read data from file in format: name score
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
        printf("Error: cannot open %s for writing.\n", PLAYER_DATA_FILE);
        return 0;
    }

    // write all players data back to file
    for (i = 0; i < count; i++) {
        fprintf(fp, "%s %d\n", players[i].name, players[i].score);
    }

    fclose(fp);
    return 1;
}

int find_player_index(const char *name, Player players[], int count) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(players[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// helper: trim newline at end of string
static void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

int register_player(Player *out_player) {
    Player players[MAX_PLAYERS];
    int count, exists, i;
    char name_input[256];

    // read all player data
    count = load_all_players(players, MAX_PLAYERS);

    printf("=== Register New Player ===\n");
    printf("Enter your username: ");

    if (fgets(name_input, sizeof(name_input), stdin) == NULL) {
        printf("Error reading name.\n");
        return 0;
    }

    trim_newline(name_input);

    // remove leading/trailing spaces
    // also ensure not empty
    if (name_input[0] == '\0') {
        printf("Name cannot be empty.\n");
        return 0;
    }

    // check for duplicate (case sensitive)
    exists = 0;
    for (i = 0; i < count; i++) {
        if (strcmp(players[i].name, name_input) == 0) {
            exists = 1;
            break;
        }
    }

    if (exists) {
        printf("This username already exists. Please try another name.\n");
        return 0;
    }

    // create new player
    strncpy(out_player->name, name_input, MAX_NAME_LEN);
    out_player->name[MAX_NAME_LEN] = '\0';
    out_player->score = 0;

    // append to array
    if (count < MAX_PLAYERS) {
        players[count] = *out_player;
        count++;
    } else {
        printf("Error: maximum number of players reached.\n");
        return 0;
    }

    // write all data back to file
    if (!save_all_players(players, count)) {
        printf("Error: could not save player data.\n");
        return 0;
    }

    printf("Registration successful! Welcome, %s.\n", out_player->name);
    return 1;
}

int login_player(Player *out_player) {
    Player players[MAX_PLAYERS];
    int count;
    char name_input[256];
    int idx;

    // load all players
    count = load_all_players(players, MAX_PLAYERS);
    if (count == 0) {
        printf("No registered players found. Please register first.\n");
        return 0;
    }

    printf("=== Login ===\n");
    printf("Enter your username: ");
    if (fgets(name_input, sizeof(name_input), stdin) == NULL) {
        printf("Error reading name.\n");
        return 0;
    }

    trim_newline(name_input);

    if (name_input[0] == '\0') {
        printf("Name cannot be empty.\n");
        return 0;
    }

    idx = find_player_index(name_input, players, count);
    if (idx == -1) {
        printf("Username not found. Please check spelling or register.\n");
        return 0;
    }

    *out_player = players[idx];
    printf("Login successful! Welcome back, %s (Score: %d).\n",
           out_player->name, out_player->score);
    return 1;
}

int update_player_score(Player *player, int delta) {
    Player players[MAX_PLAYERS];
    int count;
    int idx;

    // load all players
    count = load_all_players(players, MAX_PLAYERS);

    // find this player in array
    idx = find_player_index(player->name, players, count);
    if (idx == -1) {
        printf("Error: player not found in database.\n");
        return 0;
    }

    // update score
    players[idx].score += delta;
    if (players[idx].score < 0) {
        players[idx].score = 0;
    }

    // update caller's copy too
    player->score = players[idx].score;

    // save all players
    if (!save_all_players(players, count)) {
        printf("Error: could not save updated score.\n");
        return 0;
    }

    return 1;
}

void print_player_info(const Player *player) {
    printf("=== Player Info ===\n");
    printf("Name : %s\n", player->name);
    printf("Score: %d\n", player->score);
}


// ======================= LEVEL 1 =======================
// level1
// bank

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

/* Helper: trim leading/trailing spaces from a string */
static void trim_spaces(char *s) {
    char *start = s;
    char *end;

    /* skip leading space */
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

/* Compare two code strings ignoring spaces and tabs */
static int compare_code(const char *user, const char *answer) {
    while (*user != '\0' || *answer != '\0') {
        /* skip spaces/tabs in user */
        while (*user == ' ' || *user == '\t') {
            user++;
        }
        /* skip spaces/tabs in answer */
        while (*answer == ' ' || *answer == '\t') {
            answer++;
        }
        if (*user != *answer) {
            return 0; // different
        }
        if (*user == '\0' && *answer == '\0') {
            break;
        }
        user++;
        answer++;
    }
    return 1; // same
}

void play_level1(Player *player) {
    char input[256];
    int step_ok;
    int level_score = 0;
    int want_next = 0;

    printf("\n=== Level 1: Basic printf and scanf ===\n");

    printf("In this level, you will learn to use printf and scanf step by step.\n");
    printf("Type " COLOR_GREEN "next" COLOR_RESET " to go to the next step.\n");
    printf("If you need help, type " COLOR_GREEN "hint" COLOR_RESET ".\n\n");

    // ---------- Step 1: Basic printf ----------
    do {
        step_ok = 0;
        printf("Step 1: Print this exact line:\n");
        printf("  Hello C Tutor Lab!\\n\n");
        printf("Try typing the C code:\n");
        printf("  printf(\"Hello C Tutor Lab!\\\\n\");\n\n");

        while (!step_ok) {
            printf("Your code (or type 'hint'): ");
            read_line(input, sizeof(input));
            trim_spaces(input);

            if (strcmp(input, "hint") == 0) {
                printf(COLOR_GREEN "Hint: " COLOR_RESET
                       "Use printf(\"Hello C Tutor Lab!\\\\n\");\n");
                continue;
            }

            if (compare_code(input, "printf(\"Hello C Tutor Lab!\\n\");")) {
                printf(COLOR_GREEN "Correct! " COLOR_RESET "Great job.\n");
                level_score += 5;
                step_ok = 1;
            } else {
                printf(COLOR_RED "Not correct yet." COLOR_RESET
                       " Remember the exact text and \\n.\n");
            }
        }

        printf("Go to next step? (type 'next' to continue): ");
        read_line(input, sizeof(input));
        trim_spaces(input);
        if (strcmp(input, "next") == 0) {
            want_next = 1;
        } else {
            printf("You chose not to continue. Ending level 1.\n");
            return;
        }
    } while (!want_next);

    // ---------- Step 2: printf with integer ----------
    printf("\nStep 2: Print your age using printf.\n");
    printf("For example:\n");
    printf("  int age = 18;\n");
    printf("  printf(\"I am %d years old.\\\\n\", age);\n\n");

    step_ok = 0;
    while (!step_ok) {
        printf("Your code (or type 'hint'): ");
        read_line(input, sizeof(input));
        trim_spaces(input);

        if (strcmp(input, "hint") == 0) {
            printf(COLOR_GREEN "Hint: " COLOR_RESET
                   "int age = 18; printf(\"I am %d years old.\\\\n\", age);\n");
            continue;
        }

        if (compare_code(input,
                         "int age = 18; printf(\"I am %d years old.\\n\", age);")) {
            printf(COLOR_GREEN "Correct!" COLOR_RESET " Nice.\n");
            level_score += 5;
            step_ok = 1;
        } else {
            printf(COLOR_RED "Not correct yet." COLOR_RESET
                   " Check spaces, %d, and \\n.\n");
        }
    }

    printf("Go to next step? (type 'next' to continue): ");
    read_line(input, sizeof(input));
    trim_spaces(input);
    if (strcmp(input, "next") != 0) {
        printf("You chose not to continue. Ending level 1.\n");
        return;
    }

    // ---------- Step 3: scanf with integer ----------
    printf("\nStep 3: Use scanf to read an integer.\n");
    printf("For example:\n");
    printf("  int x;\n");
    printf("  scanf(\"%d\", &x);\n\n");

    step_ok = 0;
    while (!step_ok) {
        printf("Your code (or type 'hint'): ");
        read_line(input, sizeof(input));
        trim_spaces(input);

        if (strcmp(input, "hint") == 0) {
            printf(COLOR_GREEN "Hint: " COLOR_RESET
                   "int x; scanf(\"%d\", &x);\n");
            continue;
        }

        if (compare_code(input, "int x; scanf(\"%d\", &x);")) {
            printf(COLOR_GREEN "Correct!" COLOR_RESET " You understand scanf.\n");
            level_score += 5;
            step_ok = 1;
        } else {
            printf(COLOR_RED "Not correct yet." COLOR_RESET
                   " You must declare int x; and use &x.\n");
        }
    }

    // finish level
    printf("\nYou finished Level 1 with score: %d\n", level_score);

    if (update_player_score(player, level_score)) {
        printf("Your total score is now: %d\n", player->score);
    } else {
        printf("Could not update your score.\n");
    }
}


// ======================= LEVEL 2 =======================
// level2
// pnoi1mil

void play_level2(Player *player) {
    printf("\n=== Level 2 ===\n");
    printf("Level 2 is currently under development.\n");
    printf("Stay tuned, %s! Your current score: %d\n\n", player->name, player->score);
}


// ======================= LEVEL 3 =======================
// level3
// film

void play_level3(Player *player) {
    printf("\n=== Level 3 ===\n");
    printf("Level 3 is currently under development.\n");
    printf("Stay tuned, %s! Your current score: %d\n\n", player->name, player->score);
}


// ======================= LEVEL 4 =======================
// level4
// ming

void play_level4(Player *player) {
    printf("\n=== Level 4 ===\n");
    printf("Level 4 is currently under development.\n");
    printf("Stay tuned, %s! Your current score: %d\n\n", player->name, player->score);
}


// ======================= LEVEL 5 =======================
// level5
// cat

void play_level5(Player *player) {
    printf("\n=== Level 5 ===\n");
    printf("Level 5 is currently under development.\n");
    printf("Stay tuned, %s! Your current score: %d\n\n", player->name, player->score);
}


// ======================= main.c (game flow) =======================
/* main.c
 main program on terminal
 - welcome page
 - register and login page
 - show 1-5 levels and players info
 */

//show welcome page
void show_welcome_screen(void) {
    printf("========================================\n");
    printf("              C Tutor Lab\n");
    printf("          CPE100 Term Project\n");
    printf("----------------------------------------\n");
    printf("                MEMBERS\n");
    printf(" 68070503415  Chadathong  Tangjitpakorn\n");
    printf(" 68070503422  Nichapa\t  Piromying\n");
    printf(" 68070503427  Thanpisit\t  Ritpetchnil\n");
    printf(" 68070503445  Peerawat\t  Boonyaprasit\n");
    printf(" 68070503457  Supakorn\t  Polphichai\n");
    printf("========================================\n\n");
}

// show register/login
void show_auth_menu(void) {
    printf("=== Player Menu ===\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("0. Exit\n");
    printf("-------------------\n");
}

// games menu
void show_game_menu(const Player *player) {
    printf("\n=== Main Game Menu ===\n");
    printf("Current player: %s (Score: %d)\n", player->name, player->score);
    printf("1. Play Level 1\n");
    printf("2. Play Level 2\n");
    printf("3. Play Level 3\n");
    printf("4. Play Level 4\n");
    printf("5. Play Level 5\n");
    printf("6. Show Player Info\n");
    printf("0. Logout / Exit\n");
    printf("-------------------------\n");
}

int main(void) {
    Player currentPlayer;
    int logged_in = 0;  // flag whether the player is logged in
    int choice;
    int ch;

    show_welcome_screen();

    // first loop: force user to register or login before accessing the game
    while (!logged_in) {
        show_auth_menu();
        printf("Select: ");
        if (scanf("%d", &choice) != 1) {
            // if input not a number -> clear input buffer
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("Please enter a number (0-2).\n\n");
            continue;
        }
        // clear leftover newline in input buffer
        while ((ch = getchar()) != '\n' && ch != EOF) { }

        if (choice == 1) {
            // register
            if (register_player(&currentPlayer)) {
                logged_in = 1;
            }
        } else if (choice == 2) {
            // login
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

    // second loop: main game menu
    while (1) {
        show_game_menu(&currentPlayer);
        printf("Select: ");
        if (scanf("%d", &choice) != 1) {
            // clear invalid input
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("Please enter a number (0-6).\n\n");
            continue;
        }
        // clear leftover newline
        while ((ch = getchar()) != '\n' && ch != EOF) { }

        switch (choice) {
            case 1:
                play_level1(&currentPlayer);
                break;
            case 2:
                play_level2(&currentPlayer);
                break;
            case 3:
                play_level3(&currentPlayer);
                break;
            case 4:
                play_level4(&currentPlayer);
                break;
            case 5:
                play_level5(&currentPlayer);
                break;
            case 6:
                print_player_info(&currentPlayer);
                break;
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