class Bug : public Positioned{
private:
	sf::CircleShape body;

	double direction; // radian θ
	double speed;

	int triggered = 0;

public:
	Bug(float, float);

	void trigger();

	void autoMove();

	void setSpeed(double newSpeed);

	void adjustSpeed(double deltaSpeed);

	void update();

	void draw(sf::RenderWindow&);
};

#include "bug.cpp"
