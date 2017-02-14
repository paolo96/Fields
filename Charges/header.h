#include <SFML/Graphics.hpp>
#include <vector>

extern const int windowWidth;
extern const int windowHeight;

extern bool showing_pop_up;
extern int selected_index;

const std::string CIRCLE_SHAPE = "CIRCLE_SHAPE";
const std::string SQUARE_SHAPE = "SQUARE_SHAPE";
const std::string CUSTOM_SHAPE = "CUSTOM_SHAPE";

void draw(sf::RenderWindow& window);
void drawCharges(sf::RenderWindow& window);
void drawButtons(sf::RenderWindow& window);
void drawPopUp(sf::RenderWindow& window);
void initializeConfig();
bool checkForButtonsClick(int x, int y);

class Charge {
	public:

    std::string shape;
	int x, y, size;
	double value;

    bool isPointInside(int tx, int ty);
    Charge(std::string shape, int x, int y, int size, double value);
};

class CustomCharge: public Charge {
    public:
    
    std::vector< std::vector<bool> > shape_map;
    
    CustomCharge(int x, int y, int size, double value);
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

