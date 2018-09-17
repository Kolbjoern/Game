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

			// MOVE
			case 1:
				sf::Vector2f acceleration;
				m_packet >> acceleration.x >> acceleration.y;
				registerAction(sender, port, acceleration);
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
		object.position = sf::Vector2f(0.0f, 0.0f);
		object.velocity = sf::Vector2f(0.0f, 0.0f);
		m_objects.emplace(client.objectId, object);

		std::cout << "New client added: " << newClient << std::endl;
		
		// send registration info back
		std::string clientId = std::to_string(client.objectId);
		m_packet << clientId;
		m_socket.send(m_packet, client.address, client.port);
		m_packet.clear();
	}
}

void Server::registerAction(sf::IpAddress address, unsigned short port, sf::Vector2f acceleration)
{
	std::string clientId = address.toString() + ":" + std::to_string(port);
	std::unordered_map<std::string, struct ClientInfo>::const_iterator client = m_clients.find(clientId);
	if (client != m_clients.end())
	{
		std::pair<int, sf::Vector2f> clientAction (client->second.objectId, acceleration);
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
		sf::Vector2f acceleration = m_actions[i].second;
		m_actions[i] = m_actions.back();
		m_actions.pop_back();

		Object &currentObject = m_objects[objectId];
		currentObject.velocity = currentObject.velocity + acceleration;
	}

	for (int j = 0; j < m_objects.size(); j++)
	{
		// update object pos
		Object &currentObject = m_objects[j];
		sf::Vector2f newPos = currentObject.position + currentObject.velocity;
		// VALIDATE AND HANDLE COLLISION
		currentObject.position = newPos;

		// slow down
		currentObject.velocity = currentObject.velocity * 0.95f;
	}

	sf::Uint8 header = 3;//"DRAWABLE"
	for (auto object : m_objects)
	{
		for (auto client : m_clients)
		{
			// send whole list of objects in one package?
			m_packet << header << object.first << object.second.position.x << object.second.position.y;
			m_socket.send(m_packet, client.second.address, client.second.port);
			m_packet.clear();
		}
	}
}
