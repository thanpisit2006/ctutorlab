// player.h
// declares player data structure and functions to manage player info

#ifndef PLAYER_H
#define PLAYER_H

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

#endif /* PLAYER_H */