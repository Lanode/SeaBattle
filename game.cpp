#include "game.h"
#include <array>
#include "windows.h"


void Game::Loop()
{
    std::map<std::string, Vector> directions = {
        {"left",  {-1, 0}},
        {"up",    {0, -1}},
        {"right", {1, 0}},
        {"down",  {0, 1}},
    };

    bool continue_step = false;
    while (true) {
        continue_step = false;

        if (gamePhase == GamePhase::gpPlacing) {
            GameArea* gameArea = playerAreas[playerStep];
            RenderGameArea(gameArea, false);
            std::string coord_inp, direction_inp, type_inp;
            int ship_type;

            std::cout << playerStep << "> ";
            std::cin >> coord_inp >> direction_inp >> ship_type;

            Vector coord = {coord_inp[0] - 'a', coord_inp[1] - '0' - 1};
            //std::cout << coord.x << " " << coord.y << std::endl;
            
            PlaceResult res = gameArea->PlaceShip(coord, directions[direction_inp], (ShipType)(ship_type-1));
            if (res == PlaceResult::prForbidden || res == PlaceResult::prAlreadyPlaced) {
                std::cout << "нельзя так размещать" << std::endl;
                Sleep(messageShowDelay);
                continue_step = true;
            } else if (res == PlaceResult::prPlaced) {
                std::cout << "размещено" << std::endl;
                Sleep(messageShowDelay);
            }

            int allPlacedPlayerCount = 0;
            for (GameArea* gameArea : playerAreas) {
                if (gameArea->GetDockedShipsCount() == 0)
                    allPlacedPlayerCount++;
            }
            if (allPlacedPlayerCount == 2) {
                gamePhase = GamePhase::gpHitting;
            }
        }
        if (gamePhase == GamePhase::gpHitting) {
            GameArea* gameArea;
            if (playerStep == 0)
                gameArea = playerAreas[1];
            else
                gameArea = playerAreas[0];
            RenderGameArea(gameArea, true);
            std::string coord_inp;
            std::cout << playerStep << "> ";
            std::cin >> coord_inp;
            Vector coord = {coord_inp[0] - 'a', coord_inp[1] - '0' - 1};

            HitResult res = gameArea->HitShip(coord);
            switch (res)
            {
            case HitResult::hrMissed:
                std::cout << "промах" << std::endl;
                Sleep(messageShowDelay);
                break;
            case HitResult::hrForbidden:
                std::cout << "уже стрелял сюда попробуй другое место" << std::endl;
                Sleep(messageShowDelay);
                break;
            case HitResult::hrStruck:
                std::cout << "попал" << std::endl;
                Sleep(messageShowDelay);
                continue_step = true;
                break;
            case HitResult::hrSinked:
                std::cout << "затопил" << std::endl;
                Sleep(messageShowDelay);
                continue_step = true;
                break;
            }

            if (gameArea->GetDockedShipsCount() >= gameArea->GetTotalShipCount())
                std::cout << "игрок " << playerStep << " выиграл" << std::endl;
        }

        if (!continue_step)
            if (playerStep == 0)
                playerStep = 1;
            else
                playerStep = 0;
    }
}

void Game::RenderGameArea(GameArea* gameArea, bool hide)
{
    std::array<std::string, 4> shipTypesNames{"один", "два", "три", "четыре"};
    system("cls");
    if (gamePhase == gpPlacing) {
        std::cout << "==== РЕЖИМ РАЗМЕЩЕНИЯ ====" << std::endl;
        std::cout << "разместите корабль (<координата> <поворот> <кол-во палуб>) (a4 right 2)" << std::endl;
    } else if (gamePhase == gpHitting) {
        std::cout << "==== РЕЖИМ ПОДБИТИЯ ====" << std::endl;
        std::cout << "подбейте корабль (координата) (a4)" << std::endl;
    }
    std::cout << "  a b c d e f g h i j" << std::endl;
    for (int y=0; y < 10; y++) {
        std::cout << y << " ";
        for (int x=0; x < 10; x++) {
            CellType cell = gameArea->GetCell({x, y});
            //std::cout << (int)cell << " ";
             if (cell == CellType::ctNone || cell == CellType::ctShipArea)
                 std::cout << ". ";
             if (cell == CellType::ctMiss)
                 std::cout << "M ";
             if (cell == CellType::ctHit)
                 std::cout << "X ";
             if (cell == CellType::ctShip)
                 std::cout << "S ";
            
        }
        if (y >= 0 && y < 4)
            std::cout << shipTypesNames[y] << ": " << gameArea->GetDockedShipsCount((ShipType)y);
        std::cout << std::endl;
    }
}
