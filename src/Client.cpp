#include "Client.h"

#include <SFML/Window/Event.hpp>
#include <SFML/System/String.hpp>

#include <iostream>

void Client::run()
{
	init();

	while (m_window.isOpen())
	{
		m_loopTimer.tick();

		handleInput();
		update(m_loopTimer.getDeltaTime());
		receive();
		render();
	}
}

void Client::init()
{
	// bind socket used for connection with server
	sf::Socket::Status status;
	status = m_socket.bind(sf::Socket::AnyPort);
	if (status != sf::Socket::Done)
		std::cout << "Could not bind socket" << std::endl;

	std::cout << "bound to:: " << m_socket.getLocalPort() << std::endl;

	// enter serverip to connect to
	std::cout << "server ip: ";
	std::string input;
	std::getline(std::cin, input);
	m_serverIp = input;//"10.0.0.52"

	//register to server
	sf::Uint8 header = 0;//"REGISTER";
	m_packet << header;
	m_socket.send(m_packet, m_serverIp, 9966);
	m_packet.clear();

	//waiting for server to register us
	sf::IpAddress sender;
	unsigned short port;
	std::cout << "waiting for response..." << std::endl;
	status = m_socket.receive(m_packet, sender, port);
	if (status != sf::Socket::Done)
		std::cout << "Could not register to server" << std::endl;

	std::string clientId;
	m_packet >> clientId;
	std::cout << clientId << std::endl;
	m_packet.clear();

	m_window.create(sf::VideoMode(1200, 800), "Window 1337");
	m_window.setFramerateLimit(60.0f);

	m_socket.setBlocking(false);

	m_loopTimer.init();
}

void Client::handleInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				m_window.close();
				break;

			case sf::Event::KeyPressed:
				std::string action = "";
				if (event.key.code == sf::Keyboard::A)
					action = "LEFT";

				if (event.key.code == sf::Keyboard::D)
					action = "RIGHT";

				if (event.key.code == sf::Keyboard::W)
					action = "UP";

				if (event.key.code == sf::Keyboard::S)
					action = "DOWN";

				std::cout << "send: " << action << " port: " << m_socket.getLocalPort() << std::endl;

				sf::Uint8 header = 1;//"ACTION";
				std::string content = action;
				m_packet << header << content;
				m_socket.send(m_packet, m_serverIp, 9966);
				m_packet.clear();
				break;
		}
	}
}

void Client::update(float deltaTime)
{
	
}

void Client::receive()
{
	sf::IpAddress sender;
	unsigned short port;
	sf::Uint8 header;
	int objectId;
	float xPos;
	float yPos;

	while (m_socket.receive(m_packet, sender, port) == sf::Socket::Done)
	{
		m_packet >> header;

		switch (header)
		{
			// DRAWABLE
			case 3:
				m_packet >> objectId >> xPos >> yPos;
				m_drawableObjects[objectId] = sf::Vector2f(xPos, yPos);
				break;
		}
	}
}

void Client::render()
{
	m_window.clear();

	sf::CircleShape body;
	body.setRadius(50.0f);
	body.setFillColor(sf::Color(100, 250, 50));
	for (auto obj : m_drawableObjects)
	{
		body.setPosition(obj.second);
		m_window.draw(body);
	}

	m_window.display();
}
