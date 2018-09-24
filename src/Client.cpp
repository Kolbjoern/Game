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
		}
	}
}

void Client::update(float deltaTime)
{
	sf::Vector2f direction(0.0f, 0.0f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		direction.x = -1.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		direction.x = 1.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		direction.y = -1.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		direction.y = 1.0f;

	if (direction.x != 0 || direction.y != 0)
	{
		sf::Uint8 header = 1;//"MOVE";
		m_packet << header << direction.x << direction.y;
		m_socket.send(m_packet, m_serverIp, 9966);
		m_packet.clear();
	}
}

void Client::receive()
{
	sf::IpAddress sender;
	unsigned short port;
	sf::Uint8 header;
	int objectId;

	while (m_socket.receive(m_packet, sender, port) == sf::Socket::Done)
	{
		m_packet >> header;

		switch (header)
		{
			// DRAWABLE
			case 3:
				DrawableObject obj;
				m_packet >> objectId >> obj.x >> obj.y >> obj.width >> obj.color.r >> obj.color.g >> obj.color.b;
				m_drawableObjects[objectId] = obj;
				break;
		}
	}
}

void Client::render()
{
	m_window.clear();

	sf::CircleShape body;
	for (std::pair<int, DrawableObject> obj : m_drawableObjects)
	{
		body.setPosition(obj.second.x - obj.second.width/2.0f, obj.second.y - obj.second.width/2.0f);
		body.setFillColor(obj.second.color);
		body.setRadius(obj.second.width/2.0f);
		m_window.draw(body);
	}

	m_window.display();
}
