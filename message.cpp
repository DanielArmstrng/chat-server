#include "message.h"
#include <iostream>
#include <string>
#include <memory>

sf::Packet& operator <<(sf::Packet& packet, const MessageType mtype)
{
	return packet << static_cast<sf::Uint8>(mtype);
}

sf::Packet& operator >>(sf::Packet& packet, MessageType& mtype)
{
	sf::Uint8 type;
	packet >> type;
	mtype = static_cast<MessageType>(type);
	return packet;
}

sf::Packet& operator << (sf::Packet& packet, const MessageReg message)
{
	return packet << message.messagetype << message.id << message.username;
}

sf::Packet& operator >> (sf::Packet& packet, MessageReg& message)
{
	return packet >> message.messagetype >> message.id >> message.username;
}

sf::Packet& operator << (sf::Packet& packet, const MessageSelect message)
{
	return packet << message.messagetype << message.id << message.builderID;
}

sf::Packet& operator >> (sf::Packet& packet, MessageSelect& message)
{
	return packet >> message.messagetype >> message.id >> message.builderID;
}

sf::Packet& operator << (sf::Packet& packet, const MessagePos message)
{
	return packet << message.messagetype << message.id << message.x << message.y;
}

sf::Packet& operator >> (sf::Packet& packet, MessagePos& message)
{
	return packet >> message.messagetype >> message.id >> message.x >> message.y;
}

sf::Packet& operator << (sf::Packet& packet, const MessageRand message)
{
	return packet << message.messagetype << message.r;
}

sf::Packet& operator >> (sf::Packet& packet, MessageRand& message)
{
	return packet >> message.messagetype >> message.r;
}

void readMessage(Message m, Message& message)
{
	MessagePos* m_pos;
	MessageRand* m_rand;
	MessageReg* m_reg;
	MessageSelect* m_select;
	std::stringstream* ss;

	m.first >> message.messagetype;
	switch (m.messagetype)
	{
	case MessageType::Register:
		m_reg = new MessageReg();
		if (m.first >> m_reg -> id >> m_reg -> username);
		{
			std::memcpy(&message, m_reg, sizeof(MessageReg));
		}
		else
		{
			ss << "Error reading register message" << endl;
		}
		delete m_reg;
		break;
	case MessageType::Select:
		m_select = new MessageSelect();
		if (m.first >> m_select -> id >> m_select -> builderID);
		{
			std::memcpy(&message, m_select, sizeof(MessageSelect));
		}
		else
		{
			ss << "Error reading select message" << endl;
		}
		delete m_select;
		break;
	case MessageType::Position:
		m_pos = new MessagePos();
		if (m.first >> m_pos -> id >> m_pos -> x >> m_pos -> y);
		{
			std::memcpy(&message, m_pos, sizeof(MessagePos));
		}
		else
		{
			ss << "Error reading position message" << endl;
		}
		delete m_pos;
		break;
	case MessageType::Random:
		m_rand = new MessageRand();
		if (m.first >> m_rand -> id >> m_rand -> r);
		{
			std::memcpy(&message, m_rand, sizeof(MessageRand));
		}
		else
		{
			ss << "Error reading random message" << endl;
		}
		delete m_rand;
		break;
	default:
		ss << "Error reading message" << endl;
		break;
	}
	std::cout << ss.str();
}
