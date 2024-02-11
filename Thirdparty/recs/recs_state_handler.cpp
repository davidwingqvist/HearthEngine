#include "recs_pch.h"
#include "recs_state_handler.h"
#include "recs_base.h"
#include <sstream>

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

	bool recs_state_handler::SaveData()
	{
		/*
			Save Components
		*/
		for (auto& data : m_dataTypeReg)
		{
			std::string path = m_fileFolderPath + std::to_string(data.first) + ".txt";
			std::ofstream stream(path, std::ios_base::binary);

			// Couldn't open file, close.
			if (!stream.is_open())
			{
				return false;
			}

			char* buffer = new char[data.second.first];
			char* ptr = (char*)m_compReg->GetComponentArray(data.first);


			for (int i = 0; i < m_registry->GetMaxSize(); i++)
			{
				// Copy data from array to buffer.
				memcpy(buffer, (ptr + (i * data.second.first)), data.second.first);
				stream.write(buffer, data.second.first);

			}
			stream.close();
			delete[] buffer;
		}

		/*
			Save entities
		*/
		{
			std::string path = m_fileFolderPath + "entities.txt";
			std::ofstream stream(path);
			// Couldn't open file, close.
			if (!stream.is_open())
			{
				return false;
			}

			char* buffer = new char[sizeof Entity];
			char* size_buffer = new char[sizeof size_t];


			const Entity_Group& entgrp = m_registry->GetEntities();
			size_t size = entgrp.size();
			memcpy(size_buffer, &size, sizeof size_t);
			stream.write(size_buffer, sizeof size_t);


			delete[] size_buffer;
			for (int i = 0; i < entgrp.size(); i++)
			{
				stream << entgrp[i] << ' ';
			}
			stream.close();
			delete[] buffer;
		}

		/*
			Save entity links
		*/
		{
			for (auto& data : m_dataTypeReg)
			{
				std::string path = m_fileFolderPath + std::to_string(data.first) + "_links.txt";
				std::ofstream stream(path);

				// Couldn't open file, close.
				if (!stream.is_open())
				{
					return false;
				}

				auto& links = m_registry->GetComponentRegistry().GetEntityLinks(data.first);
				// start with size
				stream << links.size() << ' ';
				for (int i = 0; i < links.size(); i++)
				{
					stream << links[i].entity << ' ' << links[i].pos << ' ';
				}
				stream.close();
			}
		}

		return true;
	}

	bool recs_state_handler::LoadData()
	{
		/*
			Load Components
		*/
		{
			for (auto& data : m_dataTypeReg)
			{
				std::string path = m_fileFolderPath + std::to_string(data.first) + ".txt";
				std::ifstream stream(path, std::ios_base::binary);

				// Couldn't open file, close.
				if (!stream.is_open())
				{
					return false;
				}

				char* buffer = new char[data.second.first];
				char* ptr = (char*)m_compReg->GetComponentArray(data.first);

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

		/*
			Load Entities
		*/
		{
			std::string path = m_fileFolderPath + "entities.txt";
			std::ifstream stream(path);

			// Couldn't open file, close.
			if (!stream.is_open())
			{
				return false;
			}

			size_t size = 0;
			char* size_buffer = new char[sizeof size_t];
			stream.read(size_buffer, sizeof size_t);
			memcpy(&size, size_buffer, sizeof size_t);
			delete[] size_buffer;

			size_t read_size = sizeof Entity;
			for (int i = 0; i < size; i++)
			{
				Entity ent = 0;
				stream >> ent;
				m_registry->CreateEntity(ent);
			}
			stream.close();
		}

		/*
			Load entity links
		*/
		{
			auto& compReg = m_registry->GetComponentRegistry();

			for (auto& data : m_dataTypeReg)
			{
				std::string path = m_fileFolderPath + std::to_string(data.first) + "_links.txt";
				std::ifstream stream(path);

				// Couldn't open file, close.
				if (!stream.is_open())
				{
					return false;
				}

				size_t length = 0;
				stream >> length;
				for (int i = 0; i < length; i++)
				{
					size_t pos = 0;
					Entity entity = 0;
					stream >> entity >> pos;

					compReg.LinkEntityToPos(data.first, entity, pos);
				}
				stream.close();
			}
		}

		return true;
	}
}
