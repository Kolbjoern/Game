#include "LoopTimer.h"

void LoopTimer::init()
{
	m_clock.restart();
	m_previous = m_clock.getElapsedTime();
}

void LoopTimer::tick()
{
	m_current = m_clock.getElapsedTime();
	m_elapsed = m_current - m_previous;
	m_previous = m_current;
}

float LoopTimer::getDeltaTime() const
{
	return m_elapsed.asSeconds();
}
