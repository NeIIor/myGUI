#include "Button.hpp"
#include "Events.hpp"
#include <iostream>

Button::Button(const std::string& label, sf::Font* font) 
    : label_(label), font_(font) {
    setSize(100, 30);
}

void Button::onMouseMove(MouseMoveEvent& event) {
    bool nowHovered = contains(event.getPosition());
    
    if (nowHovered != hovered_) {
        if (nowHovered) {
            onMouseEnter();
        } else {
            onMouseLeave();
        }
    }
}

void Button::onMouseDown(MouseButtonEvent& event) {
    if (hovered_ && event.isPressed()) {
        pressed_ = true;
        event.stopPropagation();
    }
}

void Button::onMouseUp(MouseButtonEvent& event) {
    if (pressed_ && hovered_ && !event.isPressed() && onClick_) {
        onClick_();
    }
    pressed_ = false;
}

void Button::onMouseEnter() {
    hovered_ = true;
}

void Button::onMouseLeave() {
    hovered_ = false;
    pressed_ = false; 
}

void Button::render(sf::RenderWindow& window) {
    if (!isVisible()) return;
    
    sf::RectangleShape shape(sf::Vector2f(rect_.width, rect_.height));
    shape.setPosition(rect_.left, rect_.top);
    
    if (pressed_ && hovered_) {
        shape.setFillColor(press_color_);
    } else if (hovered_) {
        shape.setFillColor(hover_color_);
    } else {
        shape.setFillColor(normal_color_);
    }
    
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);
    
    window.draw(shape);
    
    if (font_ && !label_.empty()) {
        sf::Text text(label_, *font_, 14);
        text.setFillColor(sf::Color::Black);
        
        sf::FloatRect text_bounds = text.getLocalBounds();
        text.setPosition(rect_.left + (rect_.width - text_bounds.width) / 2,
                         rect_.top + (rect_.height - text_bounds.height) / 2 - 5);
        
        window.draw(text);
    }
}