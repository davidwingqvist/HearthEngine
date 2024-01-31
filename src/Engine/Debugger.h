#pragma once
#include <iostream>
#include <Windows.h>

class Debugger
{
private:

	Debugger();
	~Debugger();
	HANDLE hConsole;
	WORD m_color;

public:

	enum ConsoleColor : WORD
	{

		COLOR_GREEN = 10,
		COLOR_WHITE = 15,
		COLOR_RED = 12,
		COLOR_PURPLE = 13

	};

	static auto& Get()
	{
		static Debugger instance;
		return instance;
	}

	void SetColor(const ConsoleColor& color);
	void Print(const std::string& line);
	void Print(const std::string& line, const ConsoleColor& color);
	void AdjustPrint(const std::string& line);

};

#define DEBUG_ERROR(string) Debugger::Get().Print(string, Debugger::COLOR_RED);
#define DEBUG_SUCCESS(string) Debugger::Get().Print(string, Debugger::COLOR_GREEN);
#define DEBUG_INFO(string) Debugger::Get().Print(string, Debugger::COLOR_WHITE);

#define DEBUG_SAMELINE(string) Debugger::Get().AdjustPrint(string);
#define DEBUG_NEWLINE() Debugger::Get().Print("\n");