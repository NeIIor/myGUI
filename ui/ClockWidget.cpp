// ClockWidget.cpp
#include "ClockWidget.hpp"

ClockWidget::ClockWidget(sf::Font* font, bool show_ms) 
    : font_(font), show_milliseconds_(show_ms) {
    
    setSize(150, 30);
    
    if (font_) {
        time_text_.setFont(*font_);
    }
    time_text_.setCharacterSize(16);
    time_text_.setFillColor(sf::Color::White);
    
    updateTimeDisplay();
}

void ClockWidget::onIdle() {
    updateTimeDisplay();
}

void ClockWidget::updateTimeDisplay() {
    time_text_.setString(getCurrentTimeString());
    
    sf::FloatRect text_bounds = time_text_.getLocalBounds();
    time_text_.setPosition(
        rect_.left + (rect_.width - text_bounds.width) / 2,
        rect_.top + (rect_.height - text_bounds.height) / 2 - 5
    );
}

std::string ClockWidget::getCurrentTimeString() const {
    sf::Time elapsed = clock_.getElapsedTime();
    int total_seconds = static_cast<int>(elapsed.asSeconds());
    
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;
    int milliseconds = static_cast<int>(elapsed.asMilliseconds()) % 1000;
    
    std::stringstream ss;
    
    if (hours > 0) {
        ss << std::setfill('0') << std::setw(2) << hours << ":";
    }
    
    ss << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;
    
    if (show_milliseconds_) {
        ss << "." << std::setfill('0') << std::setw(3) << milliseconds;
    }
    
    return ss.str();
}

void ClockWidget::render(sf::RenderWindow& window) {
    if (!isVisible()) return;
    
    sf::RectangleShape background(sf::Vector2f(rect_.width, rect_.height));
    background.setPosition(rect_.left, rect_.top);
    background.setFillColor(sf::Color(30, 30, 40, 180));
    background.setOutlineThickness(1);
    background.setOutlineColor(sf::Color::White);
    window.draw(background);
    
    window.draw(time_text_);
    
    Widget::render(window);
}