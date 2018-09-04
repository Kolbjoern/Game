#include "Client.h"

#include <SFML/Window/Event.hpp>
#include <SFML/System/String.hpp>

#include <iostream>

#define SERVER_PORT 9966

void Client::run()
{
	init();

	while (m_window.isOpen())
	{
		m_loopTimer.tick();

		handleInput();
		update(m_loopTimer.getDeltaTime());
		render();
	}
}

void Client::init()
{
	if (m_socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
		std::cout << "Could not bind socket" << std::endl;

	m_serverIp = "10.0.0.52";

	m_window.create(sf::VideoMode(1200, 800), "Window 1337");
	m_window.setFramerateLimit(60.0f);

	m_loopTimer.init();

	m_body.setRadius(50.0f);
	m_body.setFillColor(sf::Color(100, 250, 50));
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

				std::cout << "send: " << action << std::endl;
				sf::Socket::Status status = m_socket.send(action.c_str(), 500, m_serverIp, 9966);
				break;
		}
	}
}

void Client::update(float deltaTime)
{
	
}

void Client::render()
{
	m_window.clear();
	m_window.draw(m_body);
	m_window.display();
}
