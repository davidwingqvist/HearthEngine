#pragma once
#include "Renderpass.h"
#include "Shader.h"

class LightPass : public IRenderpass
{
private:

	VertexShader m_lightVertex;
	PixelShader m_lightPixel;

	ID3D11Buffer* m_vertexBuffer = nullptr;
	const vertex_data m_screenVertexes[4] = {
		{{1.0f, 1.0f, .0f},   {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{1.0f, -1.0f, .0f},  {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{-1.0f, -1.0f, .0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{-1.0f, 1.0f, .0f},  {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}
	};

	ID3D11Buffer* m_indicesBuffer = nullptr;
	const UINT m_screenIndices[6] = {
		0,1,2, // Triangle 1
		3,0,2 // Triangle 2
	};

	// Decides if cel shading should be used for the light pass.
	bool m_useCelShading = false;

	bool SetUpScreenTriangles();

public:

	LightPass(PipelineManager* pipe);
	~LightPass();

	// Inherited via IRenderpass
	void Prepass() override;

	void Pass(Scene* currentScene) override;

	void Postpass() override;


	// Inherited via IRenderpass
	void Create() override;

};

