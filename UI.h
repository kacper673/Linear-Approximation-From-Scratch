#ifndef UI_H
#define UI_H
#include <SFML/Graphics.hpp>
#include "aprox.h"
//Forward declaration
class ReferenceFrame;
class Points;
class LinearAproximation;
class LinearInterpolation;

class WindowManager{
public:
	sf::RenderWindow window;
	ReferenceFrame* reference_frame;
	Points* points;
	data* Data;
	LinearAproximation* LinearAprox;
	LinearInterpolation* LinearInter;

	WindowManager();
	~WindowManager();
	sf::Vector2u getSize();
	void run(int option);
};

class ReferenceFrame {
private:
	sf::Vertex x_axis[2];
	sf::Vertex y_axis[2];
	sf::RectangleShape graduation;
	std::vector<sf::RectangleShape> graduation_scale_x;
	std::vector<sf::RectangleShape> graduation_scale_y;
	sf::Font font;
	sf::Text text;
	std::vector<sf::Text> num_scale_x;
	std::vector<sf::Text> num_scale_y;
public:
	ReferenceFrame(WindowManager& Window, data& data);
	void draw(sf::RenderWindow& window);
};

class Points {
private:
	sf::CircleShape point;
	sf::Vector2f positionUI;
	std::vector <sf::Vector2f> positionsUI;

public:
	Points();
	void scale(sf::RenderWindow& window, data& data);
	void draw(sf::RenderWindow& Window, data& data);
};

class LinearAproximation {
private:
	sf::Vertex line[2];
public:
	void setLine(sf::RenderWindow& window, data& data);
	void draw(sf::RenderWindow& window);
};

class LinearInterpolation {
private:
	sf::Vertex line[2];
	std::vector<sf::Vertex> lines;
public:
	void setLine(sf::RenderWindow& window, data& data);
	void draw(sf::RenderWindow& window);
};
#endif