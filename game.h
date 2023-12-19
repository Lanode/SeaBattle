#include <iostream>
#include "gamelogic.h"
#include "render.h"

enum GamePhase { gpPlacing, gpHitting };

class Game 
{
    std::vector<GameArea> playerAreas {GameArea(), GameArea()};
	GamePhase gamePhase = GamePhase::gpPlacing;
    int playerStep = 0;

    void RenderGameArea(GameArea gameArea, bool hide);

public:
    void Loop();
};