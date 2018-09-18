#pragma once

#include <unordered_map>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Network.hpp>

#include "utils/LoopTimer.h"

struct DrawableObject
{
	float x;
	float y;
	float width;
	sf::Color color;
};

class Client
{
	public:
		void run();

	private:
		void init();
		void handleInput();
		void update(float deltaTime);
		void receive();
		void render();

		std::unordered_map<int, DrawableObject> m_drawableObjects;

		sf::RenderWindow m_window;
		sf::UdpSocket m_socket;
		sf::IpAddress m_serverIp;
		sf::Packet m_packet;

		LoopTimer m_loopTimer;
};
