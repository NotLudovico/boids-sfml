#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Text {
 private:
  sf::Text text_;

 public:
  Text(std::string const& content, int size, sf::Color const& color,
       sf::Vector2f const& position, sf::Font const& font);
  void draw(sf::RenderWindow& window) const;
  void update_text(std::string const& new_content);
};

inline Text::Text(std::string const& content, int size, sf::Color const& color,
                  sf::Vector2f const& position, sf::Font const& font) {
  text_.setFont(font);
  text_.setString(content);
  text_.setCharacterSize(size);
  text_.setFillColor(color);
  text_.setPosition(position);
}
inline void Text::draw(sf::RenderWindow& window) const { window.draw(text_); }

inline void Text::update_text(std::string const& new_content) {
  text_.setString(new_content);
}
#endif