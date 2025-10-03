    // Window.hpp
    #ifndef WINDOW_HPP
    #define WINDOW_HPP

    #include "Container.hpp"
    #include <string>

    class Window : public Container {
    private:
        std::string title_;
        bool dragging_ = false;
        sf::Vector2f drag_offset_;
        sf::FloatRect title_bar_;

    public:
        Window(const std::string& title, const sf::FloatRect& rect);
        
        void onMouseMove(MouseMoveEvent&   event) override;
        void onMouseDown(MouseButtonEvent& event) override;
        void onMouseUp  (MouseButtonEvent& event) override;
        
        void render    (sf::RenderWindow& window) override;
        
        void setTitle(const std::string& title) { title_ = title; }
        const std::string& getTitle() const     { return   title_;}

    private:
        void updateLayout() override;
    };

    #endif // WINDOW_HPP