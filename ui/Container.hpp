// Container.hpp
#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "Widget.hpp"

class Container : public Widget {
public:
    Container() = default;
    
    bool    contains        (const sf::Vector2f& point) const override;
    Widget* getPointerTarget(const sf::Vector2f& point)       override;
    
    void onMouseMove(MouseMoveEvent&   event) override;
    void onMouseDown(MouseButtonEvent& event) override;
    void onMouseUp  (MouseButtonEvent& event) override;
    
    void render    (sf::RenderWindow& window) override;

protected:
    void updateLayout() override;
};

#endif // CONTAINER_HPP