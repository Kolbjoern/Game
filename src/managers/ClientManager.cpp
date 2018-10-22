#include "ClientManager.h"

#include <climits>

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

std::unordered_map<std::string, ClientInfo> ClientManager::getClients()
{
	return m_clients;
}

std::string ClientManager::getUniqueId(sf::IpAddress& address, unsigned short port)
{
	return address.toString() + ":" + std::to_string(port);
}

bool ClientManager::clientExists(std::string id)
{
	return m_clients.find(id) != m_clients.end();
}
