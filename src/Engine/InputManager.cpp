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
}

void InputManager::Initialize(const HWND& window)
{
	m_mouse->SetWindow(window);
}

dx::Keyboard* InputManager::GetKeyboard() const
{
	return m_keyboard.get();
}

dx::Mouse* InputManager::GetMouse() const
{
	return m_mouse.get();
} 

void InputManager::Update()
{
	m_keyboardTracker.Update(m_keyboard->GetState());
	m_mouseTracker.Update(m_mouse->GetState());
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
