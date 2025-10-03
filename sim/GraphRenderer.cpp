// GraphRenderer.cpp
#include "GraphRenderer.hpp"
#include <algorithm>

GraphRenderer::GraphRenderer(Reactor& reactor, sf::Font* font) 
    : reactor_(reactor), font_(font) {
    background_.setFillColor(sf::Color(30, 30, 30, 200));
    background_.setSize(sf::Vector2f(300, 500));
    background_.setPosition(1150, 200);
}

void GraphRenderer::setPosition(float x, float y) {
    background_.setPosition(x, y);
}

void GraphRenderer::setSize(float width, float height) {
    background_.setSize(sf::Vector2f(width, height));
}

void GraphRenderer::render(sf::RenderWindow& window) {
    window.draw(background_);

    const int GRAPH_HEIGHT = 40;
    const int PADDING = 10;
    
    size_t maxHistorySize = std::max({
        reactor_.getMoleculeHistory()      .size(),
        reactor_.getRoundMoleculeHistory() .size(),
        reactor_.getSquareMoleculeHistory().size(),
        reactor_.getEnergyHistory()        .size(),
        reactor_.getTemperatureHistory()   .size()
    });
    
    float xStep = (maxHistorySize <= 1) ? 0.0f : 
                 (background_.getSize().x - 2 * PADDING) / static_cast<float>(maxHistorySize - 1);
    
    sf::Vector2f graph_pos = background_.getPosition();
    
    drawGraph(window, reactor_.getMoleculeHistory      (), sf::Color::Cyan,   graph_pos.y + 30 , "Total",  xStep, graph_pos);
    drawGraph(window, reactor_.getRoundMoleculeHistory (), sf::Color::White,  graph_pos.y + 80 , "Round",  xStep, graph_pos);
    drawGraph(window, reactor_.getSquareMoleculeHistory(), sf::Color::Green,  graph_pos.y + 130, "Square", xStep, graph_pos);
    drawGraph(window, reactor_.getEnergyHistory        (), sf::Color::Yellow, graph_pos.y + 180, "Energy", xStep, graph_pos);
    drawGraph(window, reactor_.getTemperatureHistory   (), sf::Color::Red,    graph_pos.y + 230, "Temp",   xStep, graph_pos);
}

template<typename T>
void GraphRenderer::drawGraph(sf::RenderWindow& window, const std::deque<T>& data, 
               sf::Color color, float y_top, const std::string& label, 
               float x_step, const sf::Vector2f& graph_pos) {
    if (data.empty()) return;

    const int PADDING = 10;
    const int GRAPH_HEIGHT = 40;

    T max_val = *std::max_element(data.begin(), data.end());
    if (max_val == 0) max_val = 1;

    sf::VertexArray lines(sf::LineStrip);

    for (size_t i = 0; i < data.size(); ++i) {
        float x = graph_pos.x + PADDING + i * x_step; 
        float y = y_top + GRAPH_HEIGHT - (static_cast<float>(data[i]) / max_val) * GRAPH_HEIGHT;
        lines.append(sf::Vertex(sf::Vector2f(x, y), color));
    }

    window.draw(lines);

    if (font_) {
        sf::Text text(label, *font_, 10);
        text.setPosition(graph_pos.x + 10, y_top - 15);
        text.setFillColor(color);
        window.draw(text);
    }
}