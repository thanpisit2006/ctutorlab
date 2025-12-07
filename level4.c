// level4
// ming

#include <stdio.h>
#include "levels.h"

void play_level4(Player *player) {
    /* 
     level4 function
    */

    printf("\n=== Level 4 ===\n");
    printf("Level 4 is currently under development.\n");
    printf("You receive +10 points for visiting this level.\n");

    /* update player's score +10 points */
    if (update_player_score(player, 10)) {
        printf("Your new score: %d\n", player->score);
    } else {
        printf("Failed to update your score.\n");
    }
}