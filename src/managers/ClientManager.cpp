#include "ClientManager.h"

#include <iostream>
#include <climits>

#include "../net/NetHeader.h"
#include "../factories/ObjectFactory.h"

void ClientManager::receive(int& objectCounter, sf::UdpSocket& socket, sf::Packet& packet,
	std::unordered_map<int, PositionComponent>& pos, std::unordered_map<int, MotionComponent>& mot,
	std::unordered_map<int, GraphicsComponent>& gra, std::unordered_map<int, CollisionComponent>& col,
	std::unordered_map<int, Action1Component>& act1)
{
	sf::IpAddress sender;
	unsigned short port;
	sf::Uint8 header;

	// empty receive buffer
	while (socket.receive(packet, sender, port) == sf::Socket::Done)
	{
		packet >> header;

		switch (static_cast<NetHeader>(header))
		{
			// TODO: init registration before the game starts
			case NetHeader::Register:
				packet.clear();

				int objId;
				if (registerClient(sender, port))
				{
					std::cout << "New client registered: " << sender.toString() << "::" << port << std::endl;
					objId = objectCounter++;
					ObjectFactory::createPlayer(objId, pos, mot, gra, col, act1);
					setObjectToClient(sender, port, objId);
				}
				else
				{
					std::cout << "Client rejoined: " << sender.toString() << "::" << port << std::endl;
					objId = getClientObject(sender, port);
				}

				// send registration info back
				header = static_cast<int>(NetHeader::Assign);
				packet << header << objId;
				socket.send(packet, sender, port);
				packet.clear();
				break;

			case NetHeader::Action:
				sf::Uint8 action;
				sf::Vector2f direction;
				packet >> action;

				if (action == 10)
				{
					sf::Uint8 move;
					packet >> move;
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
					packet >> direction.x >> direction.y;

				registerInput(sender, port, action, direction, mot, act1);
				packet.clear();
				break;
		}
	}
}

void ClientManager::draw(sf::UdpSocket& socket, sf::Packet& packet,
	std::unordered_map<int, PositionComponent>& pos, std::unordered_map<int, GraphicsComponent>& gra)
{
	int oId;
	sf::Uint8 header = static_cast<int>(NetHeader::Draw);
	for (std::pair<int, GraphicsComponent> graphic : gra)
	{
		oId = graphic.first;
		for (std::pair<std::string, ClientInfo> client : m_clients)
		{
			if (pos.find(oId) != pos.end())
			{
				// send whole list of objects in one package?
				packet << header << oId << pos[oId].x << pos[oId].y << graphic.second.width << graphic.second.shape << graphic.second.color.r << graphic.second.color.g << graphic.second.color.b;
				socket.send(packet, client.second.address, client.second.port);
				packet.clear();
			}
		}
	}
}

void ClientManager::destroyObject(int objectId, sf::UdpSocket& socket, sf::Packet& packet)
{
	sf::Uint8 header = static_cast<int>(NetHeader::Death);
	packet << header << objectId;
	for (std::pair<std::string, ClientInfo> client : m_clients)
	{
		socket.send(packet, client.second.address, client.second.port);
	}
	packet.clear();
}

bool ClientManager::registerClient(sf::IpAddress& address, unsigned short port)
{
	std::string uniqueId = getUniqueId(address, port);
	if (clientExists(uniqueId))
		return false;

	ClientInfo info;
	info.address = address;
	info.port = port;
	info.objectId = INT_MIN;
	m_clients.emplace(uniqueId, info);

	return true;
}

void ClientManager::registerInput(sf::IpAddress& address, unsigned short port, 
									sf::Uint8 action, sf::Vector2f& direction,
									std::unordered_map<int, MotionComponent>& mot,
									std::unordered_map<int, Action1Component>& act1)
{
	std::string uniqueId = getUniqueId(address, port);
	if (!clientExists(uniqueId))
		return;

	int oId = m_clients[uniqueId].objectId;

	switch (static_cast<int>(action))
	{
		// MOVE
		case 10:
			if (mot.find(oId) != mot.end())
				mot[oId].direction = direction;
			break;

		// ACTIONS
		case 20:
			if (act1.find(oId) != act1.end())
			{
				// enable more actions to fill the action slots
				act1[oId].vector = direction;
				act1[oId].triggered = true;
			}
			break;
	}
}

void ClientManager::setObjectToClient(sf::IpAddress& address, unsigned short port, int objectId)
{
	std::string uniqueId = getUniqueId(address, port);
	if (!clientExists(uniqueId))
		return;

	m_clients[uniqueId].objectId = objectId;
}

int ClientManager::getClientObject(sf::IpAddress& address, unsigned short port)
{
	std::string uniqueId = getUniqueId(address, port);
	return m_clients[uniqueId].objectId;
}

std::string ClientManager::getUniqueId(sf::IpAddress& address, unsigned short port)
{
	return address.toString() + ":" + std::to_string(port);
}

bool ClientManager::clientExists(std::string id)
{
	return m_clients.find(id) != m_clients.end();
}
