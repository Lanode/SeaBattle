#include <iostream>
#include "gamelogic.h"
#include "render.h"

enum GamePhase { Placing, Hitting };

class Game 
{
    std::vector<GameArea> playerAreas {GameArea(), GameArea()};
	GamePhase gamePhase = GamePhase::Placing;
    uint8_t playerStep = 0;

    void Loop();
    void RenderGameArea(GameArea gameArea, bool hide);
};