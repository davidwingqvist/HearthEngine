#include "Header.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

size_t ResourceManager::GetHashCode(const std::string& filename)
{
    return Get().m_hasher(filename);
}
