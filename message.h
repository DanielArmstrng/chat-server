#ifndef MESSAGE_HPP
#define MESAGE_HPP

#include "util.h"
#include <SFML/Network.hpp>

enum class MessageType: sf::Uint8
{
    Undefined = 0,
    Build = 1,
    Move = 2,
    Place = 4,
    Rand = 8,
    Register = 16,
    Select = 32
};

sf::Packet& operator >> (sf::Packet& packet, MessageType& mtype);
sf::Packet& operator << (sf::Packet& packet, const MessageType mtype);

struct Message
{
    MessageType messagetype = MessageType::Undefined;
    sf::Uint32 id;
    char data[216];
};

struct MessageReg
{
    MessageType messagetype = MessageType::Register;
    sf::Uint32 id = 0;
    std::string username;
    MessageReg(){}
    MessageReg(sf::Uint32 id_, std::string s): id(id_), username(s) {}
};

sf::Packet& operator << (sf::Packet& packet, const MessageReg message);
sf::Packet& operator >> (sf::Packet& packet,  MessageReg message);

struct MessageSelect
{
    MessageType messagetype = MessageType::Select;
    sf::Uint32 id = 0;
    sf::Uint8 builderID = 0;
    MessageSelect(){}
    MessageSelect(sf::Uint32 id_, sf::Uint8 builderID_) : id(id_), builderID(builderID_) {}
};

sf::Packet& operator >> (sf::Packet& packet, MessageSelect& mtessage);
sf::Packet& operator << (sf::Packet& packet, const MessageSelect message);



#endif