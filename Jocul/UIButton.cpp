#include "UIButton.h"

UIButton::UIButton(const std::string& label, const sf::Font& font,
    float x, float y, float width, float height)
    : isHovered(false), isEnabled(true), text(font), buttonFont(&font) {

    position.x = x;
    position.y = y;
    size.x = width;
    size.y = height;

    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(50, 50, 50));
    shape.setOutlineThickness(2.0f);
    shape.setOutlineColor(sf::Color(100, 100, 100));

    text.setString(label);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = text.getLocalBounds();
    float textX = position.x + (width - textBounds.size.x) / 2.0f - textBounds.position.x;
    float textY = position.y + (height - textBounds.size.y) / 2.0f - textBounds.position.y;
    text.setPosition(sf::Vector2f(textX, textY));
}

void UIButton::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    shape.setPosition(position);

    sf::FloatRect textBounds = text.getLocalBounds();
    float textX = position.x + (size.x - textBounds.size.x) / 2.0f - textBounds.position.x;
    float textY = position.y + (size.y - textBounds.size.y) / 2.0f - textBounds.position.y;
    text.setPosition(sf::Vector2f(textX, textY));
}

void UIButton::setEnabled(bool enabled) {
    isEnabled = enabled;
    if (!enabled) {
        shape.setFillColor(sf::Color(30, 30, 30));
        text.setFillColor(sf::Color(100, 100, 100));
    }
    else {
        shape.setFillColor(sf::Color(50, 50, 50));
        text.setFillColor(sf::Color::White);
    }
}

void UIButton::setLabel(const std::string& label) {
    text.setString(label);

    sf::FloatRect textBounds = text.getLocalBounds();
    float textX = position.x + (size.x - textBounds.size.x) / 2.0f - textBounds.position.x;
    float textY = position.y + (size.y - textBounds.size.y) / 2.0f - textBounds.position.y;
    text.setPosition(sf::Vector2f(textX, textY));
}

bool UIButton::contains(float x, float y) const {
    return x >= position.x && x <= position.x + size.x &&
        y >= position.y && y <= position.y + size.y;
}

void UIButton::updateHover(float mouseX, float mouseY) {
    isHovered = contains(mouseX, mouseY) && isEnabled;

    if (isEnabled) {
        if (isHovered) {
            shape.setFillColor(sf::Color(70, 70, 70));
            shape.setOutlineColor(sf::Color(150, 150, 150));
        }
        else {
            shape.setFillColor(sf::Color(50, 50, 50));
            shape.setOutlineColor(sf::Color(100, 100, 100));
        }
    }
}

void UIButton::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}