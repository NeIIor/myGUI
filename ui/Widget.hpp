// Widget.hpp
#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

class MouseMoveEvent;
class MouseButtonEvent;

class Widget {
protected:
    sf::FloatRect rect_;
    std::vector<std::unique_ptr<Widget>> children_;
    Widget* parent_ = nullptr;
    int z_order_ = 0;
    bool visible_ = true;

public:
             Widget() = default;
    virtual ~Widget() = default;

    virtual bool    contains        (const sf::Vector2f& point) const;
    virtual Widget* getPointerTarget(const sf::Vector2f& point);
    
    virtual void onMouseMove (MouseMoveEvent&   event);
    virtual void onMouseDown (MouseButtonEvent& event);
    virtual void onMouseUp   (MouseButtonEvent& event);
    virtual void onMouseEnter();
    virtual void onMouseLeave();
    virtual void onIdle      ();
    
    virtual void render(sf::RenderWindow& window);
    
    void addChild(std::unique_ptr<Widget> child);
    void removeChild(Widget* child);
    
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setRect(const sf::FloatRect& rect);
    sf::FloatRect getRect() const { return rect_; }
    
    void setZOrder(int order) { z_order_ = order; }
    int getZOrder () const     {  return z_order_;}
    
    void setVisible(bool visible)  { visible_ = visible; }
    bool isVisible () const        { return     visible_; }
    
    Widget*     getParent  () const { return parent_;   }
    const auto& getChildren() const { return children_; }

protected:
    virtual void updateLayout();
};

#endif // WIDGET_HPP