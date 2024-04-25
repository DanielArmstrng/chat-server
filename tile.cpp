#include "colours.h"
#include "tile.h"

tile::tile():
    pos(0, 0),
    level(0)
{
}

void tile::draw(sf::RenderWindow &w)
{
    sf::RectangleShape rectangle;
    rectangle.setOutlineThickness(5);

    if (level == 0)
    {
        rectangle.setSize(sf::Vector2f(TILE_SIZE-10, TILE_SIZE-10));
        rectangle.setPosition(pos.x*TILE_SIZE+5, pos.y*TILE_SIZE+5);
        rectangle.setFillColor(col_level0);
        w.draw(rectangle);
    }
    else
    {
        int limit = (level<4)?level:3;
        for (int i = 0; i < limit; i++)
        {
            int s = TILE_SIZE-10 - (i * 20);
            rectangle.setSize(sf::Vector2f(s, s));
            rectangle.setPosition((pos.x*TILE_SIZE)+(i*10)+5, (pos.y*TILE_SIZE)+(i*10)+5);
            rectangle.setOutlineColor(col_outline);
            rectangle.setFillColor(col_level1);
            w.draw(rectangle);
        }
    }
    if (level == 4)
    {
        sf::CircleShape circle(40);
        circle.setPosition(pos.x*TILE_SIZE+20, pos.y*TILE_SIZE+20);
        circle.setFillColor(col_dome);
        w.draw(circle);
    }
}

bool tile::build()
{
    if (level < 4)
    {
        level++;
        return true;
    }
    return false;
}
