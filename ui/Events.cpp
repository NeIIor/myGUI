// Events.cpp
#include "Events.hpp"
#include "Widget.hpp"

EventResult MouseMoveEvent::apply(Widget* widget) {
    if (!widget) return EventResult::PROPAGATE;
    
    widget->onMouseMove(*this);
    return bubbling_ ?  EventResult::PROPAGATE : EventResult::CONSUME;
}

EventResult MouseButtonEvent::apply(Widget* widget) {
    if (!widget) return EventResult::PROPAGATE;
    
    if (pressed_) {
        widget->onMouseDown(*this);
    } else {
        widget->onMouseUp (*this);
    }
    return bubbling_ ? EventResult::PROPAGATE : EventResult::CONSUME;
}

EventResult IdleEvent::apply(Widget* widget) {
    if (!widget) return EventResult::PROPAGATE;
    
    widget->onIdle();
    return bubbling_ ? EventResult::PROPAGATE : EventResult::CONSUME;
}