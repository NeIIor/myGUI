// Button.hpp
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Widget.hpp"
#include <functional>
#include <string>

class Button : public Widget {
private:
    std::string label_;
    std::function<void()> onClick_;
    bool hovered_ = false;
    bool pressed_ = false;
    sf::Color normal_color_ = sf::Color::White;
    sf::Color hover_color_ = sf::Color(200, 200, 200);
    sf::Color press_color_ = sf::Color(150, 150, 150);
    sf::Font* font_ = nullptr;

public:
    Button(const std::string& label, sf::Font* font = nullptr);
    
    void onMouseMove(MouseMoveEvent&   event) override;
    void onMouseDown(MouseButtonEvent& event) override;
    void onMouseUp  (MouseButtonEvent& event) override;
    void onMouseEnter() override;
    void onMouseLeave() override;
    
    void render(sf::RenderWindow& window) override;
    
    void setOnClick(std::function<void()> callback) { onClick_ = std::move(callback); }
    void setLabel(const std::string& label) { label_ = label; }
    void setFont(sf::Font* font) { font_ = font; }
    
    void setNormalColor(const sf::Color& color) { normal_color_ = color; }
    void setHoverColor (const sf::Color& color) { hover_color_  = color; }
    void setPressColor (const sf::Color& color) { press_color_  = color; }
};

#endif // BUTTON_HPP