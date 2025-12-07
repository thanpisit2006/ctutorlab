// player.c
// managing player data: read/write from file,
// registering new players, logging in, updating scores

#include <stdio.h>
#include <string.h>
#include "player.h"

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
        printf("Error: cannot open player data file for writing.\n");
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
        if (strcmp(name, players[i].name) == 0) {
            return i; // player found
        }
    }
    return -1; // player not found
}

int register_player(Player *out_player) {
    Player players[MAX_PLAYERS];
    int count;
    char name[MAX_NAME_LEN + 1];

    // read all existing players from file
    count = load_all_players(players, MAX_PLAYERS);

    printf("\n=== Register ===\n");
    printf("Enter new player name (no spaces): ");
    if (scanf("%31s", name) != 1) {
        printf("Invalid input.\n");
        return 0;
    }

    // check for duplicate name
    if (find_player_index(name, players, count) != -1) {
        printf("This name is already taken. Please try again.\n");
        return 0;
    }

    // check if the player list is full
    if (count >= MAX_PLAYERS) {
        printf("Player list is full. Cannot register a new player.\n");
        return 0;
    }

    // add new player in array
    strcpy(players[count].name, name);
    players[count].score = 0; // start score = 0
    count++;

    // save to the file
    if (!save_all_players(players, count)) {
        printf("Failed to save player data.\n");
        return 0;
    }

    // write data back to the output player pointer
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

    // read all players from file
    count = load_all_players(players, MAX_PLAYERS);

    printf("\n=== Login ===\n");
    printf("Enter player name: ");
    if (scanf("%31s", name) != 1) {
        printf("Invalid input.\n");
        return 0;
    }

    // if no player
    if (count == 0) {
        printf("No players found. Please register first.\n");
        return 0;
    }

    // search for player name
    idx = find_player_index(name, players, count);
    if (idx == -1) {
        printf("Player not found. Please register first.\n");
        return 0;
    }

    // send data into the output player struct
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

    // read all player data from file
    count = load_all_players(players, MAX_PLAYERS);

    if (count == 0) {
        printf("Error: no player data found.\n");
        return 0;
    }

    // find index of player
    idx = find_player_index(player->name, players, count);
    if (idx == -1) {
        printf("Error: current player not found in data file.\n");
        return 0;
    }

    // update score in array and player variable
    players[idx].score += delta;
    if (players[idx].score < 0) {
        players[idx].score = 0; // prevent negative scores
    }

    player->score = players[idx].score;

    // write update to the file
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