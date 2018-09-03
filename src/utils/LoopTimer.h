#pragma once

#include <SFML/System/Clock.hpp>

class LoopTimer
{
	public:
		void init();
		void tick();

		/*Time between the two latest ticks*/
		float getDeltaTime() const;

	private:
		sf::Clock m_clock;
		sf::Time m_previous;
		sf::Time m_current;
		sf::Time m_elapsed;
};
