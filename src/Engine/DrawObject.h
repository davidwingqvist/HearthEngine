#pragma once

class IDrawObject
{
private:

	bool m_isVisible = true;

public:

	IDrawObject();

	void SetVisibility(const bool& toggle);

	const bool& IsVisible();

	virtual ~IDrawObject() = 0 {};

	virtual void Draw() = 0;

};