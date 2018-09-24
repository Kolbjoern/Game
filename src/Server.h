#pragma once

#include <unordered_map>
#include <vector>
#include <utility>

#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>

#include "utils/LoopTimer.h"
#include "ecs/Systems/PhysicsSystem.h"

struct ClientInfo
{
	int objectId;
	sf::IpAddress address;
	unsigned short port;
};

class Server
{
	public:
		void run();

	private:
		void init();
		void receive();
		void update(float deltaTime);

		void registerClient(sf::IpAddress &address, unsigned short &port);
		void registerAction(sf::IpAddress &address, unsigned short &port, sf::Vector2f &direction);

		std::unordered_map<std::string, struct ClientInfo> m_clients;
		std::vector<std::pair<int, sf::Vector2f>> m_actions;
		std::unordered_map<int, PositionComp> m_positionComps;
		std::unordered_map<int, AccelerationComp> m_accelerationComps;
		std::unordered_map<int, VelocityComp> m_velocityComps;
		std::unordered_map<int, GraphicsComp> m_graphicsComps;
		std::unordered_map<int, CollisionComp> m_collisionComps;

		int m_currentObjectId;

		sf::UdpSocket m_socket;
		sf::Packet m_packet;

		LoopTimer m_loopTimer;
};
