#include <iostream>
#include <SFML/Graphics.hpp>
#include "entity.h"

#ifndef TILE_HPP
#define TILE_HPP

#define TILE_SIZE 120

class tile : public entity {
public:
    tile();
    void draw(sf::RenderWindow &w);
    bool build();
    sf::Vector2i pos;
    unsigned short level = 0;
private:
};

#endif
