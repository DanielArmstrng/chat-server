#include "world.h"
#include "game.h"
#include <cstring>
#include "message.h"

world::world(sf::RenderWindow& w, game *g_):
    window(w),
    g(g_)
    queue(),
    network(queue),
{
    id = rand();
    network.reg(id);
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

void world::build(const sf::Vector2i& pos, bool toSend)
{
    // Check if no builder is there.
    // Check if near selected builder.
    // Check if not on a dome.
    if (noBuilder(pos) && nearSelectecBuilder(pos) && noDome(pos))
    {
        if(toSend)
        {
            network.send(MessagePos(MessageType::Build, id, pos));
        }
        tiles[pos.x][pos.y].build();
        state = WorldState::Select;
        turn++;
    }
}

void world::move(const sf::Vector2i& pos, bool toSend)
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
                if(toSend)
                {
                    network.send(MessageSelect(id, i));
                }
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
        if(toSend)
        {
            network.send(MessagePos(MessageType::Build, id, pos));
        }
        sf::Vector2i m = pos - builders[selectedBuilderIndex].pos;
        builders[selectedBuilderIndex].move(m);
        state = WorldState::Build;
    }
}

void world::place(const sf::Vector2i& pos, bool toSend)
{
    // Check not on top of other builder.
    if (noBuilder(pos))
    {
        if(toSend)
        {
            network.send(MessagePos(MessageType::Build, id, pos));
        }
        builders.push_back(builder(pos.x, pos.y, buildersSoFar/2));
        buildersSoFar++;
        if (buildersSoFar == 4)
        {
            state = WorldState::Select;
        }
    }
}

void world::select(const sf::Vector2i& pos, bool toSend)
{
    // Check correct player.
    // Check builder has a valid move. TODO
    for (size_t i = 0; i < builders.size(); i++)
    {
        if (builders[i].pos == pos)
        {
            if (turn % 2 == builders[i].player)
            {
                if(toSend)
                {
                    network.send(MessageSelect(id, i));
                }
                selectedBuilderIndex = i;
                state = WorldState::Move;
                return;
            }
        }
    }
}

void world::processPlace(const Message& message)
{
    MessagePos m_pos;
    std::memcpy(&m_pos, &message, sizeof(MessagePos));
    sf::Vector2i pos(m_pos.x, m_pos.y);
    place(pos, false);
}

void world::update()
{
    static bool clicked = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clicked)
    {
        clicked = true;
        sf::Vector2i pos = sf::Mouse::getPosition(window) / TILE_SIZE;
        switch (state) 
        {
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

    Message m;
    queue.pop(m);

    if (m.first.endOfPacket())
    {
        return;
    }

    Msg msg;
    readMessage(m, msg);

    switch(msg.MessageType)
    {
        case MessageType::Register:
            std::cout << "Message type: register" << std::endl;
            processRegister(msg);
            break;
        case MessageType::Select:
            std::cout << "Message type: select" << std::endl;
            processSelect(msg);
            break;
        case MessageType::Build:
            std::cout << "Message type: build" << std::endl;
            processBuild(msg);
            break;
        case MessageType::Move:
            std::cout << "Message type: move" << std::endl;
            processMove(msg);
            break;
        case MessageType::Rand:
            std::cout << "Message type: rand" << std::endl;
            break;
        case MessageType::Place:
            std::cout << "Message type: place" << std::endl;
            processPlace(msg);
            break;
        default:
            break;
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
