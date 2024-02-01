#pragma once
#include "Renderpass.h"
#include "Shader.h"
#include "BasicPass.h"

class LightPass : public IRenderpass
{
private:

	VertexShader m_lightVertex;
	PixelShader m_lightPixel;

	DXPointer<ID3D11Buffer> m_vertexBuffer;
	const vertex_data m_screenVertexes[4] = {
		{{1.0f, 1.0f, .0f},   {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{1.0f, -1.0f, .0f},  {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{-1.0f, -1.0f, .0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{-1.0f, 1.0f, .0f},  {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}
	};

	DXPointer<ID3D11Buffer> m_indicesBuffer;
	const UINT m_screenIndices[6] = {
		0,1,2, // Triangle 1
		3,0,2 // Triangle 2
	};

	// Decides if cel shading should be used for the light pass.
	bool m_useCelShading = false;

	bool SetUpScreenTriangles();

	/*
	
		Lightning values

	*/
	DXPointer<ID3D11Buffer> m_lightData;
	std::vector<Light> m_lightDataVector;
	UINT m_nrOfRegLights = (UINT)-1;

	void SetUpLightBuffer();
	void UpdateLightBuffer();

	// Gather all the lights data.
	void GatherLights(recs::recs_registry* reg);

public:

	BasicPass* m_basicPassRef;

	LightPass(PipelineManager* pipe);
	~LightPass();

	// Inherited via IRenderpass
	void Prepass() override;

	void Pass(Scene* currentScene) override;

	void Postpass() override;


	// Inherited via IRenderpass
	void Create() override;

};

