#include "gamelogic.h"
#include <cmath>
#include <numbers>
#include <algorithm>
#include <iostream>

inline double round_away(double x)
{
	return x < 0 ? floor(x) : ceil(x);
}

Vector Vector::operator+(Vector a)
{
	return {x+a.x, y+a.y};
}

Vector Vector::operator*(int a)
{
	return {x*a, y*a};
}

Vector Vector::operator-()
{
	return {-x, -y};
}

bool Vector::operator==(Vector a)
{
    return x == a.x && y == a.y;
}

Vector Vector::Rotate(int degrees)
{
	double rad = degrees * std::numbers::pi / 180;
    return {
		(int)(cos(rad)*x - sin(rad)*y),
		(int)(sin(rad)*x + cos(rad)*y)
	};
}

GameArea::GameArea()
{
	for (auto ship : dockedShips)
		totalShipCount += ship.second;
}

CellType GameArea::GetCell(Vector p)
{
	if (this->cells.contains(p))
		return this->cells.at(p);
	else
		return CellType::ctNone;
}

void GameArea::SetCell(Vector p, CellType type)
{
	this->cells[p] = type;
}

std::set<Vector> GameArea::GetShipCells(Vector p)
{
	std::set<Vector> shipCells;

	Vector rot;
	for (int d=0; d<=360; d+=90) {
		Vector r = Vector{ -1,0 }.Rotate(d);
		Vector near_coords = p + r;
		if (GetCell(near_coords) == ctShip || GetCell(near_coords) == ctHit) {
			rot = r;
			break;
		}
	}

	Vector next_coords = p;
	int i = 0;
	while (GetCell(next_coords) == ctShip || GetCell(next_coords) == ctHit) {
		shipCells.insert(next_coords);
		next_coords = p + rot * i;
		i++;
	}
	next_coords = p;
	i = 0;
	while (GetCell(next_coords) == ctShip || GetCell(next_coords) == ctHit) {
		shipCells.insert(next_coords);
		next_coords = p + (-rot) * i;
		i++;
	}

    return shipCells;
}

std::set<Vector> GameArea::GetShipAreaCells(Vector p)
{
	std::set<Vector> shipAreaCells;

	std::set<Vector> shipCells = GetShipCells(p);
	for (Vector p : shipCells) {
		for (int x=-1; x<=1; x++)
			for (int y = -1; y <= 1; y++) {
				Vector near_coord = p + Vector{x,y};
				if (GetCell(near_coord) != ctShip && GetCell(near_coord) != ctHit) {
					shipAreaCells.insert(near_coord);
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

int GameArea::GetDockedShipsCount(ShipType type)
{
	return dockedShips[type];
}

int GameArea::GetTotalShipCount()
{
    return totalShipCount;
}

PlaceResult GameArea::PlaceShip(Vector coords, Vector orientation, ShipType type)
{
	if (dockedShips[type] <= 0) 
		return PlaceResult::prAlreadyPlaced;

	std::set<Vector> shipCells;
	for (int i = 0; i < type+1; i++)
	{
		Vector next_coord = coords + orientation * i;

		//std::cout << next_coord.x << " " << next_coord.y <<std::endl;

		if (GetCell(next_coord) == ctShip || GetCell(next_coord) == ctShipArea)
			return PlaceResult::prForbidden;
		else {
			shipCells.insert(next_coord);
		}
	}

	for (Vector p : shipCells) {
		SetCell(p, ctShip);
	}

	auto shipArea = GetShipAreaCells(coords);
	for (Vector p : shipArea) {
		SetCell(p, ctShipArea);
	}

	dockedShips[type]--;
	return PlaceResult::prPlaced;
}

HitResult GameArea::HitShip(Vector p)
{
	if (GetCell(p) == ctShip) {
		SetCell(p, CellType::ctHit);
		
		std::set<Vector> shipCells = GetShipCells(p);
		int struckCellsCount = 0;
		for (Vector cell : GetShipCells(p))
			if (GetCell(cell) == CellType::ctHit)
				struckCellsCount++;

		if (shipCells.size() <= struckCellsCount) {
			dockedShips[(ShipType)(shipCells.size()-1)] += 1;
			auto shipArea = GetShipAreaCells(p);
			for (Vector p_area : shipArea) {
				SetCell(p_area, CellType::ctSinked);
			}
			auto shipCells = GetShipCells(p);
			for (Vector p_ship : shipCells) {
				SetCell(p_ship, CellType::ctSinked);
			}
			return HitResult::hrSinked;
		} else
			return HitResult::hrStruck;
	} else if (GetCell(p) == CellType::ctMiss || 
			   GetCell(p) == CellType::ctHit  || 
		       GetCell(p) == CellType::ctSinked) {
		return HitResult::hrForbidden;
	} else {
		SetCell(p, ctMiss);
		return HitResult::hrMissed;
	}
}