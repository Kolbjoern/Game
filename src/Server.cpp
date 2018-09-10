#include "Server.h"

#include <iostream>

#define PORTNUM 9966
#define MAX_BUFFER 500

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
	idCounter = 0;
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
	char buffer[MAX_BUFFER];
	std::size_t messageSize;
	std::string message;

	// empty receive buffer
	while (m_socket.receive(buffer, MAX_BUFFER, messageSize, sender, port) == sf::Socket::Done)
	{
		buffer[messageSize] = '\0';
		message = buffer;

		//TODO init this another place before the games starts
		if (message == "REGISTER")
			registerClient(sender, port);

		if (message == "RIGHT" || message == "LEFT" || message == "UP" || message == "DOWN")
		{
			std::string clientId = sender.toString() + ":" + std::to_string(port);

			std::cout << clientId << " - " << message << std::endl;

			std::unordered_map<std::string, struct ClientInfo>::const_iterator client = m_clients.find(clientId);

			if (client != m_clients.end())
			{
				std::pair<int, std::string> action (client->second.id, message);
				m_actions.push_back(action);
			}
		}
	}
}

void Server::registerClient(sf::IpAddress address, unsigned short port)
{
	// check whether client already exists
	std::string newClient = address.toString() + ":" + std::to_string(port);
	if (m_clients.find(newClient) == m_clients.end())
	{
		ClientInfo client;
		client.id = idCounter++;
		client.position.x = 0.0f;
		client.position.y = 0.0f;
		client.port = port;
		client.address = address;

		m_clients.emplace(newClient, client);
		std::cout << "New client added: " << newClient << std::endl;
		
		// send registration info back
		std::string clientId = std::to_string(client.id);
		m_socket.send(clientId.c_str(), clientId.length(), client.address, client.port);
	}
}

void Server::update(float deltaTime)
{
	// need better iterator?
	for (int i = 0; i < m_actions.size(); i++)
	{
		std::cout << m_actions[i].first << " did: " << m_actions[i].second << std::endl;
		m_actions[i] = m_actions.back();
		m_actions.pop_back();

	}
	/*static const float MOVE_ACCELERATION = 500.0f;
	sf::Vector2f newPos = m_body.getPosition();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		newPos.x -= MOVE_ACCELERATION * deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		newPos.x += MOVE_ACCELERATION * deltaTime;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		newPos.y -= MOVE_ACCELERATION * deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		newPos.y += MOVE_ACCELERATION * deltaTime
		*/
}
