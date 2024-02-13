#pragma once
#include <unordered_map>
#include "Resource.h"
#include "Debugger.h"

/*

	TODO:
	* Save texture strings to file.
	
	
*/

class ResourceManager
{
private:

	// {resource Id, resource ptr}
	std::unordered_map<size_t, std::shared_ptr<IResource>> m_resources;

	ResourceManager();
	~ResourceManager();

	std::hash<std::string> m_hasher;

public:

	static auto& Get()
	{
		static ResourceManager instance;
		return instance;
	}

	template<class T>
	std::shared_ptr<T> GetResource(const std::string& filename, bool createIfFailed = true);
	
	template<class T>
	std::shared_ptr<T> GetResource(const size_t& hash_code);

	// Get the hash code for file object.
	size_t GetHashCode(const std::string& filename);

private:

	bool LoadStoredFileItems();
	bool SaveStoredItemsToFile();
};

template<class T>
inline std::shared_ptr<T> ResourceManager::GetResource(const std::string & filename, bool createIfFailed)
{
	auto id = Get().m_hasher(filename);
	auto f = m_resources.find(id);
	if (f != m_resources.end())
	{
		return std::dynamic_pointer_cast<T>(f->second);
	}
	else if (createIfFailed)
	{

		std::shared_ptr<T> resource = std::make_shared<T>();

		if (resource->Create(filename))
		{
			Debugger::Get().Print("Resource: '" + filename + "' created.\n", Debugger::COLOR_GREEN);
			DEBUG_INFO("Total amount of Resources: " + std::to_string(m_resources.size() + 1) + "\n")
			m_resources.emplace(id, resource);

			return std::shared_ptr<T>(resource);
		}
		else
		{
			Debugger::Get().Print("Resource: " + filename + " couldnt be created.\n", Debugger::COLOR_RED);
		}
	}


	return std::shared_ptr<T>(nullptr);

}

template<class T>
inline std::shared_ptr<T> ResourceManager::GetResource(const size_t& hash_code)
{
	auto f = m_resources.find(hash_code);
	if (f != m_resources.end())
	{
		return std::dynamic_pointer_cast<T>(f->second);
	}

	return std::shared_ptr<T>(nullptr);
}
