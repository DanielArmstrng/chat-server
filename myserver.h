#include <SFML/Network.hpp>
#include <memory>

typedef std::pair<sf:Uint32, std::shared_ptr<sf::TcpSocket>> Client;

class myserver 
{
public:
    myserver();
    void operator()();
private:
    void run();
};