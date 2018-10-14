#include "ClientManager.h"

#include <climits>

#include "../utils/VectorMath.h"

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

void ClientManager::registerAction(sf::IpAddress& address, unsigned short port, sf::Uint8 action, sf::Vector2f& direction)
{
	std::string uniqueId = getUniqueId(address, port);
	if (!clientExists(uniqueId))
		return;

	ClientAction act;
	act.actionId = static_cast<int>(action);
	act.direction = direction;

	m_clients[uniqueId].actions.emplace(act.actionId, act);
	m_actions.emplace(uniqueId, m_clients[uniqueId].objectId);
}

void ClientManager::setObjectToClient(sf::IpAddress& address, unsigned short port, int objectId)
{
	std::string uniqueId = getUniqueId(address, port);
	if (!clientExists(uniqueId))
		return;

	m_clients[uniqueId].objectId = objectId;
}

void ClientManager::update(float deltaTime, int& counter, std::unordered_map<int, MotionComponent>& mot, std::unordered_map<int, PositionComponent>& pos, std::unordered_map<int, AgeComponent>& age, std::unordered_map<int, GraphicsComponent>& gra)
{
	for (std::pair<std::string, int> clientAction : m_actions)
	{
		int oId = clientAction.second;
		for (std::pair<int, ClientAction> currentAction : m_clients[clientAction.first].actions)
		{
			ClientAction action = currentAction.second;
			switch (action.actionId)
			{
				// MOVE
				case 10:
					if (mot.find(oId) != mot.end())
						mot[oId].direction = action.direction;
					break;

				// SHOOT
				case 20:
					sf::Vector2f gunslingerPos = sf::Vector2f(pos[oId].x, pos[oId].y);
					sf::Vector2f fireDir = action.direction - gunslingerPos;
					sf::Vector2f normalized = VectorMath::normalize(fireDir);

					int newId = counter++;

					PositionComponent posComp;
					posComp.x = gunslingerPos.x;
					posComp.y = gunslingerPos.y;
					pos.emplace(newId, posComp);

					MotionComponent motComp;
					motComp.velocity = sf::Vector2f(0.0f, 0.0f);
					motComp.direction = normalized;
					motComp.speed = 500.0f;
					motComp.friction = 0.0f;
					mot.emplace(newId, motComp);

					AgeComponent ageComp;
					ageComp.lifeTime = 2.0f;
					ageComp.lifeLived = 0.0f;
					age.emplace(newId, ageComp);

					GraphicsComponent graComp;
					graComp.width = 15.0f;
					graComp.color = sf::Color(10.0f, 255.0f, 50.0f);
					gra.emplace(newId, graComp);
					break;
			}
		}
		m_clients[clientAction.first].actions.clear();
	}
	m_actions.clear();
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
