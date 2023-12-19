#include <vector>
#include <list>
#include <compare>
#include <map>

enum ShipType { One, Two, Three, Four };
enum CellType { None, Ship, ShipArea, Miss, Hit };
enum PlaceResult { Placed, Forbidden, AlreadyPlaced };
enum HitResult { Missed, Forbidden, Struck, Sinked };

struct Vector 
{
	int x;
	int y;

	auto operator <=> (const Vector&) const = default;
	bool operator == (Vector a);
	Vector operator+(Vector a);
	Vector operator*(int a);
	Vector Rotate(int degrees);
};

Vector Left = {-1, 0};
Vector Up = {0, -1};
Vector Right = {1, 0};
Vector Down = {0, 1};

class GameArea
{
private:
	std::map<Vector, CellType> cells;

public:
	std::map<ShipType, int> shipsNotOnField = {
		{One,   4},
		{Two,   3},
		{Three, 2},
		{Four,  1}
	};

	CellType GetCell(Vector p);
	void SetCell(Vector p, CellType type);

	std::vector<Vector> GetShipCells(Vector p);
	std::vector<Vector> GetShipAreaCells(Vector p);
	bool AllShipsInDocks();

	PlaceResult PlaceShip(Vector coords, Vector orientation, ShipType type);
	HitResult HitShip(Vector p);
};

class GameLogic
{
	GameArea player1Area;
	GameArea player2Area;
	GamePhase gamePhase = GamePhase::Placing;
	
	
};
