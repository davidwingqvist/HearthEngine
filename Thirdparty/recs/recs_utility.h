#pragma once
#include <utility>
#include <functional>
#include <fstream>
#include <string>
#include <iostream>

namespace recs
{
	/*
		Turn a string into a hashed integer.
	*/
	//const int hash_string(const char* str)
	//{
	//	std::hash<std::string> hasher;
	//	const int hashed = hasher(str);
	//	return hashed;
	//}

	/*
		Create a lua file out of the filePath and name.
	*/
	void create_lua_file(const std::string& filePath, const std::string& fileName)
	{
		const std::string file = filePath + fileName + ".lua";

		std::fstream writer;
		writer.open(file, std::ios_base::out);
		writer << "-- This is " + fileName + "'s lua file.\n\n";
		writer << "local " + fileName + " = {}\n";
		writer << "\nfunction " + fileName + "::Start()\n";
		writer << "{\n\t\n}\n";
		writer << "\n\nfunction " + fileName + "::Update()\n{\n\t\n}\n";
		writer << "\nreturn " + fileName;

		writer.close();
	}
}