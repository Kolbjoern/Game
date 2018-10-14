#pragma once

#include <SFML/Network.hpp>

#include <string>
#include <unordered_map>

#include "../ecs/Components.h"

struct ClientAction
{
	int actionId;
	sf::Vector2f direction;
};

struct ClientInfo
{
	sf::IpAddress address;
	unsigned int port;
	int objectId;
	std::unordered_map<int, struct ClientAction> actions;
};

class ClientManager
{
	public:
		bool registerClient(sf::IpAddress& address, unsigned short port);
		void registerAction(sf::IpAddress& address, unsigned short port, sf::Uint8 action, sf::Vector2f& direction);
		void setObjectToClient(sf::IpAddress& address, unsigned short port, int objectId);
		void update(float deltaTime, int& counter, std::unordered_map<int, MotionComponent>& mot, std::unordered_map<int, PositionComponent>& pos, std::unordered_map<int, AgeComponent>& age, std::unordered_map<int, GraphicsComponent>& gra);

		std::unordered_map<std::string, ClientInfo> getClients();

	private:
		std::string getUniqueId(sf::IpAddress& address, unsigned short port);
		bool clientExists(std::string id);

		std::unordered_map<std::string, ClientInfo> m_clients;
		std::unordered_map<std::string, int> m_actions;
};
