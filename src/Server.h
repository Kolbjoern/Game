#pragma once

#include <unordered_map>
#include <vector>
#include <utility>

#include <SFML/Network.hpp>

#include "managers/ClientManager.h"
#include "utils/LoopTimer.h"
#include "ecs/Components.h"

struct Map
{
	float tileSize;
	int width;
	int height;
	std::vector<int> tileData;
};

class Server
{
	public:
		void run();

	private:
		void init();
		void receive();
		void update(float deltaTime);

		void purgeTheDead();

		std::unordered_map<int, PositionComponent> m_positionComps;
		std::unordered_map<int, MotionComponent> m_motionComps;
		std::unordered_map<int, GraphicsComponent> m_graphicsComps;
		std::unordered_map<int, CollisionComponent> m_collisionComps;
		std::unordered_map<int, AgeComponent> m_ageComponents;
		std::unordered_map<int, Action1Component> m_action1Comps;

		std::vector<int> m_deathRow;

		int m_currentObjectId;

		sf::UdpSocket m_socket;
		sf::Packet m_packet;

		ClientManager m_clientManager;
		LoopTimer m_loopTimer;
		Map m_map;
};
