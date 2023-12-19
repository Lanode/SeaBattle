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
        if (gamePhase == GamePhase::gpPlacing) {
            GameArea gameArea = playerAreas[playerStep];
            RenderGameArea(gameArea, false);
            std::string coord_inp, direction_inp, type_inp;
            int ship_type;

            std::cout << playerStep << "> ";
            std::cin >> coord_inp >> direction_inp >> ship_type;

            Vector coord = {coord_inp[0] - 'a', coord_inp[1] - '0'};
            std::cout << coord.x << " " << coord.y << std::endl;
            
            PlaceResult res = gameArea.PlaceShip(coord, directions[direction_inp], (ShipType)(ship_type-1));
            if (res == PlaceResult::prForbidden || res == PlaceResult::prAlreadyPlaced) {
                std::cout << "нельзя так размещать" << std::endl;
                continue_step = true;
            } else if (res == PlaceResult::prPlaced) {
                std::cout << "размещено" << std::endl;
            }

            int allPlacedPlayerCount = 0;
            for (GameArea gameArea : playerAreas) {
                if (gameArea.GetDockedShipsCount() == 0)
                    allPlacedPlayerCount++;
            }
            if (allPlacedPlayerCount == 2) {
                gamePhase = GamePhase::gpHitting;
                std::cout << "==== РЕЖИМ ПОДБИТИЯ ====" << std::endl;
                std::cout << "подбейте корабль (координата) (a4)" << std::endl;
            }
        }
        if (gamePhase == GamePhase::gpHitting) {
            std::string coord_inp;
            std::cout << playerStep << "> ";
            std::cin >> coord_inp;
            Vector coord = {coord_inp[0] - 'a', coord_inp[1] - '0'};
            GameArea gameArea;
            if (playerStep == 0) {
                gameArea = playerAreas[1];
            } else {
                gameArea = playerAreas[0];
            }

            HitResult res = gameArea.HitShip(coord);
            switch (res)
            {
            case HitResult::hrMissed:
                std::cout << "промах" << std::endl;
                break;
            case HitResult::hrForbidden:
                std::cout << "уже стрелял сюда попробуй другое место" << std::endl;
                break;
            case HitResult::hrStruck:
                std::cout << "попал" << std::endl;
                continue_step = true;
                break;
            case HitResult::hrSinked:
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
    for (int x=0; x < 10; x++) {
        for (int y=0; y < 10; y++) {
            CellType cell = gameArea.GetCell({x, y});
            std::cout << (int)cell << " ";
            // if (cell == CellType::ctNone || cell == CellType::ctShipArea)
            //     std::cout << "0 ";
            // if (cell == CellType::ctMiss)
            //     std::cout << "M ";
            // if (cell == CellType::ctHit)
            //     std::cout << "X ";
            // if (cell == CellType::ctShip)
            //     std::cout << "S ";
            
        }
        std::cout << std::endl;
    }
}
