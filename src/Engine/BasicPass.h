#pragma once
#include "Renderpass.h"
#include "Shader.h"
#include "WireFramePass.h"

class BasicPass : public IRenderpass
{
private:

	ID3D11Texture2D* m_colorTexture = nullptr;
	ID3D11ShaderResourceView* m_colorShader = nullptr;
	ID3D11RenderTargetView* m_colorTarget = nullptr;

	ID3D11Texture2D* m_normalsTexture = nullptr;
	ID3D11ShaderResourceView* m_normalsShader = nullptr;
	ID3D11RenderTargetView* m_normalsTarget = nullptr;

	ID3D11Texture2D* m_depthTexture = nullptr;
	ID3D11ShaderResourceView* m_depthShader = nullptr;
	ID3D11DepthStencilView* m_depthTarget = nullptr;

	bool SetUpTextures();
	bool SetUpDepthTexture();

	void ClearRenderTargets();

private:

	WireFramePass m_gridPass;

public:


	BasicPass(PipelineManager* pipe);
	virtual ~BasicPass() override;

	virtual void Prepass() override;
	virtual void Pass(Scene* currentScene) override;
	virtual void Postpass() override;

	// Inherited via IRenderpass
	void Create() override;
};