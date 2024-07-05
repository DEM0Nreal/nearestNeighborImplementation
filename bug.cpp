Bug::Bug(float x, float y){
	this->posX = x;
	this->posY = y;

	this->body.setRadius(5.f);

	this->body.setFillColor(sf::Color::Red);
	this->body.setOrigin(2.5, 2.5);

	this->direction = (double)(rand()%7000)/1000;

	this->speed = 200;
}

void Bug::trigger(){
	this->triggered = 1;
}

void Bug::autoMove(){
	this->posX = this->posX + this->speed * cos(direction) * dt;
	this->posY = this->posY + this->speed * -sin(direction) * dt;

	this->direction += ((double)(rand()%100-50)/100.f) * dt * 500;
}

void Bug::setSpeed(double newSpeed){
	this->speed = newSpeed;
}

void Bug::adjustSpeed(double deltaSpeed){
	this->speed += deltaSpeed;
	if (this->speed < 0) this->speed = 0;
}

void Bug::update(){
	if (this->triggered == 2){
		this->triggered = 0;
		this->body.setFillColor(sf::Color::Red);
	}
	else if (this->triggered == 1){
		this->body.setFillColor(sf::Color::Green);
		++this->triggered;
	}

	if (this->posX < 0) {
		this->posX = 0;
		direction = 2 * 3*M_PI/2 - direction;
	}
	if (this->posY < 0) {
		this->posY = 0;
		direction = 2 * 0 - direction;
	}

	if (this->posX >= _window_width){
		this->posX = _window_width-1;
		direction = 2 * M_PI/2 - direction;
	}

	if (this->posY >= _window_height){
		this->posY = _window_height-1;
		direction = 2 * M_PI - direction;
	}

	this->body.setPosition(this->posX, this->posY);
}

void Bug::draw(sf::RenderWindow &window){
	window.draw(this->body);
}
