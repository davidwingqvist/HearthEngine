#pragma once
#include "Renderpass.h"
#include "Shader.h"
#include "BasicPass.h"

/*

	TODO:
	Put update lights in separate function and call it more optimally.

*/

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

	// holds light specfic data, ambient, diffuse, specular...
	DXPointer<ID3D11Buffer> m_lightData;
	DXPointer<ID3D11Buffer> m_lightInfoBuffer;
	std::vector<Light> m_lightDataVector;

	struct LightSpecs
	{
		sm::Vector4 position;
	};
	std::vector<LightSpecs> m_lightWorldSpecs;
	// Holds the position data, and other.
	DXPointer<ID3D11Buffer> m_lightWorldData;

	UINT m_nrOfRegLights = 0;
	sm::Vector4 m_lightInfo = {};

	void SetUpLightBuffer();
	void UpdateLightBuffer();

	// Gather all the lights data.
	void GatherLights(recs::recs_registry* reg);

	/*
	
		Edit values
	
	*/
	bool m_forceUpdate = false;
	Scene* m_currentScene = nullptr;

public:

	BasicPass* m_basicPassRef;

	LightPass(PipelineManager* pipe);
	~LightPass();

	// Inherited via IRenderpass
	void Prepass() override;

	void Pass(Scene* currentScene) override;

	void Postpass() override;

	// Force an update of all lights.
	void ForceLightUpdate();

	// Inherited via IRenderpass
	void Create() override;

};

