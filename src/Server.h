#pragma once

#include <unordered_map>
#include <vector>
#include <utility>

#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>

#include "utils/LoopTimer.h"

struct ClientInfo
{
	int id;
	sf::IpAddress address;
	unsigned short port;
	sf::Vector2f position;
};

class Server
{
	public:
		void run();

	private:
		void init();
		void receive();
		void update(float deltaTime);

		void registerClient(sf::IpAddress address, unsigned short port);
		void registerAction(sf::IpAddress address, unsigned short port, std::string action);

		std::unordered_map<std::string, struct ClientInfo> m_clients;
		std::vector<std::pair<int, std::string>> m_actions;

		sf::UdpSocket m_socket;
		sf::Packet m_packet;

		int idCounter;
		LoopTimer m_loopTimer;
};
