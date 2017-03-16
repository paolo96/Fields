#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

const int windowWidth = 800;
const int windowHeight = 600;

const double k0 = 8987551787.3681764;
const double PI = 3.14159265358979323846;
const double ARROW_ANGLE = PI/4;
const double COLOR_FADE = 0.6;

const int EL_FIELD_LINE_LENGHT_LIMIT = 8000;
const int EL_FIELD_LINE_PER_CHARGE = 20;
const int LINES_DEEP_REDUCTION = 35;
const int LINES_DRAWING_REDUCTION = 4;
const int ARROWS_REDUCTION = 100;
const int ARROW_SIZE = 10;
const int MARGIN_CHARGE_LINES = 6;
const int BUTTONS_SIZE = 80;

const sf::Color GREY_CHARGE(128, 128, 128);
const sf::Color RED_BUTTON_ON(255, 99, 71);
const sf::Color RED_BOUNDS_BUTTON_ON(255, 69, 0);
const sf::Color ORANGE_BUTTON_OFF(255, 165, 0);
const sf::Color ORANGE_BOUNDS_BUTTON_OFF(255, 140, 0);

extern int mouse_draw_size;
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

//Functions defined in sfml.cpp
bool checkForButtonsClick(int x, int y);
void leftButtonMousePress(sf::RenderWindow& window);
void paintingCustomCharge(sf::RenderWindow& window);
void keyboardPress(sf::RenderWindow& window, sf::Event& event);
void initializeConfig();


class Point {
	public:

		int x, y;
		double value;
		
		Point(int x, int  y);
		Point(int x, int  y, double value);

		bool operator<(const Point& temp) const;
		bool operator==(const Point& temp) const;
		
};

class Vector {
	public:

		int x, y;
		double intensity, alpha;

		Vector(int x, int y, double intensity, double alpha);
};

class Charge {
	public:

		int x, y, size;
		double value;

		Charge(int x, int y, int size, double value);
		virtual bool isPointInside(int tx, int ty) = 0;
		virtual double potInPoint(int tx, int ty) = 0;
		virtual void drawCharge(sf::RenderWindow& window) = 0;
		virtual Point externalPoint(double alpha) = 0;
		virtual Vector elFieldInPoint(int tx, int ty) = 0;
		virtual ~Charge();
};

class CircleCharge: public Charge {
    public:
    
		CircleCharge(int x, int y, int size, double value);
		bool isPointInside(int tx, int ty);
		double potInPoint(int tx, int ty);
		void drawCharge(sf::RenderWindow& window);
		Point externalPoint(double alpha);
		Vector elFieldInPoint(int tx, int ty);
};

class SquareCharge: public Charge {
    public:
    
		SquareCharge(int x, int y, int size, double value);
		bool isPointInside(int tx, int ty);
		double potInPoint(int tx, int ty);
		void drawCharge(sf::RenderWindow& window);
		Point externalPoint(double alpha);
		Vector elFieldInPoint(int tx, int ty);
};

class CustomCharge: public Charge {
    public:
    
		std::vector< Point > shape_map;
		std::vector< Point > external_points;
		
		CustomCharge(int x, int y, int size, double value);
		bool isPointInside(int tx, int ty);
		double potInPoint(int tx, int ty);
		void drawCharge(sf::RenderWindow& window);
		void findExternalPoints();
		Point externalPoint(double alpha);
		Vector elFieldInPoint(int tx, int ty);
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

		double maxElFieldIntensity;
		double minElFieldIntensity;

		std::vector< std::shared_ptr<Charge> > qCharges;
		std::vector< std::vector<double> > potIntensityMap;
		std::vector<Point> eqPotPoints;
		std::vector<Line> eqPotLines;
		std::vector<Line> elFieldLines;
		std::vector< std::vector<sf::Vertex> > arrows;
		std::vector< std::vector<double> > elFieldIntensityMap;
	
		Field();
		void update();
		void clear();

		//Equipotential lines related methods
		void mapPotential();
		double potValueInPoint(int x, int y);
		void findEquipotentialPoints();
		void findEquipotentialLines();

		//Electric Field lines related methods
		double angleFromPoints(double x1, double y1, double x2, double y2);
		bool insideCharges(double tx, double ty, int startCharge);
		Vector elFieldValueInPoint(int x, int y);
		Line buildElFieldLine(Point startField, int startCharge);
		void findElFieldLines();
		std::vector<sf::Vertex> buildElFieldArrowHead(double tx, double ty, double talpha);

		//Electric Field color related methods
		void mapElFieldIntensity();

		//Charge managing related methods
		void addCharge();
		void removeCharge(int x, int y);
		void selectCharge(int x, int y);
		//TODO implement moveCharge(...)
	
};

extern Field mainField;

//Functions defined in draws.cpp
void draw(sf::RenderWindow& window);
void drawField(sf::RenderWindow& window);
void drawCharges(sf::RenderWindow& window);
void drawButtons(sf::RenderWindow& window);
void drawPopUp(sf::RenderWindow& window);
