#include "myserver.h"
#include <thread>
#include <SFML/Network.hpp>
#include <iostream>
#include "queue.h"
#include "list.h"
#include "accepter.h"

myserver::myserver()
{

}

void myserver::run()
{
    auto runUdpLoop = [] () 
    {
        sf::UdpSocket udpSocket;
        while(true)
        {
            // recv message
            char udpMsg[256];
            std::size_t size;
            sf::IpAddress serverAddress;
            unsigned short remote_port;
            sf::Socket::Status status = udpSocket.receive(udpMsg, 256, size, serverAddress, remote_port);
            // TODO send a message back
            udpSocket.send(udpMsg, size, serverAddress, remote_port);
        }
    };
    
    std::thread { runUdpLoop }.detach();

    // TODO copy from server.cpp
    Queue<std::string> queue;
    List<std::shared_ptr<sf::TcpSocket>> sockets;
    
    // TODO launch an accepter thread.
    std::thread reciever_thread(Accepter(queue, sockets));

    while(1)
    {
        std::string s = queue.pop();
        std::cout << "Main read: \"" << s << "\"\n";
        
        // TODO send to all in sockets. Be careful to synchronise.
        auto sendMessage = [&s](std::shared_ptr<sf::TcpSocket> socket)
        {
            if(socket->send(s.c_str(), s.size() + 1) != sf::Socket::Done)
            {
                std::cout << "Message was not sent." << std::endl;
            }
        };

        sockets.for_each(sendMessage);
    }
    reciever_thread.join();
    return;
}