#include "Header.h"
#include "InputManager.h"
#include "D3D11Context.h"
#include "Debugger.h"

InputManager::InputManager()
{
	m_keyboard = std::make_unique<dx::Keyboard>();
	m_mouse = std::make_unique<dx::Mouse>();

	m_keyboardTracker.Reset();
	m_mouseTracker.Reset();

	m_mouseX = 0;
	m_mouseY = 0;
	m_scrollValue = 0;
}

void InputManager::Initialize(const HWND& window)
{
	m_mouse->SetWindow(window);
	SetMouseMode(dx::Mouse::MODE_ABSOLUTE);
}

dx::Keyboard* InputManager::GetKeyboard()
{
	return m_keyboard.get();
}

dx::Mouse* InputManager::GetMouse()
{
	return m_mouse.get();
}
const int& InputManager::GetMouseX() const
{
	return m_mouseX;
}

const int& InputManager::GetMouseY() const
{
	return m_mouseY;
}

const int& InputManager::GetScrollValue() const
{
	return m_scrollValue;
}

void InputManager::SetMouseMode(const dx::Mouse::Mode& mode)
{
	m_mouse->SetMode(mode);
}


void InputManager::Update()
{
	const auto mouseState = m_mouse->GetState();

	m_keyboardTracker.Update(m_keyboard->GetState());
	m_mouseTracker.Update(mouseState);

	m_mouseX = mouseState.x;
	m_mouseY = mouseState.y;
	m_scrollValue = mouseState.scrollWheelValue;

	m_mouse->ResetScrollWheelValue();
	//DEBUG_INFO(std::to_string(m_mouseX) + " | " + std::to_string(m_mouseY) + "\n")
}

const bool InputManager::CheckKey(const dx::Keyboard::Keys& key, const key_state& state) const
{
	switch (state)
	{
		case key_state::PRESSED:
			return m_keyboardTracker.IsKeyPressed(key);
		case key_state::RELEASED:
			return m_keyboardTracker.IsKeyReleased(key);
		case key_state::HOLD:
			return m_keyboard->GetState().IsKeyDown(key);
	}
	
	// No state inserted was recognized, return false.
	return false;
}

const bool InputManager::CheckMouseKey(const MouseKey& key, const key_state state)
{
	dx::Mouse::ButtonStateTracker::ButtonState* button = nullptr;
	switch (key)
	{
	case MouseKey::LEFT:
		button = &m_mouseTracker.leftButton;
		break;
	case MouseKey::MIDDLE:
		button = &m_mouseTracker.middleButton;
		break;
	case MouseKey::RIGHT:
		button = &m_mouseTracker.rightButton;
		break;
	default:
		button = &m_mouseTracker.leftButton;
		break;
	}

	switch (state)
	{
	case key_state::PRESSED:
		return *button == m_mouseTracker.PRESSED;
		break;
	case key_state::HOLD:
		return *button == m_mouseTracker.HELD;
		break;
	case key_state::RELEASED:
		return *button == m_mouseTracker.RELEASED;
		break;
	default:
		return false;
		break;
	}

	return false;
}
