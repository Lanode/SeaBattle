#include "gamelogic.h"
#include <cmath>
#include <algorithm>
#include <iostream>

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

GameArea::GameArea()
{
	for (auto ship : dockedShips)
		totalShipCount += ship.second;
}

CellType GameArea::GetCell(Vector p)
{
	if (cells.contains(p))
		return cells.at(p);
	else
		return CellType::ctNone;
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
		if (GetCell(near_coords) == ctShip) {
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
			if (GetCell(near_coord) != ctShip) {
				shipAreaCells.push_back(near_coord);
			}
		}
	}
    return shipAreaCells;
}

int GameArea::GetDockedShipsCount()
{
	int DockedShipCount = 0;
	for (auto shipCount : dockedShips) {
		DockedShipCount += shipCount.second;
	}
	return DockedShipCount;
}

int GameArea::GetTotalShipCount()
{
    return totalShipCount;
}

PlaceResult GameArea::PlaceShip(Vector coords, Vector orientation, ShipType type)
{
	if (dockedShips[type] <= 0) 
		return PlaceResult::prAlreadyPlaced;

	for (int i = 0; i < type+1; i++)
	{
		Vector next_coord = coords + orientation * i;

		std::cout << next_coord.x << " " << next_coord.y <<std::endl;

		if (GetCell(next_coord) == ctShip || GetCell(next_coord) == ctShipArea)
			return PlaceResult::prForbidden;
		else {
			SetCell(next_coord, ctShip);
			dockedShips[type]--;
		}
	}

	return PlaceResult::prPlaced;
}

HitResult GameArea::HitShip(Vector p)
{
	if (GetCell(p) == ctShip) {
		SetCell(p, CellType::ctHit);
		
		std::vector<Vector> shipCells = GetShipCells(p);
		int struckCellsCount = 0;
		for (Vector cell : GetShipCells(p))
			if (GetCell(cell) == CellType::ctHit)
				struckCellsCount++;

		if (shipCells.size() <= struckCellsCount) {
			dockedShips[(ShipType)(shipCells.size()-1)] += 1;
			auto shipArea = GetShipAreaCells(p);
			for (Vector p_area : shipArea) {
				SetCell(p_area, CellType::ctHit);
			}
			return HitResult::hrSinked;
		} else
			return HitResult::hrStruck;
	} else if ((GetCell(p) == CellType::ctMiss) || (GetCell(p) == CellType::ctHit)) {
		return HitResult::hrForbidden;
	} else {
		return HitResult::hrMissed;
	}
}