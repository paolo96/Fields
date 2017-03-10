#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

extern const int windowWidth;
extern const int windowHeight;

const std::string CIRCLE_SHAPE = "CIRCLE_SHAPE";
const std::string SQUARE_SHAPE = "SQUARE_SHAPE";
const std::string CUSTOM_SHAPE = "CUSTOM_SHAPE";

const double k0 = 8987551787.3681764;
const double PI = 3.14159265358979323846;

const int linesDeepReduction = 35;
const int linesPerCharge = 20;
const int lineDrawingReduction = 4;
const int buttonsSize = 80;

extern int selected_index;
extern bool inputing_size;
extern bool inputing_custom;
extern bool showing_pop_up;
extern bool adding_charge;
extern bool removing_charge;
extern bool selecting_charge;
extern bool isEqPotLines;
extern bool isElFieldLines;
extern bool isElFieldColor;
extern bool shapingCircle;
extern bool shapingSquare;
extern bool shapingCustom;

extern sf::Font font;
extern sf::RectangleShape pop_up_rect;

extern std::string size_input;
extern std::string value_input;

bool checkForButtonsClick(int x, int y);
void initializeConfig();

class Charge {
	public:

    std::string shape;
	int x, y, size;
	double value;

    Charge(std::string shape, int x, int y, int size, double value);
    virtual bool isPointInside(int tx, int ty);
    virtual ~Charge();
};

class Point {
	public:

	int x, y;
	double value;
    
    Point(int x, int  y);
    Point(int x, int  y, double value);

	bool operator<(const Point& temp) const;
};

class Vector {
	public:

	int x, y;
	double intensity, alpha;

	Vector(int x, int y, double intensity, double alpha);
};

class SquareCharge: public Charge {
    public:
    
    SquareCharge(int x, int y, int size, double value);
    bool isPointInside(int tx, int ty);
};

class CustomCharge: public Charge {
    public:
    
    std::vector< Point > shape_map;
    
    CustomCharge(int x, int y, int size, double value);
    bool isPointInside(int tx, int ty);
};

extern std::shared_ptr<CustomCharge> currentCustom;

class Button {
    public:
    
    int x, y, width, height;
    bool checked;
    std::string name;
    
    Button(std::string name, int x, int y, int width, int height, bool checked);
    bool isPointInside(int tx, int ty);
    
};

extern std::vector<Button> buttons;

class Line {
	public:

	std::vector<Point> pointsCoord;
};

class Field {
	public:

    std::vector< std::shared_ptr<Charge> > qCharges;

	std::vector< std::vector<double> > potIntensityMap;
	std::vector<Point> eqPotPoints;
	std::vector<Line> eqPotLines;

	std::vector<Line> elFieldLines;

	Field();
	void mapField();

	//Potential related methods
	double potValueInPoint(int x, int y);
	void findEquipotentialPoints();
    void findEquipotentialLines();

	//Electric Field related methods
	double angleFromPoints(double x1, double y1, double x2, double y2);
	bool insideCharges(double tx, double ty, int startCharge);
	Vector elFieldValueInPoint(int x, int y);
	Line buildElFieldLine(Point startField, int startCharge);
	void findElFieldLines();

	//Charge managing related methods
    void addCharge();
    void removeCharge(int x, int y);
    void selectCharge(int x, int y);

    void clear();
	
};

void draw(sf::RenderWindow& window, Field& mainField);
void drawCharges(sf::RenderWindow& window, Field& mainField);
void drawButtons(sf::RenderWindow& window);
void drawPopUp(sf::RenderWindow& window, Field& mainField);
