// ClockWidget.hpp
#ifndef CLOCK_WIDGET_HPP
#define CLOCK_WIDGET_HPP

#include "Widget.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <iomanip>
#include <sstream>

class ClockWidget : public Widget {
private:
    sf::Text time_text_;
    sf::Font* font_;
    sf::Clock clock_;
    bool show_milliseconds_ = false;

public:
    ClockWidget(sf::Font* font = nullptr, bool show_ms = false);
    
    void onIdle() override;
    void render(sf::RenderWindow& window) override;
    
    void setTimeFormat(bool show_ms) { show_milliseconds_ = show_ms; }
    void setTextColor(const sf::Color& color) { time_text_.setFillColor(color); }
    void setCharacterSize(unsigned int size) { time_text_.setCharacterSize(size); }
    
    std::string getCurrentTimeString() const;

private:
    void updateTimeDisplay();
};

#endif // CLOCK_WIDGET_HPP