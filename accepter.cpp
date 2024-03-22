#include "accepter.h"
#include "receiver.h"
#include "util.h"
#include <iostream>
#include <sstream>
#include <thread>

Accepter::Accepter(Queue<std::string>& q, List<std::shared_ptr<sf::TcpSocket>>& s):
    m_queue(q),
    m_socket(s)
{}

void Accepter::operator()()
{
    sf::TcpListener listener;
    // TODO the listener has to listen.
    if (listener.listen(PORT) != sf::Socket::Done)
    {
        // output
        // return
    }

    //listner.listen(53000);

    // while (running)
    // {
    //     sf::TcpSocket client;
    //     if(listner.accept(client) == sf::Socket::Done)
    //     {
    //         std::cout << "New connection from " << client.getRemoteAddress() << std::endl;
            
    //     }
    // }



    std::cout << "Bound to port\n";
    while (true)
    {
        std::shared_ptr<sf::TcpSocket> socket = std::make_shared<sf::TcpSocket>();
        // TODO accept a connection on socket
        if (listener.accept(*socket) != sf::Socket::Done)
        {
            std::cout << "Connection wasnt accepted." << std::endl;
            // return
        }


        m_socket.push(socket);
        std::stringstream ss;
        ss << "Accepted a connection from: "
            << socket->getRemoteAddress()
            << ":"
            << socket->getRemotePort()
            << std::endl;
        std::cout << ss.str();
        std::shared_ptr<Receiver> receiver = std::make_shared<Receiver>(socket, m_queue);
        // TODO launch a thread to receive with the receiver
        std::thread accepter_thread(&Receiver::recv_loop, receiver);
        accepter_thread.detach();
    }
}

