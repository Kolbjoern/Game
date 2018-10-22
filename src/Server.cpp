#include "Server.h"

#include <iostream>

#include "net/NetHeader.h"
#include "ecs/Systems/PhysicsSystem.h"
#include "ecs/Systems/DeathSystem.h"
#include "ecs/Systems/ActionSystem.h"
#include "factories/ObjectFactory.h"
#include "utils/VectorMath.h"

#define PORTNUM 9966

void Server::run()
{
	init();

	while (true)
	{
		m_loopTimer.tick();
		float iterationTime = m_loopTimer.getDeltaTime();

		receive();

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

	// OSTACLE FOR COLLISION TESTING
	int objId = m_currentObjectId++;
	ObjectFactory::createTestObstacle(objId, m_positionComps, m_graphicsComps, m_collisionComps);

	std::cout << "SERVER::INITIALIZED" << std::endl;
	std::cout << "HOST ADDRESS:" << std::endl;
	std::cout << "public: " << sf::IpAddress::getPublicAddress() << std::endl;
	std::cout << "local: " << sf::IpAddress::getLocalAddress() << std::endl;
}

void Server::receive()
{
	sf::IpAddress sender;
	unsigned short port;
	sf::Uint8 header;

	// empty receive buffer
	while (m_socket.receive(m_packet, sender, port) == sf::Socket::Done)
	{
		m_packet >> header;

		switch (static_cast<NetHeader>(header))
		{
			// TODO: init registration before the game starts
			case NetHeader::Register:
				m_packet.clear();
				
				if (m_clientManager.registerClient(sender, port))
				{
					int objId = m_currentObjectId++;
					ObjectFactory::createPlayer(objId, m_positionComps, m_motionComps, m_graphicsComps, m_collisionComps, m_action1Comps);
					m_clientManager.setObjectToClient(sender, port, objId);

					// send registration info back
					sf::Uint8 header = static_cast<int>(NetHeader::Assign);
					m_packet << header << objId;
					m_socket.send(m_packet, sender, port);
					m_packet.clear();
				}
				break;

			case NetHeader::Action:
				sf::Uint8 action;
				sf::Vector2f direction;
				m_packet >> action;

				if (action == 10)
				{
					sf::Uint8 move;
					m_packet >> move;
					int movement = static_cast<int>(move);

					if (movement & 1)
						direction.x = -1.0f;

					if (movement & 2)
						direction.x = 1.0f;

					if (movement & 4)
						direction.y = -1.0f;

					if (movement & 8)
						direction.y = 1.0f;
				}
				else
					m_packet >> direction.x >> direction.y;

				m_clientManager.registerInput(sender, port, action, direction, m_motionComps, m_action1Comps);
				m_packet.clear();
				break;
		}
	}
}

void Server::update(float deltaTime)
{
	ActionSystem::update(deltaTime, m_currentObjectId, m_action1Comps, m_positionComps, m_motionComps, m_ageComponents, m_graphicsComps);
	PhysicsSystem::update(deltaTime, m_positionComps, m_motionComps, m_collisionComps);
	DeathSystem::update(deltaTime, m_deathRow, m_ageComponents);

	int objId;
	sf::Uint8 header = static_cast<int>(NetHeader::Draw);
	for (std::pair<int, GraphicsComponent> graphic : m_graphicsComps)
	{
		objId = graphic.first;
		for (std::pair<std::string, ClientInfo> client : m_clientManager.getClients())
		{
			if (m_positionComps.find(objId) != m_positionComps.end())
			{
				// send whole list of objects in one package?
				m_packet << header << objId << m_positionComps[objId].x << m_positionComps[objId].y << graphic.second.width << graphic.second.color.r << graphic.second.color.g << graphic.second.color.b;
				m_socket.send(m_packet, client.second.address, client.second.port);
				m_packet.clear();
			}
		}
	}
}

void Server::purgeTheDead()
{
	for (int i = 0; i < m_deathRow.size(); i++)
	{
		int id = m_deathRow[i];
		m_positionComps.erase(id);
		m_motionComps.erase(id);
		m_graphicsComps.erase(id);
		m_collisionComps.erase(id);
		m_ageComponents.erase(id);
		m_action1Comps.erase(id);

		sf::Uint8 header = static_cast<int>(NetHeader::Death);
		m_packet << header << id;

		for (std::pair<std::string, ClientInfo> client : m_clientManager.getClients())
		{
			m_socket.send(m_packet, client.second.address, client.second.port);
		}
		m_packet.clear();
	}
	m_deathRow.clear();
}
