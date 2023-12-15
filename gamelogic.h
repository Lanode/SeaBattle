#include <vector>

// Definition

enum Direction { Left, Up, Right, Down };
enum Type { One, Two, Three, Four };
enum CellType { None, Ship, ShipArea, Miss, Hit };
//enum PlaceReturn { Forbidden, Hit };
enum HitResult { Missed, Forbidden, Struck, Sinked };

struct Coordinate 
{
	int x;
	int y;
};

struct ShipInfo
{
	Type type;
	Coordinate p;
	Direction direction;
};

class PlayerSpace
{
private:
	std::vector<std::vector<CellType>> map;
	std::vector<ShipInfo> ships;

public:
	Coordinate GetShipNextCoord(ShipInfo ship, int i); // по правильному надо перенести в ShipInfo

	void Init();

	bool PlaceShip(ShipInfo ship);

	HitResult HitShip(Coordinate p);
};

class GameLogic
{

};
