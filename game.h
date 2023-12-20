#include <iostream>
#include "gamelogic.h"
#include "render.h"

enum GamePhase { gpPlacing, gpHitting };

class Game 
{
    const int messageShowDelay = 500;

    std::vector<GameArea*> playerAreas {new GameArea(), new GameArea()};
	GamePhase gamePhase = GamePhase::gpPlacing;
    int playerStep = 0;

    void RenderGameArea(int step, GameArea* gameArea, bool hide);

public:
    void Loop();
};