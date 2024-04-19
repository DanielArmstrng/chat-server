#include "builder.h"
#include "colours.h"
#include "tile.h"
#include <iostream>

builder::builder(unsigned short x_, unsigned short y_, unsigned short p_):
    pos(x_, y_),
    player(p_)
{
}

void builder::draw(sf::RenderWindow &window)
{
    sf::CircleShape circle(40, 40);
    if (player == 0)
    {
        circle.setFillColor(col_player1);
    }
    else
    {
        circle.setFillColor(col_player2);
    }
    circle.setPosition(pos.x * TILE_SIZE + 20, pos.y * TILE_SIZE + 20);
    window.draw(circle);
}

void builder::move(sf::Vector2i m)
{
    pos += m;
}
