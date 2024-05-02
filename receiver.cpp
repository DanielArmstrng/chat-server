#include "receiver.h"
#include "util.h"
#include <chrono>
#include <cstring>
#include <iostream>
#include <iostream>
#include <sstream>
#include <thread>

Receiver::Receiver(std::shared_ptr<sf::TcpSocket> s,
        Queue<std::string> &q):
    m_socket(s),
    m_queue(q)
{
}

void Receiver::recv_loop()
{
    char buffer[256];
    {
        std::stringstream ss;
        ss << m_socket->getRemoteAddress() << ":" << m_socket->getRemotePort() << std::endl;
        std::cout << ss.str();
    }
    while(1)
    {
        std::memset(buffer, 0, 256);
        std::size_t received;
        // Receives a message
        if (m_socket->receive(buffer, 256, received) != sf::Socket::Done) 
        {
            // Error: output a message then quit the function
            std::cout << "Nothing was recieved." << std::endl;
            return;
        }
        {
            // amount of data received
            std::stringstream ss;
            ss << "Received: \"" << buffer << "\", " << received << " bytes." << std::endl;
            std::cout << ss.str();
        }
        m_queue.push(std::string(buffer));
    }
}

