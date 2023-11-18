#include "Header.h"
#include "Debugger.h"

Debugger::Debugger()
{
#if _DEBUG
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	m_color = ConsoleColor::COLOR_WHITE;
	SetConsoleTextAttribute(hConsole, m_color);
#endif
}

Debugger::~Debugger()
{
}

void Debugger::SetColor(const ConsoleColor& color)
{
#if _DEBUG
	SetConsoleTextAttribute(hConsole, color);
	m_color = color;
#endif
}

void Debugger::Print(const std::string& line)
{
#if _DEBUG
	std::cout << line;
#endif
}

void Debugger::Print(const std::string& line, const ConsoleColor& color)
{
#if _DEBUG
	SetConsoleTextAttribute(hConsole, color);
	std::cout << line;
	SetConsoleTextAttribute(hConsole, m_color);
#endif
}
