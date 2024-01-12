#pragma once
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

	void Initialize(const HWND& window);

	dx::Keyboard* GetKeyboard() const;
	dx::Mouse* GetMouse() const;

	void Update();

	const bool& CheckKey(const dx::Keyboard::Keys& key) const;
};

