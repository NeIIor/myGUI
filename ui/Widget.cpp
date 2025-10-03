// Widget.cpp
#include "Widget.hpp"
#include "Events.hpp"
#include <algorithm>

bool Widget::contains(const sf::Vector2f& point) const {
    return rect_.contains(point);
}

Widget* Widget::getPointerTarget(const sf::Vector2f& point) {
    if (!contains(point) || !visible_) return nullptr;
    
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        auto& child = *it;
        if (child->isVisible()) {
            if (auto target = child->getPointerTarget(point)) {
                return target;
            }
        }
    }
    
    return this;
}

void Widget::onMouseMove(MouseMoveEvent& event) {
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        auto& child = *it;
        if (child->isVisible() && child->contains(event.getPosition())) {
            child->onMouseMove(event);
            if (!event.isBubbling()) return;
        }
    }
}

void Widget::onMouseDown(MouseButtonEvent& event) {
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        auto& child = *it;
        if (child->isVisible() && child->contains(event.getPosition())) {
            child->onMouseDown(event);
            if (!event.isBubbling()) return;
        }
    }
}

void Widget::onMouseUp(MouseButtonEvent& event) {
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        auto& child = *it;
        if (child->isVisible() && child->contains(event.getPosition())) {
            child->onMouseUp(event);
            if (!event.isBubbling()) return;
        }
    }
}

void Widget::onIdle() {
    for (auto& child : children_) {
        child->onIdle();
    }
}

void Widget::onMouseEnter() {
}

void Widget::onMouseLeave() {
}

void Widget::render(sf::RenderWindow& window) {
    if (!visible_) return;
    
    for (auto& child : children_) {
        child->render(window);
    }
}

void Widget::addChild(std::unique_ptr<Widget> child) {
    child->parent_ = this;
    children_.push_back(std::move(child));
    
    std::sort(children_.begin(), children_.end(), 
              [](const auto& a, const auto& b) { 
                  return a->getZOrder() < b->getZOrder(); 
              });
}

void Widget::removeChild(Widget* child) {
    children_.erase(std::remove_if(children_.begin(), children_.end(),
        [child](const auto& ptr) { return ptr.get() == child; }), children_.end());
}

void Widget::setPosition(float x, float y) {
    rect_.left = x;
    rect_.top = y;
    updateLayout();
}

void Widget::setSize(float width, float height) {
    rect_.width = width;
    rect_.height = height;
    updateLayout();
}

void Widget::setRect(const sf::FloatRect& rect) {
    rect_ = rect;
    updateLayout();
}

void Widget::updateLayout() {
}