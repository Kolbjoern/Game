#include "Server.h"

#include <iostream>

#include "ecs/Systems/PhysicsSystem.h"
#include "ecs/Systems/DeathSystem.h"
#include "ecs/Systems/ActionSystem.h"
#include "factories/ObjectFactory.h"

#define PORTNUM 9966

void Server::run()
{
	init();

	while (true)
	{
		m_loopTimer.tick();
		float iterationTime = m_loopTimer.getDeltaTime();

		update(iterationTime);
		purgeTheDead();

		sf::sleep(sf::milliseconds(16.66f - iterationTime/1000));
	}
}

void Server::init()
{
	m_currentObjectId = 0;
	m_socket.setBlocking(false);

	if (m_socket.bind(PORTNUM) != sf::Socket::Done)
		std::cout << "Could not bind socket" << std::endl;
	
	m_loopTimer.init();

	m_map.tileSize = 16.0f;
	m_map.width = 3;
	m_map.height = 3;
	m_map.tileData.resize(m_map.width * m_map.height);
	m_map.tileData[0] = 1;
	m_map.tileData[1] = 0;
	m_map.tileData[2] = 1;
	m_map.tileData[3] = 1;
	m_map.tileData[4] = 0;
	m_map.tileData[5] = 0;
	m_map.tileData[6] = 1;
	m_map.tileData[7] = 1;
	m_map.tileData[8] = 1;

	int index;
	int tileObjId;
	int tileSize = m_map.tileSize;
	/*for (int x = 0; x < m_map.width; x++)
	{
		for (int y = 0; y < m_map.height; y++)
		{
			tileObjId = m_currentObjectId++;
			index = y * m_map.width + x;

			ObjectFactory::createTile(tileObjId, m_positionComps, m_graphicsComps, m_collisionComps, m_healthComps);
			m_positionComps[tileObjId].x = (x * tileSize);
			m_positionComps[tileObjId].y = (y * tileSize);

			if (m_map.tileData[index] == 0)
				m_graphicsComps[tileObjId].color = sf::Color(0.0f, 0.0f, 0.0f);
			else
				m_graphicsComps[tileObjId].color = sf::Color(255.0f, 255.0f, 255.0f);

			m_graphicsComps[tileObjId].width = tileSize;
			m_graphicsComps[tileObjId].height = tileSize;
		}
	}*/

	// OSTACLE FOR COLLISION TESTING
	int objId = m_currentObjectId++;
	ObjectFactory::createTestObstacle(objId, m_positionComps, m_shapeComps, m_healthComps);

	std::cout << "SERVER::INITIALIZED" << std::endl;
	std::cout << "HOST ADDRESS:" << std::endl;
	std::cout << "public: " << sf::IpAddress::getPublicAddress() << std::endl;
	std::cout << "local: " << sf::IpAddress::getLocalAddress() << std::endl;
}

void Server::update(float deltaTime)
{
	m_clientManager.receive(m_currentObjectId, m_socket, m_packet, m_positionComps, m_motionComps, m_shapeComps, m_action1Comps);

	ActionSystem::update(deltaTime, m_currentObjectId, m_action1Comps, m_positionComps, m_motionComps, m_ageComps, m_shapeComps, m_healthComps);
	PhysicsSystem::update(deltaTime, m_positionComps, m_motionComps, m_shapeComps, m_healthComps);
	DeathSystem::update(deltaTime, m_deathRow, m_ageComps, m_healthComps);

	m_clientManager.draw(m_socket, m_packet, m_positionComps, m_shapeComps);
}

void Server::purgeTheDead()
{
	for (int i = 0; i < m_deathRow.size(); i++)
	{
		int id = m_deathRow[i];
		m_positionComps.erase(id);
		m_motionComps.erase(id);
		m_shapeComps.erase(id);
		m_ageComps.erase(id);
		m_healthComps.erase(id);
		m_action1Comps.erase(id);

		m_clientManager.destroyObject(id, m_socket, m_packet);
	}
	m_deathRow.clear();
}
