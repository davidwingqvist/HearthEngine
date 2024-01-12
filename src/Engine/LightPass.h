#pragma once
#include "Renderpass.h"
#include "Shader.h"

class LightPass : public IRenderpass
{
private:

	VertexShader m_lightVertex;
	PixelShader m_lightPixel;

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

