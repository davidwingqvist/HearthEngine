#include "recs_pch.h"
#include "recs_state_handler.h"
#include "recs_base.h"

struct Test2
{
	int i = 0;
};

namespace recs
{
	const std::string DEFAULT_STATE_FOLDER = "Options/recs/";

	recs_state_handler::recs_state_handler(recs_registry* registryRef, recs_component_registry* compRegRef)
		:m_registry(registryRef), m_compReg(compRegRef)
	{
		m_fileFolderPath = DEFAULT_STATE_FOLDER;
	}

	void recs_state_handler::SetFolderPath(const std::string& path)
	{
		m_fileFolderPath = path;
	}

	void recs_state_handler::SaveData()
	{
		for (auto& data : m_dataTypeReg)
		{
			char* buffer = new char[data.second.first];
			char* ptr = (char*)m_compReg->GetComponentArray(data.first);
			std::string path = m_fileFolderPath + std::to_string(data.first) + ".txt";
			std::ofstream stream(path);
			for (int i = 0; i < m_registry->GetMaxSize(); i++)
			{
				// Copy data from array to buffer.
				memcpy(buffer, (ptr + (i * data.second.first)), data.second.first);
				stream.write(buffer, data.second.first);
			}
			stream.close();
			delete[] buffer;
		}
	}

	void recs_state_handler::LoadData()
	{
		for (auto& data : m_dataTypeReg)
		{
			char* buffer = new char[data.second.first + 1];
			buffer[data.second.first] = '\0'; // make terminate string.
			char* ptr = (char*)m_compReg->GetComponentArray(data.first);
			std::string path = m_fileFolderPath + std::to_string(data.first) + ".txt";
			std::ifstream stream(path);
			for (int i = 0; i < m_registry->GetMaxSize(); i++)
			{
				stream.read(buffer, data.second.first);
				// Copy data from buffer to array.
				memcpy(ptr + (i * data.second.first), buffer, data.second.first);
			}
			stream.close();
			delete[] buffer;
		}
	}
}
