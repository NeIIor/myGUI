// Container.cpp
#include "Container.hpp"
#include "Events.hpp"

bool Container::contains(const sf::Vector2f& point) const {
    return Widget::contains(point);
}

Widget* Container::getPointerTarget(const sf::Vector2f& point) {
    return Widget::getPointerTarget(point);
}

void Container::onMouseMove(MouseMoveEvent&   event) {
    Widget::onMouseMove(event);
}

void Container::onMouseDown(MouseButtonEvent& event) {
    Widget::onMouseDown(event);
}

void Container::onMouseUp  (MouseButtonEvent& event) {
    Widget::onMouseUp(event);
}

void Container::render     (sf::RenderWindow& window) {
    Widget::render(window);
}

void Container::updateLayout() {
    Widget::updateLayout();
}