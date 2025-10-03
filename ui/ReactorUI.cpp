// ReactorUI.cpp
#include "ReactorUI.hpp"
#include "ClockWidget.hpp"
#include <iostream>

ReactorUI::ReactorUI(Reactor& reactor) 
    : reactor_(reactor), 
      reactor_renderer_(reactor),
      graph_renderer_(reactor, &font_) {
}

bool ReactorUI::initialize() {
    if (!font_.loadFromFile("../resources/arialmt.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return false;
    }
    
    auto root = std::make_unique<Widget>();
    root->setRect(sf::FloatRect(0, 0, 1500, 900));
    app_.setRoot(std::move(root));
    
    createControlWindow();
    createStatsWindow();
    createReactorWindow();
    createClockWidget();
    
    return true;
}

void ReactorUI::createClockWidget() {
    auto clock = std::make_unique<ClockWidget>(&font_, true);
    clock->setRect(sf::FloatRect(1500 - 160, 10, 150, 30));
    clock->setTextColor(sf::Color::Yellow);
    
    app_.getRoot()->addChild(std::move(clock));
}

void ReactorUI::createControlWindow() {
    control_window_ = std::make_unique<Window>("Controls", sf::FloatRect(10, 10, 400, 250));
    
    auto temp_up = std::make_unique<Button>("Temp Up", &font_);
    temp_up->setRect(sf::FloatRect(20, 40, 80, 30));
    temp_up->setOnClick([this]() { reactor_.increaseLeftWallTemperature(); });
    control_window_->addChild(std::move(temp_up));
    
    auto temp_down = std::make_unique<Button>("Temp Down", &font_);
    temp_down->setRect(sf::FloatRect(110, 40, 80, 30));
    temp_down->setOnClick([this]() { reactor_.decreaseLeftWallTemperature(); });
    control_window_->addChild(std::move(temp_down));
    
    auto wall_left = std::make_unique<Button>("Wall Left", &font_);
    wall_left->setRect(sf::FloatRect(200, 40, 80, 30));
    wall_left->setOnClick([this]() { 
        if (reactor_.getReactorWidth() > 200) {
            reactor_.resize(reactor_.getReactorWidth() - 10);
            reactor_renderer_.updateGraphics();
        }
    });
    control_window_->addChild(std::move(wall_left));
    
    auto wall_right = std::make_unique<Button>("Wall Right", &font_);
    wall_right->setRect(sf::FloatRect(290, 40, 80, 30));
    wall_right->setOnClick([this]() { 
        if (reactor_.getReactorWidth() < 800) {
            reactor_.resize(reactor_.getReactorWidth() + 10);
            reactor_renderer_.updateGraphics();
        }
    });
    control_window_->addChild(std::move(wall_right));
    
    auto add_round = std::make_unique<Button>("Add Round", &font_);
    add_round->setRect(sf::FloatRect(20, 80, 100, 30));
    add_round->setOnClick([this]() { reactor_.addRoundMolecule(); });
    control_window_->addChild(std::move(add_round));
    
    auto add_square = std::make_unique<Button>("Add Square", &font_);
    add_square->setRect(sf::FloatRect(130, 80, 100, 30));
    add_square->setOnClick([this]() { reactor_.addSquareMolecule(); });
    control_window_->addChild(std::move(add_square));
    
    auto remove_btn = std::make_unique<Button>("Remove Last", &font_);
    remove_btn->setRect(sf::FloatRect(240, 80, 100, 30));
    remove_btn->setOnClick([this]() { reactor_.removeLastMolecule(); });
    control_window_->addChild(std::move(remove_btn));
    
    auto clear_btn = std::make_unique<Button>("Clear All", &font_);
    clear_btn->setRect(sf::FloatRect(20, 120, 100, 30));
    clear_btn->setOnClick([this]() { reactor_.clearAll(); });
    control_window_->addChild(std::move(clear_btn));
    
    app_.getRoot()->addChild(std::move(control_window_));
}

void ReactorUI::createStatsWindow() {
    stats_window_ = std::make_unique<Window>("Statistics", sf::FloatRect(1150, 10, 340, 300));
    graph_renderer_.setPosition(1160, 80);
    graph_renderer_.setSize(320, 260);
}

void ReactorUI::createReactorWindow() {
    reactor_renderer_.updateGraphics();
}

void ReactorUI::handleEvent(const sf::Event& event) {
    app_.handleEvent(event);
}

void ReactorUI::render(sf::RenderWindow& window) {
    reactor_renderer_.render(window);
    app_.render(window);
    graph_renderer_.render(window);
    
    sf::Text info_text;
    info_text.setFont(font_);
    info_text.setCharacterSize(12);
    info_text.setFillColor(sf::Color::White);
    info_text.setPosition(10, 270);
    
    std::string info = "Reactor: " + std::to_string((int)reactor_.getReactorWidth()) + "x" + 
                      std::to_string((int)reactor_.getReactorHeight()) + 
                      " | Molecules: " + std::to_string(reactor_.getMolecules().size()) +
                      " | Temp: " + std::to_string(reactor_.getLeftWallTemperature()).substr(0, 4);
    info_text.setString(info);
    window.draw(info_text);
}

void ReactorUI::update(float dt) {
    reactor_renderer_.updateGraphics();
    app_.update(dt);
}