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

//🏝️ STORY: “Survival Decision System on a Deserted Island”

/*You wake up on a deserted island after a shipwreck.
You only have a backpack, a small knife, and a watch.
To survive, you must make the right decisions in each situation!
Let’s create a decision-making program using if–else to help you survive.*/

/*🪶 Problem 1 (Easy): Check the Weather Before Exploring
🧭 Problem Description*/

/*Write a program to determine whether today is suitable for exploring the island,
using the wind speed (in kilometers per hour) entered by the user.*/

//🔹 Conditions

//If the wind speed is less than 10, display "Go explore!" (safe to explore)

//If the wind speed is between 10–24, display "Be careful!" (you may explore but should be cautious)

//If the wind speed is 25 or more, display "Stay in camp!" (dangerous to go out)

/*🔹 Examples
Input	Output
5	Go explore!
15	Be careful!
30	Stay in camp!8*/

