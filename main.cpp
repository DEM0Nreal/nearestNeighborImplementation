#include <SFML/Graphics.hpp>

#include <cmath>
#include <ctime>
#include <chrono>
#include <unordered_map>

const unsigned _window_width = 1000;
const unsigned _window_height = 800;
float dt;

constexpr double distance(float ax, float ay, float bx, float by){
	double a = abs(ax-bx);
	double b = abs(ay-by);
	double c = sqrt(pow(a, 2) + pow(b, 2));
	return c;
}

#include "positioned.hpp"
#include "bug.hpp"
#include "sgi.hpp"


int main(){
	auto previousTime = std::chrono::high_resolution_clock::now();
	srand(time(nullptr));

    sf::RenderWindow window(sf::VideoMode(_window_width, _window_height), "nearest neighbor - SFML");
	//window.setFramerateLimit(300);
	
	sgi grid(50, sf::Vector2i(_window_width, _window_height));

	std::vector<Bug*> bugs;

    while (window.isOpen()){
		auto currentTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - previousTime).count();
        previousTime = currentTime;

        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
			
			if (event.type == sf::Event::MouseButtonPressed){
				if (event.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2i mPos = sf::Mouse::getPosition(window);
					bugs.push_back(new Bug(mPos.x, mPos.y));
					grid.addElement(bugs.back());
				}
			}

			if (event.type == sf::Event::MouseWheelScrolled){
					for (auto bug : bugs){
						bug->adjustSpeed(event.mouseWheelScroll.delta * 10);
					}
			}
        }

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
			
					sf::Vector2i mPos = sf::Mouse::getPosition(window);
					bugs.push_back(new Bug(mPos.x, mPos.y));
					grid.addElement(bugs.back());
		}

		sf::Vector2i mPos = sf::Mouse::getPosition(window);
		Bug *mouseNeighbor = (Bug*)grid.nearestNeighbor(mPos.x, mPos.y);
		if (mouseNeighbor) mouseNeighbor->trigger();

		for (auto bug : bugs){
			bug->autoMove();
			bug->update();

			grid.updateElement(bug);
		}


        window.clear();
		
		for (auto bug : bugs){
			bug->draw(window);
		}

        window.display();
    }

	for (auto bug : bugs){
		delete bug;
	}

    return 0;
}
