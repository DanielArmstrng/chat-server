#include "queue.h"
#include "receiver.h"
#include "util.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

// create a udp socket 
// send a message to the broadcast address
// wait 1 second
// set the udp socket to non blocking
// receive
// if receive worked, we know the address for the server
// if not, make a thread that runs the server
// wait a bit

int main()
{
    std::shared_ptr<sf::TcpSocket> socket = std::make_shared<sf::TcpSocket>();
    sf::Socket::Status status = socket->connect("152.105.66.55", PORT);
    if (status != sf::Socket::Done) 
    {
        return 1;
    }
    
    std::cout << "Connected\n";
    Queue<std::string> queue;
    // TODO launch a receiver thread to receive messages from the server.
    Receiver receiver(socket, queue);
    std::thread recv_thread(&Receiver::recv_loop, &receiver);

    std::string s = "";
    while (1)
    {
        std::getline(std::cin, s);
        std::cout << "Sending: \"" << s << "\"" << std::endl;
        // TODO send messages to the server
        std::size_t sent;
        if (socket->send(s.c_str(), s.size() + 1, sent) != sf::Socket::Done) 
        {
            std::cout << "Nothing was sent." << std::endl;
        }
    }

    recv_thread.join();
    return 0;
}
