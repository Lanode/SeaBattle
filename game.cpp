#include "game.h"



void Game::Loop()
{
    std::map<std::string, Vector> directions = {
        {"left",  {-1, 0}},
        {"up",    {0, -1}},
        {"right", {1, 0}},
        {"down",  {0, 1}},
    };

    std::cout << "==== РЕЖИМ РАЗМЕЩЕНИЯ ====" << std::endl;
    std::cout << "разместите корабль (координата, поворот, кол-во палуб) (a4, right, 2)" << std::endl;
    bool continue_step = false;
    while (true) {
        continue_step = false;
        if (gamePhase == GamePhase::Placing) {
            std::string coord_inp, direction_inp, type_inp;
            int ship_type;

            std::cout << "> ";
            std::cin >> coord_inp >> direction_inp >> ship_type;

            Vector coord = {coord_inp[0] - 'a', coord_inp[0] - '0'};
            
            GameArea gameArea = playerAreas[playerStep];
            PlaceResult res = gameArea.PlaceShip(coord, directions[direction_inp], (ShipType)(ship_type-1));
            switch (res)
            {
            case PlaceResult::Forbidden:
                std::cout << "нельзя так размещать" << std::endl;
                continue_step = true;
                break;
            
            default:
                break;
            }

            int allPlacedPlayerCount = 0;
            for (GameArea gameArea : playerAreas) {
                if (gameArea.GetDockedShipsCount() == 0)
                    allPlacedPlayerCount++;
            }
            if (allPlacedPlayerCount == 2) {
                gamePhase = GamePhase::Hitting;
                std::cout << "==== РЕЖИМ ПОДБИТИЯ ====" << std::endl;
                std::cout << "подбейте корабль (координата) (a4)" << std::endl;
            }
        }
        if (gamePhase == GamePhase::Hitting) {
            std::string coord_inp;
            std::cout << "> ";
            std::cin >> coord_inp;
            Vector coord = {coord_inp[0] - 'a', coord_inp[0] - '0'};
            GameArea gameArea;
            if (playerStep == 0) {
                gameArea = playerAreas[1];
            } else {
                gameArea = playerAreas[0];
            }

            HitResult res = gameArea.HitShip(coord);
            switch (res)
            {
            case HitResult::Missed:
                std::cout << "промах" << std::endl;
                break;
            case HitResult::Forbidden:
                std::cout << "уже стрелял сюда попробуй другое место" << std::endl;
                break;
            case HitResult::Struck:
                std::cout << "попал" << std::endl;
                continue_step = true;
                break;
            case HitResult::Sinked:
                continue_step = true;
                std::cout << "затопил" << std::endl;
                break;
            }

            if (gameArea.GetDockedShipsCount() >= gameArea.GetTotalShipCount())
                std::cout << "игрок " << playerStep << " выиграл" << std::endl;
        }

        if (!continue_step)
            if (playerStep == 0)
                playerStep = 1;
            else
                playerStep = 0;
    }
}

void Game::RenderGameArea(GameArea gameArea, bool hide)
{
    for (int x=0; x < 10; x++)
        for (int y=0; y < 10; y++) {
            CellType cell = gameArea.GetCell({x, y});
            switch (cell)
            {
            case CellType::None:
                std::cout << "0 ";
                break;
            
            case CellType::Miss:
                std::cout << "M ";
                break;

            case CellType::Hit:
                std::cout << "X ";
                break;

            case CellType::Ship:
                if (!hide)
                    std::cout << "S ";
                break;
            }
            std::cout << std::endl;
        }
}
