#include "Header.h"
#include "Debugger.h"
#include "EngineGUI.h"

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

	ConsoleLogEvent event;
	event.data = line;
	EngineGUI::Get().RegisterConsoleLog(event);
#endif
}

void Debugger::Print(const std::string& line, const ConsoleColor& color)
{
#if _DEBUG
	SetConsoleTextAttribute(hConsole, color);
	std::cout << line;
	SetConsoleTextAttribute(hConsole, m_color);

	ConsoleLogEvent event;
	ImVec4 imColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	switch (color)
	{
	case COLOR_GREEN:
		imColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		event.type = ConsoleLogEventType::LOG_SUCCESS;
		break;
	case COLOR_RED:
		imColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		event.type = ConsoleLogEventType::LOG_ERROR;
		break;
	}

	event.color = imColor;
	event.data = line;
	EngineGUI::Get().RegisterConsoleLog(event);
#endif
}
