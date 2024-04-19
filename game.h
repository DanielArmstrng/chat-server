#include <SFML/Graphics.hpp>
#include "world.h"

#ifndef GAME_HPP
#define GAME_HPP

enum class gamestate {
    place,
    play,
    victory,
    defeat,
    terminate
};

class game {
    private:
        sf::RenderWindow window;
        world *w;
        void place();
        void play();
        void victory();
        void defeat();
    public:
        gamestate state = gamestate::play;
        game();
        ~game();
        void run();
};
#endif
