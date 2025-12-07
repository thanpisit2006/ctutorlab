// level5
// cat

#include <stdio.h>
#include "levels.h"

void play_level5(Player *player) {
    /* 
     level5 function
     
    */

    printf("\n=== Level 5 ===\n");
    printf("Level 5 is currently under development.\n");
    printf("You receive +10 points for visiting this level.\n");

    /* update player's score +10 points */
    if (update_player_score(player, 10)) {
        printf("Your new score: %d\n", player->score);
    } else {
        printf("Failed to update your score.\n");
    }
}