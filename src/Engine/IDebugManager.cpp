#include "Header.h"
#include "IDebugManager.h"

void IDebugManager::SetVisiblity()
{
	m_visible = !m_visible;
}

void IDebugManager::SetVisiblity(const bool& visible)
{
	m_visible = visible;
}

IDebugManager::IDebugManager()
{
	m_visible = true;
}
