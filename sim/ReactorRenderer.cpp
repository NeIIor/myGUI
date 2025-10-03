// ReactorRenderer.cpp
#include "ReactorRenderer.hpp"
#include <algorithm>

ReactorRenderer::ReactorRenderer(Reactor& reactor) : reactor_(reactor) {
    updateGraphics();
}

void ReactorRenderer::updateGraphics() {
    reactor_bg_.setSize(sf::Vector2f(reactor_.getReactorWidth(), reactor_.getReactorHeight()));
    reactor_bg_.setPosition(reactor_.getReactorX(), reactor_.getReactorY());
    reactor_bg_.setFillColor(sf::Color(50, 50, 70));

    left_wall_.setSize(sf::Vector2f(reactor_.getWallThickness(), reactor_.getReactorHeight()));
    left_wall_.setPosition(reactor_.getReactorX(), reactor_.getReactorY());
    left_wall_.setFillColor(getLeftWallColor());

    right_wall_.setSize(sf::Vector2f(reactor_.getWallThickness(), reactor_.getReactorHeight()));
    right_wall_.setPosition(reactor_.getReactorX() + reactor_.getReactorWidth() - reactor_.getWallThickness(), reactor_.getReactorY());
    right_wall_.setFillColor(getWallColorBasedOnHits());

    top_wall_.setSize(sf::Vector2f(reactor_.getReactorWidth(), reactor_.getWallThickness()));
    top_wall_.setPosition(reactor_.getReactorX(), reactor_.getReactorY());
    top_wall_.setFillColor(sf::Color::White);

    bottom_wall_.setSize(sf::Vector2f(reactor_.getReactorWidth(), reactor_.getWallThickness()));
    bottom_wall_.setPosition(reactor_.getReactorX(), reactor_.getReactorY() + reactor_.getReactorHeight() - reactor_.getWallThickness());
    bottom_wall_.setFillColor(sf::Color::White);
}

void ReactorRenderer::render(sf::RenderWindow& window) {
    window.draw(reactor_bg_);
    
    left_wall_.setFillColor(getLeftWallColor());
    window.draw(left_wall_);
    window.draw(right_wall_);
    window.draw(top_wall_);
    window.draw(bottom_wall_);

    drawMolecules(window);
}

sf::Color ReactorRenderer::getLeftWallColor() const {
    float temp = reactor_.getLeftWallTemperature();
    
    if (temp < 1.0f) {
        float factor = temp;
        return sf::Color(255 * factor, 255 * factor, 255);
    } else {
        float factor = std::min((temp - 1.0f) / 2.0f, 1.0f);
        return sf::Color(255, 255 * (1.0f - factor), 255 * (1.0f - factor));
    }
}

sf::Color ReactorRenderer::getWallColorBasedOnHits() const {
    int r = std::min(255, reactor_.getRightWallHits() * 5);
    return sf::Color(r, 50, 50);
}

void ReactorRenderer::drawMolecules(sf::RenderWindow& window) {
    for (const auto& mol : reactor_.getMolecules()) {
        if (!mol) continue;
        
        Vector2f pos = mol->getPosition();
        if (mol->getType() == MoleculeType::Round) {
            RoundMolecule* roundMol = dynamic_cast<RoundMolecule*>(mol.get());
            if (roundMol) {
                sf::CircleShape circle(roundMol->getRadius());
                circle.setFillColor(sf::Color::White);
                circle.setPosition(pos.getX(), pos.getY());
                circle.setOrigin(roundMol->getRadius(), roundMol->getRadius());
                window.draw(circle);
            }
        } else {
            SquareMolecule* squareMol = dynamic_cast<SquareMolecule*>(mol.get());
            if (squareMol) {
                sf::RectangleShape square(sf::Vector2f(squareMol->getSizeValue(), squareMol->getSizeValue()));
                square.setFillColor(sf::Color::Green);
                square.setPosition(pos.getX(), pos.getY());
                square.setOrigin(squareMol->getSizeValue()/2, squareMol->getSizeValue()/2);
                window.draw(square);
            }
        }
    }
}