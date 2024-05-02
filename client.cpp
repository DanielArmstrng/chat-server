#include "queue.h"
#include "receiver.h"
#include "util.h"
#include "myserver.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

int main()
{
    //UDP broadcast
    sf::UdpSocket udpSocket;
    std::string bcast_msg = "Are you a server?";
    udpSocket.send(bcast_msg.c_str(), bcast_msg.size() + 1, sf::IpAddress::Broadcast, PORT + 1);
    //set udp socket to non blocking
    udpSocket.setBlocking(false);
    //waits 1s
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //recv
    char udpMsg[256];
    std::size_t size;
    sf::IpAddress serverAddress;
    unsigned short remote_port;
    sf::Socket::Status status = udpSocket.receive(udpMsg, 256, size, serverAddress, remote_port);
    //if there is an answer connect to it
    if(status != sf::Socket::Done)
    {
        serverAddress = sf::IpAddress::LocalHost;
        myserver server;
        std::thread runServer (&myserver::run, &server);
        runServer.detach();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::shared_ptr<sf::TcpSocket> socket = std::make_shared<sf::TcpSocket>();
    status = socket->connect(serverAddress, PORT);
    if (status != sf::Socket::Done) 
    {
        return 1;
    }
    
    std::cout << "Connected\n";
    Queue<std::string> queue;
    //launches a receiver thread to receive messages from the server.
    Receiver receiver(socket, queue);
    std::thread recv_thread(&Receiver::recv_loop, &receiver);

    std::string s = "";
    while (1)
    {
        std::getline(std::cin, s);
        std::cout << "Sending: \"" << s << "\"" << std::endl;
        //sends messages to the server
        std::size_t sent;
        if (socket->send(s.c_str(), s.size() + 1, sent) != sf::Socket::Done) 
        {
            std::cout << "Nothing was sent." << std::endl;
        }
    }

    recv_thread.join();
    return 0;
}

