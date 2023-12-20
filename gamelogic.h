#include <vector>
#include <compare>
#include <map>
#include <set>

enum ShipType { spOne, spTwo, spThree, spFour };
enum CellType { ctNone, ctShip, ctShipArea, ctMiss, ctHit, ctSinked };
enum PlaceResult { prPlaced, prForbidden, prAlreadyPlaced };
enum HitResult { hrMissed, hrForbidden, hrStruck, hrSinked };

struct Vector 
{
	int x;
	int y;

	auto operator <=> (const Vector&) const = default;
	bool operator == (Vector a);
	Vector operator+(Vector a);
	Vector operator*(int a);
	Vector operator-();
	Vector Rotate(int degrees);
};

const Vector GAME_FIELD_SIZE {10, 10};

/*
Vector vecLeft {-1, 0};
Vector vecUp {0, -1};
Vector vecRight {1, 0};
Vector vecDown {0, 1};
*/

class GameArea
{
private:
	std::map<Vector, CellType> cells;
	std::map<ShipType, int> dockedShips = {
		{spOne,   4},
		{spTwo,   3},
		{spThree, 2},
		{spFour,  1}
	};
	int totalShipCount = 0;

public:
	GameArea();

	CellType GetCell(Vector p);
	void SetCell(Vector p, CellType type);

	std::set<Vector> GetShipCells(Vector p);
	std::set<Vector> GetShipAreaCells(Vector p);
	int GetDockedShipsCount();
	int GetDockedShipsCount(ShipType type);
	int GetTotalShipCount();

	PlaceResult PlaceShip(Vector coords, Vector orientation, ShipType type);
	HitResult HitShip(Vector p);
};
