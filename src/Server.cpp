#include "Server.h"

#include <iostream>

#define PORTNUM 9966

void Server::run()
{
	init();

	while (true)
	{
		m_loopTimer.tick();

		receive();

		update(m_loopTimer.getDeltaTime());
		sf::sleep(sf::milliseconds(50));
	}
}

void Server::init()
{
	m_currentObjectId = 0;
	m_socket.setBlocking(false);

	if (m_socket.bind(PORTNUM) != sf::Socket::Done)
		std::cout << "Could not bind socket" << std::endl;
	
	m_loopTimer.init();

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
	std::string content;

	// empty receive buffer
	while (m_socket.receive(m_packet, sender, port) == sf::Socket::Done)
	{
		m_packet >> header;

		switch (header)
		{
			// REGISTRATION
			// TODO: init registration before the game starts
			case 0:
				registerClient(sender, port);
				break;

			// ACTION
			case 1:
				m_packet >> content;
				registerAction(sender, port, content);
				break;
		}

		m_packet.clear();
	}
}

void Server::registerClient(sf::IpAddress address, unsigned short port)
{
	// check whether client already exists
	std::string newClient = address.toString() + ":" + std::to_string(port);
	if (m_clients.find(newClient) == m_clients.end())
	{
		ClientInfo client;
		client.objectId = m_currentObjectId++;
		client.port = port;
		client.address = address;
		m_clients.emplace(newClient, client);

		Object object;
		object.position.x = 0.0f;
		object.position.y = 0.0f;
		m_objects.emplace(client.objectId, object);

		std::cout << "New client added: " << newClient << std::endl;
		
		// send registration info back
		std::string clientId = std::to_string(client.objectId);
		m_packet << clientId;
		m_socket.send(m_packet, client.address, client.port);
		m_packet.clear();
	}
}

void Server::registerAction(sf::IpAddress address, unsigned short port, std::string action)
{
	if (action != "RIGHT" && action != "LEFT" && action != "UP" && action != "DOWN")
		return;

	std::string clientId = address.toString() + ":" + std::to_string(port);
	std::cout << clientId << " - " << action << std::endl;
	std::unordered_map<std::string, struct ClientInfo>::const_iterator client = m_clients.find(clientId);
	if (client != m_clients.end())
	{
		std::pair<int, std::string> clientAction (client->second.objectId, action);
		m_actions.push_back(clientAction);
	}
}

void Server::update(float deltaTime)
{
	// need better iterator?
	for (int i = 0; i < m_actions.size(); i++)
	{
		//std::cout << m_actions[i].first << " did: " << m_actions[i].second << std::endl;
		int objectId = m_actions[i].first;
		std::string action = m_actions[i].second;
		m_actions[i] = m_actions.back();
		m_actions.pop_back();

		static const float MOVE_ACCELERATION = 50.0f;
		sf::Vector2f newPos = m_objects[objectId].position;

		if (action == "LEFT")
			newPos.x -= MOVE_ACCELERATION * deltaTime;

		if (action == "RIGHT")
			newPos.x += MOVE_ACCELERATION * deltaTime;

		if (action == "UP")
			newPos.y -= MOVE_ACCELERATION * deltaTime;

		if (action == "DOWN")
			newPos.y += MOVE_ACCELERATION * deltaTime;

		m_objects[objectId].position = newPos;

		std::cout << "newPos - x:: " << newPos.x << " y:: " << newPos.y << std::endl; 
	}
}
