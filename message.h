#ifndef MESSAGE_HPP
#define MESSAGE_HPP

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

sf::Packet& operator >> (sf::Packet& packet, MessageSelect& message);
sf::Packet& operator << (sf::Packet& packet, const MessageSelect message);

struct MessagePos
{
    MessageType messagetype = MessageType::Undefined;
    sf::Uint32 id = 0;
    sf::Uint8 x = 0;
    sf::Uint8 y = 0;
    MessagePos() {}
    MessagePos(MessageType messagetype_, sf::Uint32 id_, sf::Veector2i pos) : messagetype(messagetype_), id(id_), x(pos.x), y(pos.y) {}
};

sf::Packet& operator >> (sf::Packet& packet, MessagePos& message);
sf::Packet& operator << (sf::Packet& packet, const MessagePos message);

struct MessageRand
{
    MessageType messagetype = MessageType::Rand;
    sf::Uint32 id = 0;
    sf::Uint32 r = 0;
    MessageRand() {}
    MessageRand(sf::Uint32 id_, sf::Uint32 r_) : id(id_), r(r_) {}
};

sf::Packet& operator >> (sf::Packet& packet, MessageRand& message);
sf::Packet& operator << (sf::Packet& packet, const MessageRand message);

void readMessage(Message m, Message& message);

#endif