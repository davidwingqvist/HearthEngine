#pragma once
#include "Renderpass.h"
#include "Shader.h"

class BasicPass : public IRenderpass
{
private:



public:


	BasicPass(PipelineManager* pipe);
	virtual ~BasicPass() override;

	virtual void Prepass() override;
	virtual void Pass(Scene* currentScene) override;
	virtual void Postpass() override;
};