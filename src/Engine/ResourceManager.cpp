#include "Header.h"
#include "ResourceManager.h"

const std::string DEFAULT_PATH = "Options/resourcemanager/";

ResourceManager::ResourceManager()
{
    //if (this->LoadStoredFileItems())
    //{
    //    DEBUG_SUCCESS("Loaded items from file successfully.");
    //}
    //else
    //{
    //    DEBUG_ERROR("Couldnt load one or more items from file.");
    //}
}

ResourceManager::~ResourceManager()
{
}

size_t ResourceManager::GetHashCode(const std::string& filename)
{
    return Get().m_hasher(filename);
}

bool ResourceManager::LoadStoredFileItems()
{
    std::ifstream stream(DEFAULT_PATH);

    if (!stream.is_open())
        return false;

    while (!stream.eof())
    {
        std::string line;
        stream >> line;

    }

    return true;
}

bool ResourceManager::SaveStoredItemsToFile()
{
    return false;
}
