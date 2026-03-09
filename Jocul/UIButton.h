#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class UIButton {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Vector2f position;
    sf::Vector2f size;
    bool isHovered;
    bool isEnabled;
    const sf::Font* buttonFont;  

public:
    UIButton(const std::string& label, const sf::Font& font,
        float x, float y, float width, float height);

    void setPosition(float x, float y);
    void setEnabled(bool enabled);
    void setLabel(const std::string& label);
    bool contains(float x, float y) const;
    void updateHover(float mouseX, float mouseY);
    void draw(sf::RenderWindow& window);
    bool isButtonEnabled() const { return isEnabled; }
};