#include <iostream>
#include <string>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Server.h"
#include "Client.h"

#define PORTNUM 9966
#define MAX_BUFFER_LEN 500

void receive(sf::UdpSocket *sock)
{
	sf::UdpSocket *socket = sock;
	char buffer[MAX_BUFFER_LEN];
	std::size_t messageSize;
	sf::IpAddress serverIp = "128.199.38.220";
	unsigned short port = socket->getLocalPort();//9966;
	sf::Socket::Status status;

	while (true)
	{
		status = socket->receive(buffer, MAX_BUFFER_LEN, messageSize, serverIp, port);

		if (status == sf::Socket::Done)
		{
			buffer[messageSize] = '\0';
			std::cout << "FROM SERVER:: " << buffer << std::endl;
		}
		else
			std::cout << "Could not receive message" << status << std::endl;
	}
}

int main(int argv, char* argc[])
{
	std::cout << "(1) Server" << std::endl;
	std::cout << "(2) Client" << std::endl;
	
	std::string input;
	std::getline(std::cin, input);

	if (input == "1")
	{
		Server server;
		server.run();
	}
	else
	{
		Client client;
		client.run();
	}

	/*

	sf::UdpSocket socket;
	sf::Socket::Status status = socket.bind(sf::Socket::AnyPort);//socket.bind(PORTNUM);
	sf::IpAddress serverIp = "128.199.38.220";

	if (status != sf::Socket::Done)
		std::cout << "ERROR:: Could not bind socket" << std::endl;

	std::cout << "DEBUG:: Socket bound to port " << socket.getLocalPort() << std::endl;

	sf::Thread listenerThread(&receive, &socket);
	listenerThread.launch();

	std::size_t messageSize;
	std::string data;

	listenerThread.terminate();
*/
	return 0;
}
