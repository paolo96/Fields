#include <vector>

extern const int windowWidth;
extern const int windowHeight;

class Charge {
	public:

	int x, y, size;
	double value;

	Charge(int x, int y, int size, double value);
};

class Point {
	public:
	int x, y;
	double value;

	Point(int x, int  y, double value);

	bool operator<(const Point& temp) const;
};


class Line {
	public:

	std::vector<Point> pointsCoord;
};

class Field {
	public:

	const double k0 = 8987551787.3681764;
	const int linesDeepReduction = 35;
	const int linesPerCharge = 20;

	std::vector<Charge> qCharges;
	std::vector< std::vector<double> > potIntensityMap;
	std::vector<Point> eqPotPoints;
	std::vector<Line> eqPotLines;

	Field();
	void mapField();
	double potValueInPoint(int x, int y);
	void findEquipotentialPoints();
	void findEquipotentialLines();
	
};

