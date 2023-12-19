#include <stdio.h>
#include "gamelogic.h"
#include "render.h"

enum GamePhase { Placing, Hitting };

class Game 
{
    std::vector<GameArea> playerAreas {GameArea(), GameArea()};
	GamePhase gamePhase = GamePhase::Placing;
    uint8_t playerStep = 0;

    void Loop()
    {
        int c = 0;
        while (true) {
            c = getch()
            


            if ()
            if (gamePhase == GamePhase::Placing) {
                playerAreas[playerStep]
            }
        };
    }
};