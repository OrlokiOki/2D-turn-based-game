#include <SFML/Graphics.hpp>

class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;
    bool isClicked = false;

    Button(float x, float y, float width, float height, const std::string& label, sf::Font& font) {
        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color::Green);

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + 10, y + 10); // Adjust for text padding
    }

    bool isPressed(sf::Vector2i mousePos) {
        return shape.getGlobalBounds().contains(sf::Vector2f(mousePos));
    }
};

void renderActionMenu(sf::RenderWindow& window, std::vector<Button>& buttons) {
    for (auto& button : buttons) {
        window.draw(button.shape);
        window.draw(button.text);
    }
}