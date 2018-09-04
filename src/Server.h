#pragma once

#include <unordered_map>

#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>

#include "utils/LoopTimer.h"

class Server
{
	public:
		void run();

	private:
		void init();
		void receive();
		void update(float deltaTime);

		std::unordered_map<std::string, sf::Vector2f> m_clients;

		sf::UdpSocket m_socket;

		LoopTimer m_loopTimer;
};
