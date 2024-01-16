#pragma once
#include "Renderpass.h"
#include "Shader.h"

class WireFramePass : public IRenderpass
{
private:

	/*
	
		GRID VALUES

	*/
	struct point_data
	{
		sm::Vector3 position;
		FLOAT id;
	};
	std::vector<point_data> m_points;
	UINT m_amount = 0;
	std::vector<UINT> m_indices;

	sm::Vector3 m_middlePoint; // Middle point of the grid.
	sm::Vector2 m_size; // Size for both x z
	float m_offset;

	/*
		Build a grid with a point placed with offset between eachother.
	*/
	void BuildGrid(const sm::Vector3& midPoint, const sm::Vector2& size, const float& offset = (float)1.0f);

	/*
	
		DIRECTX VALUES

	*/
	ID3D11InputLayout* m_inputLayout = nullptr;
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	VertexShader m_vertexShader;
	PixelShader m_pixelShader;

	// Graphical Values
	sm::Vector4 m_gridColor;
	ID3D11Buffer* m_gridColorBuffer = nullptr;

	bool CreateShaders();
	bool CreateInput();
	inline bool CreateIndexBuffer();
	inline bool CreateColorBuffer();
	inline bool UpdateColorBuffer();

public:

	WireFramePass();
	~WireFramePass();

	// Inherited via IRenderpass
	void Create() override;
	void Prepass() override;
	void Pass(Scene* currentScene) override;
	void Postpass() override;

	// Set the color of the wireframe grid.
	void SetGridColor(const sm::Vector3& newColor);
};