#pragma once

class Time
{
private:

	float m_deltaTime;

	Time();
	~Time();

public:

	static auto& Get()
	{
		static Time instance;
		return instance;
	}

	void SetDeltaTime(const float& deltaTime);
	const float& GetDeltaTime() const;

};