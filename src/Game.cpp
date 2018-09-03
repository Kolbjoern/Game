#include "Game.h"

#include <SFML/Window/Event.hpp>

#include <iostream>

void Game::run()
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

void Game::init()
{
	m_window.create(sf::VideoMode(1200, 800), "Window 1337");
	m_window.setFramerateLimit(60.0f);

	m_loopTimer.init();

	m_body.setRadius(50.0f);
	m_body.setFillColor(sf::Color(100, 250, 50));
}

void Game::handleInput()
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

void Game::update(float deltaTime)
{
	static const float MOVE_ACCELERATION = 500.0f;
	sf::Vector2f newPos = m_body.getPosition();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		newPos.x -= MOVE_ACCELERATION * deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		newPos.x += MOVE_ACCELERATION * deltaTime;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		newPos.y -= MOVE_ACCELERATION * deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		newPos.y += MOVE_ACCELERATION * deltaTime;

	m_body.setPosition(newPos);
}

void Game::render()
{
	m_window.clear();
	m_window.draw(m_body);
	m_window.display();
}
