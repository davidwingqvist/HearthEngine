#include "Header.h"
#include "ResourceManager.h"
#include "Texture.h"
#include <sstream>

const std::string DEFAULT_PATH = "Options/resourcemanager/";

ResourceManager::ResourceManager()
{
    if (this->LoadStoredFileItems())
    {
        DEBUG_SUCCESS("Loaded items from file successfully.");
    }
    else
    {
        DEBUG_ERROR("Couldnt load one or more items from file.");
    }
}

ResourceManager::~ResourceManager()
{
    this->SaveStoredItemsToFile();
}

size_t ResourceManager::GetHashCode(const std::string& filename)
{
    return Get().m_hasher(filename);
}

void ResourceManager::ClearResources()
{
    std::ofstream ofs;
    ofs.open(DEFAULT_PATH + "resources.txt", std::ofstream::out | std::ofstream::trunc);
    
    if (!ofs.is_open())
    {
        DEBUG_ERROR("Couldnt clear resources from file")
    }
    
    ofs.close();

    m_resources.clear();
}

bool ResourceManager::LoadStoredFileItems()
{
    std::ifstream stream(DEFAULT_PATH + "resources.txt", std::ios_base::in);

    if (!stream.is_open())
        return false;

    std::string line;
    while (std::getline(stream, line))
    {
        std::istringstream iss(line);

        std::string type;
        std::string name;

        iss >> type;
        iss >> name;

        size_t id = this->m_hasher(name);

        if (type == "Model")
        {
            std::shared_ptr<Model3D> newModel = std::make_shared<Model3D>();
            
            if (newModel->Create(name))
            {
                DEBUG_SUCCESS("Resource: '" + name + "' created.");
            }
            else
            {
                DEBUG_ERROR("Resource: '" + name + "' failed to be created.");
            }

            m_resources.emplace(id, newModel);

            continue;
        }
        else if (type == "Texture")
        {
            std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();
            

            if (newTexture->Create(name))
            {
                DEBUG_SUCCESS("Resource: '" + name + "' created.");
            }
            else
            {
                DEBUG_ERROR("Resource: '" + name + "' failed to be created.");
            }

            m_resources.emplace(id, newTexture);

            continue;
        }
    }

    stream.close();

    return true;
}

bool ResourceManager::SaveStoredItemsToFile()
{
    std::ofstream stream(DEFAULT_PATH + "resources.txt", std::ios_base::out);

    if (!stream.is_open())
    {
        return false;
    }

    for (auto& resource : m_resources)
    {
        std::string type;

        if (dynamic_cast<Model3D*>(resource.second.get()))
        {
            type = "Model";
        }
        else if (dynamic_cast<Texture*>(resource.second.get()))
        {
            type = "Texture";
        }

        stream << type << ' ' << resource.second.get()->GetName() << "\n";
    }

    stream.close();

    return true;
}
