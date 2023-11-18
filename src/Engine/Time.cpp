#include "Header.h"
#include "Time.h"

Time::Time()
{
    m_deltaTime = 0.0f;
}

Time::~Time()
{
}

void Time::SetDeltaTime(const float& deltaTime)
{
    m_deltaTime = deltaTime;
}

const float& Time::GetDeltaTime() const
{
    return m_deltaTime;
}
