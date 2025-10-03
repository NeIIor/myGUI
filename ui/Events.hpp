// Events.hpp
#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <SFML/Graphics.hpp>

class Widget;

enum class EventResult {
    CONSUME,
    PROPAGATE
};

class Event {
protected:
    bool bubbling_ = true;

public:
    virtual ~Event() = default;
    virtual EventResult apply(Widget* widget) = 0;
    
    void stopPropagation() { bubbling_ = false; }
    bool isBubbling() const { return bubbling_; }
};

class CoordEvent : public Event {
protected:
    sf::Vector2f position_;

public:
    CoordEvent(const sf::Vector2f& position) : position_(position) {}
    sf::Vector2f getPosition() const { return position_; }
};

class MouseMoveEvent : public CoordEvent {
public:
    MouseMoveEvent(const sf::Vector2f& position) : CoordEvent(position) {}
    EventResult apply(Widget* widget) override;
};

class MouseButtonEvent : public CoordEvent {
private:
    bool pressed_;
    int button_;

public:
    MouseButtonEvent(const sf::Vector2f& position, bool pressed, int button)
        : CoordEvent(position), pressed_(pressed), button_(button) {}
    
    bool isPressed() const { return pressed_; }
    int getButton() const { return button_; }
    EventResult apply(Widget* widget) override;
};

class IdleEvent : public Event {
public:
    IdleEvent() = default;
    EventResult apply(Widget* widget) override;
};

#endif // EVENTS_HPP