#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "utils/LoopTimer.h"

class Game
{
	public:
		void run();

	private:
		void init();
		void handleInput();
		void update(float deltaTime);
		//void update
		void render();

		sf::RenderWindow m_window;
		sf::CircleShape m_body;

		LoopTimer m_loopTimer;
};
