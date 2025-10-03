// main.cpp
#include <SFML/Graphics.hpp>
#include "ui/ReactorUI.hpp"
#include "sim/Reactor.hpp"

const int   WINDOW_WIDTH      = 1500;
const int   WINDOW_HEIGHT     = 900;
const int   REACTOR_X         = 50;
const int   REACTOR_Y         = 300;  
const int   REACTOR_WIDTH     = 500;
const int   REACTOR_HEIGHT    = 400;
const int   WALL_THICKNESS    = 10;
const float MOLECULE_RADIUS   = 1.f;
const float SQUARE_SIZE       = 2.f;
const int   INITIAL_MOLECULES = 1000;
const float MOLECULE_SPEED    = 100.f;

int main() {
    Reactor reactor(REACTOR_X, REACTOR_Y, REACTOR_WIDTH, REACTOR_HEIGHT, 
                   WALL_THICKNESS, MOLECULE_RADIUS, SQUARE_SIZE, MOLECULE_SPEED);
    
    for (int i = 0; i < INITIAL_MOLECULES; ++i) {
        reactor.addRoundMolecule();
    }
    
    ReactorUI reactor_ui(reactor);
    if (!reactor_ui.initialize()) {
        return -1;
    }
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Reactor Simulator");
    sf::Clock clock;
    
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            reactor_ui.handleEvent(event);
        }
        
        reactor.update(dt);
        reactor_ui.update(dt);
        
        window.clear(sf::Color(20, 20, 30));
        reactor_ui.render(window);
        window.display();
    }
    
    return 0;
}