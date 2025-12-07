// level3
// film

#include <stdio.h>
#include "levels.h"

void play_level3(Player *player) {
    /* 
     level3 function
    */

    printf("\n=== Level 3 ===\n");
    printf("Level 3 is currently under development.\n");
    printf("You receive +10 points for visiting this level.\n");

    /* update player's score +10 points */
    if (update_player_score(player, 10)) {
        printf("Your new score: %d\n", player->score);
    } else {
        printf("Failed to update your score.\n");
    }
}