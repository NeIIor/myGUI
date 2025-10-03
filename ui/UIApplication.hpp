// UIApplication.hpp
#ifndef UI_APPLICATION_HPP
#define UI_APPLICATION_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Widget.hpp"

class UIApplication {
private:
    std::unique_ptr<Widget> root_;
    Widget* target_ = nullptr;
    Widget* hovered_widget_ = nullptr;  
    sf::Vector2f pointer_position_;
    bool pointer_pressed_ = false;
    float idle_timer_ = 0.f; 

public:
     UIApplication();
    ~UIApplication();

    void setRoot(std::unique_ptr<Widget> root);
    Widget* getRoot  () const { return root_.get(); }
    Widget* getTarget() const { return target_; }
    void setTarget(Widget* target) { target_ = target; }
    
    Widget* getHoveredWidget() const { return hovered_widget_; }
    void setHoveredWidget(Widget* widget) { hovered_widget_ = widget; }
    
    sf::Vector2f getPointerPosition() const { return pointer_position_; }
    void         setPointerPosition(  const sf::Vector2f& pos) { pointer_position_ = pos; }
    bool         isPointerPressed  () const { return pointer_pressed_;  }
    void         setPointerPressed (bool pressed) { pointer_pressed_ = pressed; }

    void handleEvent(const sf::Event& sfml_event);
    void update(float dt); 
    void render(sf::RenderWindow& window);
};

#endif