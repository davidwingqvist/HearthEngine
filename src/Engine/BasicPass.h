#pragma once
#include "Renderpass.h"
#include "Shader.h"

class BasicPass : public IRenderpass
{
private:

	ID3D11Texture2D* m_colorTexture = nullptr;
	ID3D11Texture2D* m_normalsTexture = nullptr;
	ID3D11ShaderResourceView* m_colorView = nullptr;
	ID3D11ShaderResourceView* m_normalsView = nullptr;
	ID3D11RenderTargetView* m_colorTarget = nullptr;
	ID3D11RenderTargetView* m_normalsTarget = nullptr;

	bool SetUpTextures();

public:


	BasicPass(PipelineManager* pipe);
	virtual ~BasicPass() override;

	virtual void Prepass() override;
	virtual void Pass(Scene* currentScene) override;
	virtual void Postpass() override;

	// Inherited via IRenderpass
	void Create() override;
};