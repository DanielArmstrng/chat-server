#define PORT 4300
#define UPORT 4301

#include <SFML/Network.hpp>
#include <functional>
#include <memory>

typedef std::pair<sf::Packet, std::shared_ptr<sf::TcpSocket>> Message;

void net_run(std::function<sf::Socket::Status(void)> f, std::string error_message);

