#pragma once
#include <unordered_map>
#include "Resource.h"
#include "Debugger.h"

class ResourceManager
{
private:

	std::unordered_map<std::string, std::shared_ptr<IResource>> m_resources;

	ResourceManager();
	~ResourceManager();

public:

	static auto& Get()
	{
		static ResourceManager instance;
		return instance;
	}

	template<class T>
	std::shared_ptr<T> GetResource(const std::string& filename, bool createIfFailed = true);
};

template<class T>
inline std::shared_ptr<T> ResourceManager::GetResource(const std::string & filename, bool createIfFailed)
{
	auto f = m_resources.find(filename);
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
			m_resources.emplace(filename, resource);

			return std::shared_ptr<T>(resource);
		}
		else
		{
			Debugger::Get().Print("Resource: " + filename + " couldnt be created.\n", Debugger::COLOR_RED);
		}
	}


	return std::shared_ptr<T>(nullptr);

}
