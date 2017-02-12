#include <SFML/Graphics.hpp>
#include <vector>

extern const int windowWidth;
extern const int windowHeight;

extern bool showing_pop_up;
extern int selected_index;

void draw(sf::RenderWindow& window);
void initializeConfig();
bool checkForButtonsClick(int x, int y);

class Charge {
	public:

	int x, y, size;
	double value;

    Charge(int x, int y, int size, double value);
    bool isPointInside(int tx, int ty);
};

class Point {
	public:
	int x, y;
	double value;

	Point(int x, int  y, double value);

	bool operator<(const Point& temp) const;
};

class Button {
    public:
    
    int x, y, width, height;
    bool checked;
    std::string name;
    
    Button(std::string name, int x, int y, int width, int height, bool checked);
    bool isPointInside(int tx, int ty);
    
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
    void addCharge(Charge c);
    void removeCharge(int x, int y);
    void selectCharge(int x, int y);
    void clear();
	
};
