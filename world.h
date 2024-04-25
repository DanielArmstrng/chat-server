#include <SFML/Graphics.hpp>
#include <array>
#include "tile.h"
#include "builder.h"

#ifndef WORLD_HPP
#define WORLD_HPP

class game;

enum class WorldState {
    Place,
    Select,
    Move,
    Build
};

class world {
    public:
        world(sf::RenderWindow& w, game *g);
        ~world();
        void update();
        void draw();
        void setup();
    private:
        game *g;
        sf::RenderWindow& window;
        std::array<std::array<tile, 5>, 5> tiles{{{{tile(), tile(), tile()}}, {{tile(), tile(), tile()}}, {{tile(), tile(), tile()}}}} ;
        std::vector<builder> builders;
        WorldState state = WorldState::Place;
        unsigned short buildersSoFar;
        unsigned short turn;
        std::size_t selectedBuilderIndex;
        void build(const sf::Vector2i& pos);
        void move(const sf::Vector2i& pos);
        void place(const sf::Vector2i& pos);
        void select(const sf::Vector2i& pos);
        bool noBuilder(const sf::Vector2i& pos);
        bool nearSelectecBuilder(const sf::Vector2i& pos);
        bool noDome(const sf::Vector2i& pos);
        int levelDiff(const sf::Vector2i& pos);
};
#endif
