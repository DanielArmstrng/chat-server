#include "accepter.h"
#include "queue.h"
#include "list.h"
#include "receiver.h"
#include "util.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>


int main()
{
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
    return 0;
}