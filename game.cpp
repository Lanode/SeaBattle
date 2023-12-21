#include "game.h"
#include <array>
#include <iomanip>
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
    bool running = true;
    while (running) {
        continue_step = false;

        if (gamePhase == GamePhase::gpPlacing) {
            Render();
            std::string coord_inp, direction_inp, type_inp;
            int ship_type;

            std::cout << playerStep << "> ";
            std::cin >> coord_inp >> direction_inp >> ship_type;

            Vector coord = {coord_inp[0] - 'a', coord_inp[1] - '0' - 1};
            //std::cout << coord.x << " " << coord.y << std::endl;
            
            GameArea* gameArea = playerAreas[playerStep];

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
            Render();
            std::string coord_inp;
            std::cout << playerStep << "> ";
            std::cin >> coord_inp;
            Vector coord = {coord_inp[0] - 'a', coord_inp[1] - '0' - 1};

            int oppositePlayer = playerStep == 0 ? 1 : 0;
            GameArea* gameArea = playerAreas[oppositePlayer];

            HitResult res = gameArea->HitShip(coord);
            if (res == HitResult::hrMissed) {
                std::cout << "промах" << std::endl;
                Sleep(messageShowDelay);
            } else if (res == HitResult::hrForbidden) {
                std::cout << "уже стрелял сюда попробуй другое место" << std::endl;
                Sleep(messageShowDelay);
                continue_step = true;
            } else if (res == HitResult::hrStruck) {
                std::cout << "попал" << std::endl;
                Sleep(messageShowDelay);
                continue_step = true;
            } else if (res == HitResult::hrSinked) {
                std::cout << "затопил" << std::endl;
                Sleep(messageShowDelay);
                continue_step = true;
            }

            if (gameArea->GetDockedShipsCount() >= gameArea->GetTotalShipCount()) {
                std::cout << "==== ИГРОК " << playerStep << " ВЫИГРАЛ ====" << std::endl;
                running = false;
            }
        }

        if (!continue_step)
            if (playerStep == 0)
                playerStep = 1;
            else
                playerStep = 0;
    }
}

void Game::Render()
{
    system("cls");
    if (gamePhase == gpPlacing) {
        std::cout << "==== РЕЖИМ РАЗМЕЩЕНИЯ ====" << std::endl;
        std::cout << "разместите корабль (<координата> <поворот> <кол-во палуб>) (a4 right 2)" << std::endl;
        RenderGameArea(playerStep, playerAreas[playerStep], false);
    }
    else if (gamePhase == gpHitting) {
        std::cout << "==== РЕЖИМ ПОДБИТИЯ ====" << std::endl;
        std::cout << "подбейте корабль (координата) (a4)" << std::endl;
        int oppositePlayer = playerStep == 0 ? 1 : 0;
        RenderGameArea(playerStep, playerAreas[playerStep], false);
        std::cout << std::endl;
        RenderGameArea(oppositePlayer, playerAreas[oppositePlayer], true);
    }
    
}

void Game::RenderGameArea(int playerId, GameArea* gameArea, bool hide)
{
    std::array<std::string, 4> shipTypesNames{ "один", "два", "три", "четыре" };
    std::cout << playerId << "  a b c d e f g h i j" << std::endl;
    for (int y = 0; y < 10; y++) {
        std::cout << std::setw(2) << y + 1 << " ";
        for (int x = 0; x < 10; x++) {
            CellType cell = gameArea->GetCell({ x, y });
            //std::cout << (int)cell;
            if (cell == CellType::ctNone || cell == CellType::ctShipArea || (cell == CellType::ctShip && hide))
                std::cout << ". ";
            if (cell == CellType::ctMiss)
                std::cout << "M ";
            if (cell == CellType::ctHit)
                std::cout << "X ";
            if (cell == CellType::ctShip && !hide)
                std::cout << "S ";
            if (cell == CellType::ctSinked)
                std::cout << "# ";

        }
        if (y >= 0 && y < 4)
            std::cout << shipTypesNames[y] << ": " << gameArea->GetDockedShipsCount((ShipType)y);
        std::cout << std::endl;
    }
}
