#pragma once
#include "Renderpass.h"
#include "Shader.h"
#include "WireFramePass.h"

class BasicPass : public IRenderpass
{
private:

	DXPointer<ID3D11Texture2D> m_colorTexture;
	DXPointer<ID3D11ShaderResourceView> m_colorShader;
	DXPointer<ID3D11RenderTargetView> m_colorTarget;

	DXPointer<ID3D11Texture2D> m_normalsTexture;
	DXPointer<ID3D11ShaderResourceView> m_normalsShader;
	DXPointer<ID3D11RenderTargetView> m_normalsTarget;

	DXPointer<ID3D11Texture2D> m_depthTexture;
	DXPointer<ID3D11ShaderResourceView> m_depthShader;
	DXPointer<ID3D11DepthStencilView> m_depthTarget;

	bool SetUpTextures();
	bool SetUpDepthTexture();

	void ClearRenderTargets();

private:

	WireFramePass m_gridPass;

public:

	void SetLightPassValues();


	BasicPass(PipelineManager* pipe);
	virtual ~BasicPass() override;

	virtual void Prepass() override;
	virtual void Pass(Scene* currentScene) override;
	virtual void Postpass() override;

	// Inherited via IRenderpass
	void Create() override;
};