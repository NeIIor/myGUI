// ReactorUI.hpp
#ifndef REACTOR_UI_HPP
#define REACTOR_UI_HPP

#include "UIApplication.hpp"
#include "../sim/Reactor.hpp"
#include "../sim/ReactorRenderer.hpp"
#include "../sim/GraphRenderer.hpp"
#include "Window.hpp"
#include "Button.hpp"
#include <memory>

class ReactorUI {
private:
    Reactor& reactor_;
    UIApplication app_;
    ReactorRenderer reactor_renderer_;
    GraphRenderer graph_renderer_;
    sf::Font font_;

    std::unique_ptr<Window> control_window_;
    std::unique_ptr<Window> stats_window_;

public:
    ReactorUI       (Reactor& reactor);
    bool initialize ();
    void handleEvent(const sf::Event& event);
    void render     (sf::RenderWindow& window);
    void update     (float dt);

private:
    void createControlWindow();
    void createStatsWindow  ();
    void createReactorWindow();
    void createClockWidget();
};

#endif // REACTOR_UI_HPP