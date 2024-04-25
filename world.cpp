#include "world.h"
#include "game.h"

world::world(sf::RenderWindow& w, game *g_):
    window(w),
    g(g_)
{
    srand(time(0));
    window.setFramerateLimit(60);
    setup();
    buildersSoFar = 0;
}

void world::setup()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            tiles[i][j].pos = sf::Vector2i(i, j);
            tiles[i][j].level = 0;
        }
    }
    turn = 0;
    state = WorldState::Place;
}

world::~world()
{
}

bool world::noBuilder(const sf::Vector2i& pos)
{
    for (builder& b: builders)
    {
        if (pos == b.pos)
        {
            return false;
        }
    }
    return true;
}

bool world::nearSelectecBuilder(const sf::Vector2i& pos)
{
    sf::Vector2i diff = pos - builders[selectedBuilderIndex].pos;
    return (diff.x >= -1 && diff.x <= 1 &&
            diff.y >= -1 && diff.y <= 1 &&
            !(diff.x == 0 && diff.y == 0));
}

bool world::noDome(const sf::Vector2i& pos)
{
    return tiles[pos.x][pos.y].level < 4;
}

int world::levelDiff(const sf::Vector2i& pos)
{
    sf::Vector2i bpos = builders[selectedBuilderIndex].pos;
    return tiles[pos.x][pos.y].level - tiles[bpos.x][bpos.y].level;
}

void world::build(const sf::Vector2i& pos)
{
    // Check if no builder is there.
    // Check if near selected builder.
    // Check if not on a dome.
    if (noBuilder(pos) && nearSelectecBuilder(pos) && noDome(pos))
    {
        tiles[pos.x][pos.y].build();
        state = WorldState::Select;
        turn++;
    }
}

void world::move(const sf::Vector2i& pos)
{
    // If on a builder and this builder is of the same colour, we should select this builder and
    // return.
    if (!noBuilder(pos)) // Builder was clicked
    {
        for (size_t i = 0; i < builders.size(); i++)
        {
            if (builders[i].pos == pos && turn % 2 == builders[i].player)
            {
                selectedBuilderIndex = i;
                return;
            }
        }
    }
    // Check if no builder is there.
    // Check not on top of dome.
    // Check near selected builder.
    // Check builder does not climb more than one floor.
    if (noBuilder(pos) && nearSelectecBuilder(pos) && noDome(pos) && levelDiff(pos) <= 1)
    {
        sf::Vector2i prevPos = builders[selectedBuilderIndex].pos;
        if (tiles[prevPos.x][prevPos.y].level == 2 && tiles[pos.x][pos.y].level == 3)
        {
            // TODO
            // Instead of checking the turn, we want to check if the LOCAL player
            // has won the game. Could be done easily with a class variable to note who the
            // local player is.
            if (turn % 2 == 0)
            {
                g->state = gamestate::victory;
            }
            else
            {
                g->state = gamestate::defeat;
            }
        }
        sf::Vector2i m = pos - builders[selectedBuilderIndex].pos;
        builders[selectedBuilderIndex].move(m);
        state = WorldState::Build;
    }
}

void world::place(const sf::Vector2i& pos)
{
    // Check not on top of other builder.
    if (noBuilder(pos))
    {
        builders.push_back(builder(pos.x, pos.y, buildersSoFar/2));
        buildersSoFar++;
        if (buildersSoFar == 4)
        {
            state = WorldState::Select;
        }
    }
}

void world::select(const sf::Vector2i& pos)
{
    // Check correct player.
    // Check builder has a valid move. TODO
    for (size_t i = 0; i < builders.size(); i++)
    {
        if (builders[i].pos == pos)
        {
            if (turn % 2 == builders[i].player)
            {
                selectedBuilderIndex = i;
                state = WorldState::Move;
                return;
            }
        }
    }
}

void world::update()
{
    static bool clicked = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clicked)
    {
        clicked = true;
        sf::Vector2i pos = sf::Mouse::getPosition(window) / TILE_SIZE;
        switch (state) {
            case WorldState::Build:
                std::cout << "Build\n";
                build(pos);
                break;
            case WorldState::Move:
                std::cout << "Move\n";
                move(pos);
                break;
            case WorldState::Place:
                std::cout << "Place\n";
                place(pos);
                break;
            case WorldState::Select:
                std::cout << "Select\n";
                select(pos);
                break;
            default:
                break;
        }
    }
    else
    {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            clicked = false;
        }
    }
}

void world::draw()
{
    window.clear();
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            tiles[i][j].draw(window);
        }
    }
    for (builder& b: builders)
    {
        b.draw(window);
    }
}
