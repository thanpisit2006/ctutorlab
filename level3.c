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

#include <stdio.h>

int main() {
    int urgency;
    scanf("%d", &urgency);

    if (urgency >= 0 && urgency <= 4) {
        printf("ASAP\n");
    } else if (urgency <= 9) {
        printf("Soon\n");
    } else {
        printf("Later\n");
    }

    return 0;
}

/*Problem 2 (Medium): Calculate How Much Food You Should Eat
🧭 Problem Description*/

/*You have limited food supplies, so you must decide how much to eat based on your hunger level.
The program receives a hunger level (0–10) and tells you how many bowls of rice you should eat.*/

//🔹 Conditions

//Hunger level 0–3 → display "Eat 1 bowl."

//Hunger level 4–6 → display "Eat 2 bowls."

//Hunger level 7–9 → display "Eat 3 bowls."

//Hunger level 10 → display "Eat everything!"

/*🔹 Examples
Input	Output
2	Eat 1 bowl.
5	Eat 2 bowls.
9	Eat 3 bowls.
10	Eat everything!*/

#include <stdio.h>

int main() {
    int hunger;
    scanf("%d", &hunger);

    if (hunger <= 3) {
        printf("Eat 1 bowl.\n");
    } else if (hunger <= 6) {
        printf("Eat 2 bowls.\n");
    } else if (hunger <= 9) {
        printf("Eat 3 bowls.\n");
    } else {
        printf("Eat everything!\n");
    }
    return 0;
}

/*🐍 Problem 3 (Hard): Survive a Wild Animal Encounter
🧭 Problem Description*/

/*While exploring the jungle, you come across a wild animal.
You must decide how to react based on the type of animal and its distance from you.*/

//🔹 Input

//A character representing the animal type:

//T = Tiger

//S = Snake

//M = Monkey

//An integer representing the distance in meters.

/*🔹 Conditions
Animal	Distance	Action
Tiger (T)	Less than 10	"Run away!"
Tiger (T)	10 or more	"Stay still."
Snake (S)	Less than 5	"Step back slowly."
Snake (S)	5 or more	"Ignore it."
Monkey (M)	Less than 3	"Offer food."
Monkey (M)	3 or more	"Wave hello."
Any other character	—	"Unknown creature!"
🔹 Example Input/Output
Input	Output
T 7	Run away!
S 6	Ignore it.
M 1	Offer food.
X 10	Unknown creature!*/


#include <stdio.h>

int main() {
    char animal;
    int distance;

    scanf(" %c %d", &animal, &distance);

    if (animal == 'T') {
        if (distance < 10)
            printf("Run away!\n");
        else
            printf("Stay still.\n");
    }
    else if (animal == 'S') {
        if (distance < 5)
            printf("Step back slowly.\n");
        else
            printf("Ignore it.\n");
    }
    else if (animal == 'M') {
        if (distance < 3)
            printf("Offer food.\n");
        else
            printf("Wave hello.\n");
    }
    else {
        printf("Unknown creature!\n");
    }

    return 0;
}


