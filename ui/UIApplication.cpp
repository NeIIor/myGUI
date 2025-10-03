// UIApplication.cpp
#include "UIApplication.hpp"
#include "Widget.hpp"
#include "Events.hpp"

UIApplication::UIApplication() = default;

UIApplication::~UIApplication() = default;

void UIApplication::setRoot(std::unique_ptr<Widget> root) {
    root_ = std::move(root);
}

void UIApplication::handleEvent(const sf::Event& sfml_event) {
    if (!root_) return;

    Widget* new_hovered = nullptr;

    switch (sfml_event.type) {
        case sf::Event::MouseMoved:
            setPointerPosition({static_cast<float>(sfml_event.mouseMove.x), 
                              static_cast<float>(sfml_event.mouseMove.y)});
            
            new_hovered = root_->getPointerTarget(getPointerPosition());
            
            if (hovered_widget_ != new_hovered) {
                if (hovered_widget_) {
                    hovered_widget_->onMouseLeave();
                }
                if (new_hovered) {
                    new_hovered->onMouseEnter();
                }
                hovered_widget_ = new_hovered;
            }
            
            if (new_hovered) {
                MouseMoveEvent event(getPointerPosition());
                event.apply(new_hovered);
            }
            break;
            
        case sf::Event::MouseButtonPressed:
            if (sfml_event.mouseButton.button == sf::Mouse::Left) {
                setPointerPressed(true);
                setPointerPosition({static_cast<float>(sfml_event.mouseButton.x), 
                                  static_cast<float>(sfml_event.mouseButton.y)});
                
                Widget* click_target = root_->getPointerTarget(getPointerPosition());
                if (click_target) {
                    MouseButtonEvent event(getPointerPosition(), true, 0);
                    event.apply(click_target);
                    setTarget(click_target);
                }
            }
            break;
            
        case sf::Event::MouseButtonReleased:
            if (sfml_event.mouseButton.button == sf::Mouse::Left) {
                setPointerPressed(false);
                setPointerPosition({static_cast<float>(sfml_event.mouseButton.x), 
                                  static_cast<float>(sfml_event.mouseButton.y)});
                
                if (target_) {
                    MouseButtonEvent event(getPointerPosition(), false, 0);
                    event.apply(target_);
                }
            }
            break;
            
        default:
            break;
    }
}

void UIApplication::update(float dt) {
    if (!root_) return;
    
    idle_timer_ += dt;
    if (idle_timer_ >= 1.0f) {
        IdleEvent event;
        event.apply(root_.get());
        idle_timer_ = 0.f;
    }
}

void UIApplication::render(sf::RenderWindow& window) {
    if (root_) {
        root_->render(window);
    }
}