#pragma once

namespace recs
{
	// Forwarding...
	class recs_registry;
	class recs_component_registry;

	class recs_state_handler
	{
	private:

		recs_registry* m_registry = nullptr;
		recs_component_registry* m_compReg = nullptr;

		// {id, {sizeof T, pointer to T}}
		std::unordered_map<size_t, std::pair<size_t, void*>> m_dataTypeReg; // Register of all data types.
		std::string m_fileFolderPath;

	public:

		recs_state_handler(recs_registry* registryRef = nullptr, recs_component_registry* compRegRef = nullptr);

		template<typename T>
		void RegisterData(const T& dataType, void* ptr);

		// Set the folder path for data files.
		void SetFolderPath(const std::string& path);

		void SaveData();
		void LoadData();
	};

	template<typename T>
	inline void recs_state_handler::RegisterData(const T& dataType, void* ptr)
	{
		m_dataTypeReg[typeid(T).hash_code()] = {sizeof(dataType), ptr};
	}
}

