#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "message.h"
#include "queue.h"
#include "util.h"
#include <SFML/Network.hpp>

class Network
{
public:
	Network(Queue<Message>& queue);
	~Network();
	void reg(const sf::Uint32 id);
	void send(const MessageReg& message);
	void send(const MessageSelect& message);
	void send(const MessagePos& message);
	void send(const MessageRand& message);
private:
	Queue<Message>& queue;
	std::shared_ptr<sf::TcpSocket> socket;
};

#endif