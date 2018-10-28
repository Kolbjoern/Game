#pragma once

#include <SFML/Network.hpp>

#include <string>
#include <unordered_map>

#include "../ecs/Components.h"

struct ClientInfo
{
	sf::IpAddress address;
	unsigned int port;
	int objectId;
};

class ClientManager
{
	public:
		void receive(int& objectCounter, sf::UdpSocket& socket, sf::Packet& packet,
				std::unordered_map<int, PositionComponent>& pos, std::unordered_map<int, MotionComponent>& mot,
				std::unordered_map<int, GraphicsComponent>& gra, std::unordered_map<int, CollisionComponent>& col,
				std::unordered_map<int, Action1Component>& act1);

		void draw(sf::UdpSocket& socket, sf::Packet& packet,
				std::unordered_map<int, PositionComponent>& pos, std::unordered_map<int, GraphicsComponent>& gra);

		void destroyObject(int objectId, sf::UdpSocket& socket, sf::Packet& packet);

	private:
		bool registerClient(sf::IpAddress& address, unsigned short port);
		void registerInput(sf::IpAddress& address, unsigned short port, 
							sf::Uint8 action, sf::Vector2f& direction,
							std::unordered_map<int, MotionComponent>& mot,
							std::unordered_map<int, Action1Component>& act1);
		void setObjectToClient(sf::IpAddress& address, unsigned short port, int objectId);
		int getClientObject(sf::IpAddress& address, unsigned short port);
		std::string getUniqueId(sf::IpAddress& address, unsigned short port);
		bool clientExists(std::string id);

		std::unordered_map<std::string, ClientInfo> m_clients;
};
