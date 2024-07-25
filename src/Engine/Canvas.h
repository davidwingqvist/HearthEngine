#pragma once

class Canvas
{
public:
	void SetVisibility(const bool& visibility);

	void SetInteractive();

	virtual void Draw() = 0;

	static void DrawObject(const object2D& object);

	const hrth::BIGINT& GetID() const;

protected:

	Canvas();

private:

	bool m_isVisible = true;

	bool m_isInteractive = false;

	std::string m_fileName;

	hrth::BIGINT m_id = -1;

	Canvas* m_parent = nullptr;
};