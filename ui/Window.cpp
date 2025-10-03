// Window.cpp
#include "Window.hpp"
#include "Events.hpp"
#include <iostream>

Window::Window(const std::string& title, const sf::FloatRect& rect) 
    : title_(title) {
    setRect(rect);
    title_bar_ = sf::FloatRect(rect.left, rect.top, rect.width, 30);
}

void Window::onMouseMove(MouseMoveEvent& event) {
    if (dragging_) {
        setPosition(event.getPosition().x - drag_offset_.x, 
                   event.getPosition().y - drag_offset_.y);
        event.stopPropagation();
        return;
    }
    
     Widget::onMouseMove(event);
}

void Window::onMouseDown(MouseButtonEvent& event) {
    if (title_bar_.contains(event.getPosition())) {
        dragging_ = true;
        drag_offset_ = event.getPosition() - sf::Vector2f(getRect().left, getRect().top);
        event.stopPropagation();
        return;
    }
    
    Widget::onMouseDown(event);
}

void Window::onMouseUp(MouseButtonEvent& event) {
    dragging_ = false;
    Widget::onMouseUp(event);
}

void Window::render(sf::RenderWindow& window) {
    if (!isVisible()) return;
    
    sf::RectangleShape background(sf::Vector2f       (rect_.width, rect_.height));
                       background.setPosition        (rect_.left, rect_.top);
                       background.setFillColor       (sf::Color(40, 40, 50, 200));
                       background.setOutlineThickness(2);
                       background.setOutlineColor    (sf::Color::White);
           window.draw(background);
    
    sf::RectangleShape title_bar(sf::Vector2f(rect_.width, 30));
    title_bar.setPosition(rect_.left, rect_.top);
    title_bar.setFillColor(sf::Color(60, 60, 80));
    window.draw(title_bar);
    
    Widget::render(window);
}

void Window::updateLayout() {
    title_bar_ = sf::FloatRect(rect_.left, rect_.top, rect_.width, 30);
    Widget::updateLayout();
}