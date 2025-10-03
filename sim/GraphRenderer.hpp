// GraphRenderer.hpp
#ifndef GRAPH_RENDERER_HPP
#define GRAPH_RENDERER_HPP

#include "Reactor.hpp"
#include <SFML/Graphics.hpp>
#include <deque>     
#include <string>    
#include <algorithm>  

class GraphRenderer {
private:
    Reactor& reactor_;
    sf::RectangleShape background_;
    sf::Font* font_;

public:
    GraphRenderer   (Reactor& reactor, sf::Font* font = nullptr);
    void setPosition(float x, float y);
    void setSize    (float width, float height);
    void render     (sf::RenderWindow& window);

private:
    template<typename T>
    void drawGraph(sf::RenderWindow& window,      const std::deque<T>& data, 
                   sf::Color color, float y_top,  const std::string& label, 
                                    float x_step, const sf::Vector2f& graph_pos);
};

#endif // GRAPH_RENDERER_HPP