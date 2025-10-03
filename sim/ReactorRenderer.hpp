// ReactorRenderer.hpp
#ifndef REACTOR_RENDERER_HPP
#define REACTOR_RENDERER_HPP

#include "Reactor.hpp"
#include <SFML/Graphics.hpp>

class ReactorRenderer {
private:
    Reactor& reactor_;
    sf::RectangleShape reactor_bg_;
    sf::RectangleShape left_wall_, right_wall_, top_wall_, bottom_wall_;

public:
    ReactorRenderer    (Reactor& reactor);
    void updateGraphics();
    void render        (sf::RenderWindow& window);

private:
    sf::Color getLeftWallColor       () const;
    sf::Color getWallColorBasedOnHits() const;
    void drawMolecules               (sf::RenderWindow& window);
};

#endif // REACTOR_RENDERER_HPP