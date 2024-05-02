#include "network.h"
#include "reciever.h"
#include "server.h"
#include "util.h"
#include <iostream>

Network::Network(Queue<Message>& queue) : queue(queue) 
{
	//Send UDP message to find server
	sf::UdpSocket socket;
	sf::Packet packet;
	packet << "Are you a server?";
	unsigned short port = UPORT;
	std::cout << "Sending UDP find server message " << port << std::endl;
	auto sendUDP = [&] { return udpSocket.send(packet, sf::IpAddress::Broadcast, UPORT); };
	net_run(sendUDP, "UDP send");
	std::cout << "Sent UDP message" << std::endl;

	//Recieve UDP message from server
	packet.clear();
	sf::IpAddress address;
	udpSocket.setBlocking(false);
	bool serverResponded = false;
	sf::Clock clock;
	while (clock.getElapsedTime().asSeconds() < 2.0f)
	{
		udpSocket.receive(packet, address, port);
		if (!packet.endOfPacket())
		{
			serverResponded = true;
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

	}
	if (!serverResponded)
	{
		std::thread(Server()).detach();
		address = sf::IpAddress::getLocalAddress();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	std::cout << "Recieved UDP message from server" << std::endl;

	socket = std::make_shared<sf::TcpSocket>();
	auto connect = [&] { return socket->connect(address, PORT); };
	net_run(connect, "TCP connect");
	std::cout << "Connected to server" << std::endl;

	Reciever reciever(socket, queue);
	std::thread(&Reciever::recv_loop, std::move(reciever)).detach();
}

Network::~Network()
{
}

void Network::reg(const sf::Uint32 id)
{
	//Register client with an id and a name
	sf::Packet m_reg_packet;
	m_reg_packet << MessageReg(id, "Daniel");
	auto send1 = [&] { return socket->send(m_reg_packet); };
	net_run(send1, "send");
	std::cout << "Sent register message" << std::endl;
}

void Network::send(const MessageReg& message)
{
	sf::Packet packet;
	packet << message;
	auto s = [&] { return socket->send(packet); };
	net_run(s, "sending TCP message");
}

void Network::send(const MessageSelect& message)
{
	sf::Packet packet;
	packet << message;
	auto s = [&] { return socket->send(packet); };
	net_run(s, "sending TCP message");
}

void Network::send(const MessageRand& message)
{
	sf::Packet packet;
	packet << message;
	auto s = [&] { return socket->send(packet); };
	net_run(s, "sending TCP message");
}

void Network::send(const MessagePos& message)
{
	sf::Packet packet;
	packet << message;
	auto s = [&] { return socket->send(packet); };
	net_run(s, "sending TCP message");
}