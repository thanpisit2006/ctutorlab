/* main.c
 main program on terminal
 - welcome page
 - register and login page
 - show 1-5 levels and players info
 */

#include <stdio.h>
#include "player.h"
#include "levels.h"

//show welcome page
void show_welcome_screen(void) {
    printf("========================================\n");
    printf("              C Tutor Lab\n");
    printf("          CPE100 Term Project\n");
    printf("----------------------------------------\n");
    printf("                MEMBERS\n");
    printf(" 68070503415  Chadathong  Tangjitpakorn\n");
    printf(" 68070503422  Nichapa	  Piromying\n");
    printf(" 68070503427  Thanpisit	  Ritpetchnil\n");
    printf(" 68070503445  Peerawat	  Boonyaprasit\n");
    printf(" 68070503457  Supakorn	  Polphichai\n");
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
    printf("6. Show player info\n");
    printf("0. Logout and exit\n");
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

    // after successful login, enter the main game loop
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