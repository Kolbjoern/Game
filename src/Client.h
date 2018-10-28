#pragma once

#include <unordered_map>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Network.hpp>

#include "utils/LoopTimer.h"

struct DrawableObject
{
	float x;
	float y;
	float width;
	int shape;
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

		std::map<int, DrawableObject> m_drawableObjects;
		int m_myObject;

		sf::RenderWindow m_window;
		sf::View m_camera;
		sf::UdpSocket m_socket;
		sf::IpAddress m_serverIp;
		sf::Packet m_packet;

		LoopTimer m_loopTimer;
};
