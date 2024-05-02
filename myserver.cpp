#include "myserver.h"
#include <thread>
#include <SFML/Network.hpp>
#include <iostream>
#include "queue.h"
#include "list.h"
#include "accepter.h"
#include "message.h"
#include "util.h"
#include <iostream>
#include <thread>

myserver::myserver()
{

}

void myserver::run()
{
    //Binds the udp socket to the port
    sf::UdpSocket socket;
    auto runBind = [&socket] { return socket.bind(UPORT); };
    net_run(runBind, "UDP bind");
    std::cout << "UDP bound to port: " << usocket.getLocalPort() << std::endl;
    sf::Packet packet;
    sf::IpAddress address;
    unsigned short remote_port;

    while (true)
    {
        //Responds to broadcasted messages
        packet.clear();
        auto runRecv = [&] { return usocket.receive(packet, address, remote_port); };
        net_run(runRecv, "UDP receive");
        std::cout << "Recieved UDP message, replying" << std::endl;
        packet.clear();
        packet << "Yes I am";
        usocket.send(packet, address, remote_port);
        std::cout << "UDP message sent back" << std::endl;
    }
}

void myserver::operator()()
{
    Queue<Message> queue;
    std::<sf::Uint32, std::shared_ptr<sf::TcpSocket>> clients;

    std::thread(&myserver::runUDP, this).detach();

    std::thread(Accepter(queue)).detach();
    
    while (true)
    {
        //Prints all clients to see information about everyone thats connected
        std::for_each(clients.begin(), clients.end(), printClient);
        Message message = queue.pop();
        Msg msg;
        readMessage(message, msg);
		
        //Register message that will send to all clients
        if (msg.messagetype == MessageType::Register)
        {
            sf::Packet to_send;
            auto send_reg = [&](const Client& client)
                {
                    to_send.clear();
                    to_send << MessageReg(client.first, "");
                    auto my_send = [&]
                        {
                            return message.second->send(to_send);
                        };
                    net_run(my_send, "send");
                };
            std::for_each(clients.begin(), clients.end(), send_reg);
            clients.insert(Client(msg.id, message.socket));
        }
        //Only sends to one client
        auto send_to_one = [&](const Client& client) 
        {
            auto my_send = [&] 
            {

                if (client.first != msg.id)
                {
                    return client.second->send(message.first);
                }
                return sf::Socket::Done;
            };
            net_run(my_send, "send");
        };
        std::for_each(clients.begin(), clients.end(), send_to_one);
    }
}


////Copy from server.cpp
    //Queue<std::string> queue;
    //List<std::shared_ptr<sf::TcpSocket>> sockets;
    //
    //// TODO launch an accepter thread.
    //std::thread reciever_thread(Accepter(queue, sockets));

    //while(1)
    //{
    //    std::string s = queue.pop();
    //    std::cout << "Main read: \"" << s << "\"\n";
    //    
    //    // TODO send to all in sockets. Be careful to synchronise.
    //    auto sendMessage = [&s](std::shared_ptr<sf::TcpSocket> socket)
    //    {
    //        if(socket->send(s.c_str(), s.size() + 1) != sf::Socket::Done)
    //        {
    //            std::cout << "Message was not sent." << std::endl;
    //        }
    //    };

    //    sockets.for_each(sendMessage);
    //}
    //reciever_thread.join();
    //return;