#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

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
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Window 256");
	window.setFramerateLimit(30);

	sf::UdpSocket socket;
	sf::Socket::Status status = socket.bind(sf::Socket::AnyPort);//socket.bind(PORTNUM);
	sf::IpAddress serverIp = "128.199.38.220";

	if (status != sf::Socket::Done)
		std::cout << "Could not bind socket" << std::endl;

	socket.send("Hei", sizeof(char) * 3, serverIp, PORTNUM);

	sf::Thread listenerThread(&receive, &socket);
	listenerThread.launch();

	std::size_t messageSize;
	std::string data;

	sf::Event event;

	while (window.isOpen())
	{	
		while(window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::KeyPressed:
					std::cout << "press" << std::endl;//send here, NO send every 0.05 seceond
					data = "pressed";
					if (socket.send(data.c_str(), sizeof(char) * data.length(), serverIp, PORTNUM) != sf::Socket::Done)
						std::cout << "Could not send data to server" << std::endl;
					break;
			}
		}

		window.clear();
		// draw
		window.display();
	}

	return 0;
}
