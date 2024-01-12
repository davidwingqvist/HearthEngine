#pragma once

using kb_key = dx::Keyboard::Keys;

enum key_state : UINT
{
	PRESSED = 0,
	RELEASED = 1,
	HOLD = 2
};

class InputManager
{
private:

	std::unique_ptr<dx::Keyboard> m_keyboard{};
	std::unique_ptr<dx::Mouse> m_mouse{};
	dx::Keyboard::KeyboardStateTracker m_keyboardTracker{};
	dx::Mouse::ButtonStateTracker m_mouseTracker{};

	InputManager();
	~InputManager() = default;

public:

	static auto& Get()
	{
		static InputManager instance;
		return instance;
	}

	/*
		Sets the window reference to the mouse.
	*/
	void Initialize(const HWND& window);

	dx::Keyboard* GetKeyboard() const;

	dx::Mouse* GetMouse() const;

	/*
		Updates the states of the keyboard and the mouse.
	*/
	void Update();

	/*
	
		Check the state of a keyboard button specified by 'key'
		Use key_state to check for different kinds of state of the key.
		Default is PRESSED state(it will run one time, when it is pressed).
	*/
	const bool CheckKey(const dx::Keyboard::Keys& key, const key_state& state = key_state::PRESSED) const;
};

