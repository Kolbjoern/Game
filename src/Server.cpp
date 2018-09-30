#include "Server.h"

#include <iostream>

#include "net/NetHeader.h"
#include "ecs/Systems/PhysicsSystem.h"
#include "ecs/Systems/DeathSystem.h"

#define PORTNUM 9966

void Server::run()
{
	init();

	while (true)
	{
		m_loopTimer.tick();

		receive();

		update(m_loopTimer.getDeltaTime());
		purgeTheDead();

		sf::sleep(sf::milliseconds(33.3));
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

	PositionComponent pos;
	pos.x = 600.0f;
	pos.y = 350.0f;
	m_positionComps.emplace(objId, pos);

	GraphicsComponent gra;
	gra.width = 200.0f;
	gra.color = sf::Color(255.0f, 255.0f, 255.0f);
	m_graphicsComps.emplace(objId, gra);

	CollisionComponent col;
	col.width = 200.0f;
	m_collisionComps.emplace(objId, col);

	AgeComponent age;
	age.lifeLived = 0.0f;
	age.lifeTime = 5.0f;
	m_ageComponents.emplace(objId, age);

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
				registerClient(sender, port);
				break;

			case NetHeader::Move:
				sf::Vector2f direction;
				m_packet >> direction.x >> direction.y;
				registerAction(sender, port, direction);
				break;
		}

		m_packet.clear();
	}
}

void Server::registerClient(sf::IpAddress &address, unsigned short &port)
{
	// check whether client already exists
	std::string newClient = address.toString() + ":" + std::to_string(port);
	if (m_clients.find(newClient) == m_clients.end())
	{
		int objId = m_currentObjectId++;

		ClientInfo client;
		client.objectId = objId;
		client.port = port;
		client.address = address;
		m_clients.emplace(newClient, client);

		PositionComponent pos;
		pos.x = 0.0f;
		pos.y = 0.0f;
		m_positionComps.emplace(objId, pos);

		AccelerationComponent acc;
		acc.value = 25.0f;
		m_accelerationComps.emplace(objId, acc);

		VelocityComponent vel;
		vel.x = 0.0f;
		vel.y = 0.0f;
		m_velocityComps.emplace(objId, vel);

		GraphicsComponent gra;
		gra.width = 50.0f;
		gra.color = sf::Color(255, 50, 60);
		m_graphicsComps.emplace(objId, gra);

		CollisionComponent col;
		col.width = 50.0f;
		m_collisionComps.emplace(objId, col);

		std::cout << "New client added: " << newClient << std::endl;
		
		// send registration info back
		std::string clientId = std::to_string(client.objectId);
		m_packet << clientId;
		m_socket.send(m_packet, client.address, client.port);
		m_packet.clear();
	}
}

void Server::registerAction(sf::IpAddress &address, unsigned short &port, sf::Vector2f &direction)
{
	std::string clientId = address.toString() + ":" + std::to_string(port);
	std::unordered_map<std::string, struct ClientInfo>::const_iterator client = m_clients.find(clientId);
	if (client != m_clients.end())
	{
		std::pair<int, sf::Vector2f> clientAction (client->second.objectId, direction);
		m_actions.push_back(clientAction);
	}
}

void Server::update(float deltaTime)
{
	// need better iterator?
	for (int i = 0; i < m_actions.size(); i++)
	{
		int objectId = m_actions[i].first;
		sf::Vector2f direction = m_actions[i].second;

		// remove action
		m_actions[i] = m_actions.back();
		m_actions.pop_back();

		if (m_accelerationComps.find(objectId) != m_accelerationComps.end())
		{
			m_accelerationComps[objectId].dirX = direction.x;
			m_accelerationComps[objectId].dirY = direction.y;
		}
	}

	PhysicsSystem::update(deltaTime, m_deathRow, m_positionComps, m_accelerationComps, m_velocityComps, m_collisionComps);
	DeathSystem::update(deltaTime, m_deathRow, m_ageComponents);

	int objId;
	sf::Uint8 header = static_cast<int>(NetHeader::Draw);
	for (std::pair<int, GraphicsComponent> graphic : m_graphicsComps)
	{
		objId = graphic.first;
		for (std::pair<std::string, ClientInfo> client : m_clients)
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
		m_accelerationComps.erase(id);
		m_velocityComps.erase(id);
		m_graphicsComps.erase(id);
		m_collisionComps.erase(id);
		m_ageComponents.erase(id);

		sf::Uint8 header = static_cast<int>(NetHeader::Death);
		m_packet << header << id;

		for (std::pair<std::string, ClientInfo> client : m_clients)
		{
			m_socket.send(m_packet, client.second.address, client.second.port);
			m_packet.clear();
		}
	}
	m_deathRow.clear();
}
