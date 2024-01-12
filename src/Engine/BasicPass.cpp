#include "Header.h"
#include "BasicPass.h"
#include "Debugger.h"
#include "D3D11Context.h"

bool BasicPass::SetUpTextures()
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Format = DXGI_FORMAT_R32G32B32A32_TYPELESS;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	desc.Height = D3D11Core::Get().GetWindow()->GetHeight();
	desc.Width = D3D11Core::Get().GetWindow()->GetWidth();
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc{};
	shaderDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDesc.Texture2D.MostDetailedMip = 1;
	shaderDesc.Texture2D.MipLevels = 1;

	D3D11_RENDER_TARGET_VIEW_DESC targetDesc{};
	targetDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	targetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	HRESULT hr = D3D11Core::Get().Device()->CreateTexture2D(&desc, nullptr, &m_colorTexture);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create color texture for Basic Pass!\n")
			return false;
	}

	hr = D3D11Core::Get().Device()->CreateShaderResourceView(m_colorTexture, &shaderDesc, &m_colorView);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create shader view of color texture for Basic Pass!\n")
			return false;
	}

	hr = D3D11Core::Get().Device()->CreateRenderTargetView(m_colorTexture, &targetDesc, &m_colorTarget);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create render target of color texture for Basic Pass!\n")
			return false;
	}

	hr = D3D11Core::Get().Device()->CreateTexture2D(&desc, nullptr, &m_normalsTexture);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create normals texture for Basic Pass!\n")
			return false;
	}

	hr = D3D11Core::Get().Device()->CreateShaderResourceView(m_normalsTexture, &shaderDesc, &m_normalsView);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create shader view of normals texture for Basic Pass!\n")
			return false;
	}

	hr = D3D11Core::Get().Device()->CreateRenderTargetView(m_normalsTexture, &targetDesc, &m_normalsTarget);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create render target of normals texture for Basic Pass!\n")
			return false;
	}

	return true;
}

BasicPass::BasicPass(PipelineManager* pipe)
	:IRenderpass(pipe)
{
}

BasicPass::~BasicPass()
{
	if (m_colorTexture)
		m_colorTexture->Release();
	if (m_colorView)
		m_colorView->Release();
	if (m_colorTarget)
		m_colorTarget->Release();

	if (m_normalsTexture)
		m_normalsTexture->Release();
	if (m_normalsView)
		m_normalsView->Release();
	if (m_normalsTarget)
		m_normalsTarget->Release();
}

void BasicPass::Prepass()
{
	D3D11Core::Get().Context()->VSSetShader(m_pipeline->m_baseVertexShader.Get(), nullptr, 0);
	D3D11Core::Get().Context()->PSSetShader(m_pipeline->m_basePixelShader.Get(), nullptr, 0);
	D3D11Core::Get().Context()->IASetInputLayout(m_pipeline->m_defaultInputLayout);
	ID3D11RenderTargetView* const m_targets[2] = { m_colorTarget, m_normalsTarget };
	D3D11Core::Get().Context()->OMSetRenderTargets(2, m_targets, m_pipeline->m_depthStencilView);
}

void BasicPass::Pass(Scene* currentScene)
{
	currentScene->Draw();
}

void BasicPass::Postpass()
{

}

void BasicPass::Create()
{
	SetUpTextures();
}
