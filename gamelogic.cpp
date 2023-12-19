#include "gamelogic.h"
#include <cmath>

Vector Vector::operator+(Vector a)
{
	return {x+a.x, y+a.y};
}

Vector Vector::operator*(int a)
{
	return {x*a, y*a};
}

bool Vector::operator==(Vector a)
{
    return x == a.x && y == a.y;
}

Vector Vector::Rotate(int degrees)
{
    return {
		(int)(cos(degrees)*x - sin(degrees)*y), 
		(int)(sin(degrees)*x + cos(degrees)*y)
	};
}

CellType GameArea::GetCell(Vector p)
{
	if (cells.contains(p))
		return cells.at(p);
	else
		return CellType::None;
}

void GameArea::SetCell(Vector p, CellType type)
{
	cells[p] = type;
}

std::vector<Vector> GameArea::GetShipCells(Vector p)
{
	std::vector<Vector> shipCells;

	Vector rot;
	for (int d=0; d<=180; d+=90) {
		Vector near_coords = p.Rotate(d);
		if (GetCell(near_coords) == Ship) {
			rot = near_coords;
			break;
		}
	}

	Vector next_coords = p;
	int i = 0;
	while (GetCell(next_coords)) {
		shipCells.push_back(next_coords);
		next_coords = p + rot * i;
		i++;
	}

    return shipCells;
}

std::vector<Vector> GameArea::GetShipAreaCells(Vector p)
{
	std::vector<Vector> shipAreaCells;

	std::vector<Vector> shipCells = GetShipCells(p);
	for (Vector p : shipCells) {
		for (int d=0; d<=180; d+=90) {
			Vector near_coord = p.Rotate(d);
			if (GetCell(near_coord) != Ship) {
				shipAreaCells.push_back(near_coord);
			}
		}
	}
    return shipAreaCells;
}

bool GameArea::AllShipsInDocks()
{
	int NOFShipCount = 0;
	for (auto shipCount : gameArea.shipsNotOnField) {
		NOFShipCount += shipCount.second;
	}
	if (NOFShipCount == 0)
    return false;
}

PlaceResult GameArea::PlaceShip(Vector coords, Vector orientation, ShipType type)
{
	if (shipsNotOnField[type] <= 0) 
		return PlaceResult::AlreadyPlaced;

	for (int i = 0; i < type+1; i++)
	{
		Vector next_coord = coords + orientation * i;

		if (GetCell(next_coord) == Ship || GetCell(next_coord) == ShipArea)
			return PlaceResult::Forbidden;
		else {
			SetCell(next_coord, Ship);
			shipsNotOnField[type]--;
		}
	}

	return PlaceResult::Placed;
}

HitResult GameArea::HitShip(Vector p)
{
	if (GetCell(p) == Ship) {
		SetCell(p, CellType::Hit);
		
		std::vector<Vector> shipCells = GetShipCells(p);
		int struckCellsCount = 0;
		for (Vector cell : GetShipCells(p))
			if (GetCell(cell) == CellType::Hit)
				struckCellsCount++;

		if (shipCells.size() <= struckCellsCount) {
			shipsNotOnField[(ShipType)(shipCells.size()-1)] += 1;
			auto shipArea = GetShipAreaCells(p);
			for (Vector p_area : shipArea) {
				SetCell(p_area, CellType::Hit);
			}
			return HitResult::Sinked;
		} else
			return HitResult::Struck;
	} else if ((GetCell(p) == CellType::Miss) || (GetCell(p) == CellType::Hit)) {
		return HitResult::Forbidden;
	} else {
		return HitResult::Missed;
	}
}