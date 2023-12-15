#include "gamelogic.h"

Coordinate PlayerSpace::GetShipNextCoord(ShipInfo ship, int i) {
    Coordinate p = ship.p;
	switch (ship.direction) {
	case Left:
		p.x -= i;
		break;
	case Up:
		p.y -= i;
		break;
	case Right:
		p.x += i;
		break;
	case Down:
		p.y += i;
		break;
	}
	return ship.p;
}

void PlayerSpace::Init()
{
	std::vector<std::vector<CellType>> map2(10, std::vector<CellType>(10));
	this->map = map2;
}

bool PlayerSpace::PlaceShip(ShipInfo ship)
{
	for (int i = 0; i < ship.type; i++)
	{
		Coordinate p = GetShipNextCoord(ship, i);

		if ((map[p.x][p.y] == Ship) || (map[p.x][p.y] == ShipArea))
			return false;
		else
			map[p.x][p.y] = Ship;
	}

	ships.push_back(ship);

	return true;
}

HitResult PlayerSpace::HitShip(Coordinate p)
{
	if (map[p.x][p.y] == Ship) {
		map[p.x][p.y] = CellType::Hit;
		// сделать проверку на затопление пройдясь по всем координатам корабля и тогда вернуть Sinked а пока что:
		return HitResult::Struck;
	} else if ((map[p.x][p.y] == Miss) || (map[p.x][p.y] == Hit)) {
		return HitResult::Forbidden;
	} else {
		return HitResult::Missed;
	}
}