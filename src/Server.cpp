#include "Server.h"

#include <iostream>

#define PORTNUM 9966
#define MAX_BUFFER 500

void Server::run()
{
	init();

	while (true)
	{
		m_loopTimer.tick();

		receive();

		update(m_loopTimer.getDeltaTime());
		sf::sleep(sf::milliseconds(50));
	}
}

void Server::init()
{
	m_socket.setBlocking(false);

	if (m_socket.bind(PORTNUM) != sf::Socket::Done)
		std::cout << "Could not bind socket" << std::endl;
	
	m_loopTimer.init();

	std::cout << "SERVER::INITIALIZED" << std::endl;
	std::cout << "HOST ADDRESS:" << std::endl;
	std::cout << "public: " << sf::IpAddress::getPublicAddress() << std::endl;
	std::cout << "local: " << sf::IpAddress::getLocalAddress() << std::endl;
}

void Server::receive()
{
	sf::IpAddress sender;
	unsigned short port;
	char buffer[MAX_BUFFER];
	std::size_t messageSize;

	//std::cout << "listen" << std::endl;

	// empty receive buffer
	while (m_socket.receive(buffer, MAX_BUFFER, messageSize, sender, port) == sf::Socket::Done)
	{
		//TODO init this another place before the games starts
		if (m_clients.find(sender.toString() + "::" + std::to_string(port)) == m_clients.end())
		{
			m_clients.emplace(sender.toString() + "::" + std::to_string(port), sf::Vector2f(0, 0));
			std::cout << "New client: " << sender << "::" << port << std::endl;
		}

		std::cout << sender << "::" << port << " - " << buffer << std::endl;
	}
}

void Server::update(float deltaTime)
{
	/*static const float MOVE_ACCELERATION = 500.0f;
	sf::Vector2f newPos = m_body.getPosition();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		newPos.x -= MOVE_ACCELERATION * deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		newPos.x += MOVE_ACCELERATION * deltaTime;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		newPos.y -= MOVE_ACCELERATION * deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		newPos.y += MOVE_ACCELERATION * deltaTime;
		*/
}
