#pragma once

class Canvas
{
public:
	/*
		Determines if the canvas is visible to the screen,
		also determines if the canvas should be updated.
	*/
	bool m_isVisible = true;

	void SetInteractive();

	virtual void Draw() = 0;

	virtual void Update() = 0;

	const hrth::BIGINT& GetID() const;

protected:

	Canvas();

private:

	bool m_isInteractive = false;

	std::string m_fileName;

	hrth::BIGINT m_id = -1;

	Canvas* m_parent = nullptr;
};