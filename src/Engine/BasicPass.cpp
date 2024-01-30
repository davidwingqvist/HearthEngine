#include "Header.h"
#include "BasicPass.h"
#include "Debugger.h"
#include "D3D11Context.h"

bool BasicPass::SetUpTextures()
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Format = DXGI_FORMAT_R16G16B16A16_TYPELESS;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	desc.Height = D3D11Core::Get().GetWindow()->GetHeight();
	desc.Width = D3D11Core::Get().GetWindow()->GetWidth();
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.MipLevels = 1;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc{};
	shaderDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDesc.Texture2D.MostDetailedMip = 0;
	shaderDesc.Texture2D.MipLevels = 1;

	D3D11_RENDER_TARGET_VIEW_DESC targetDesc{};
	targetDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	targetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	/*
	
		ALBEDO TEXTURE

	*/

	HRESULT hr = D3D11Core::Get().Device()->CreateTexture2D(&desc, nullptr, m_colorTexture.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create color texture for Basic Pass!\n")
			return false;
	}

	hr = D3D11Core::Get().Device()->CreateShaderResourceView(m_colorTexture.Get(), &shaderDesc, m_colorShader.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create shader view of color texture for Basic Pass!\n")
			return false;
	}

	hr = D3D11Core::Get().Device()->CreateRenderTargetView(m_colorTexture.Get(), &targetDesc, m_colorTarget.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create render target of color texture for Basic Pass!\n")
			return false;
	}

	/*
	
		NORMAL TEXTURE
	
	*/

	hr = D3D11Core::Get().Device()->CreateTexture2D(&desc, nullptr, m_normalsTexture.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create normals texture for Basic Pass!\n")
			return false;
	}

	hr = D3D11Core::Get().Device()->CreateShaderResourceView(m_normalsTexture.Get(), &shaderDesc, m_normalsShader.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create shader view of normals texture for Basic Pass!\n")
			return false;
	}

	hr = D3D11Core::Get().Device()->CreateRenderTargetView(m_normalsTexture.Get(), &targetDesc, m_normalsTarget.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create render target of normals texture for Basic Pass!\n")
			return false;
	}

	/*
	
		WORLD POSITION TEXTURE

	*/

	hr = D3D11Core::Get().Device()->CreateTexture2D(&desc, nullptr, m_worldPositionTexture.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create world position texture for Basic Pass!\n")
			return false;
	}

	hr = D3D11Core::Get().Device()->CreateShaderResourceView(m_worldPositionTexture.Get(), &shaderDesc, m_worldPositionShader.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create shader view of world position texture for Basic Pass!\n")
			return false;
	}

	hr = D3D11Core::Get().Device()->CreateRenderTargetView(m_worldPositionTexture.Get(), &targetDesc, m_worldPositionTarget.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create render target of normals texture for Basic Pass!\n")
			return false;
	}

	return true;
}

bool BasicPass::SetUpDepthTexture()
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	desc.Height = D3D11Core::Get().GetWindow()->GetHeight();
	desc.Width = D3D11Core::Get().GetWindow()->GetWidth();
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc{};
	shaderDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDesc.Texture2D.MostDetailedMip = 1;
	shaderDesc.Texture2D.MipLevels = 1;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc{};
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Texture2D.MipSlice = 0;


	HRESULT hr = D3D11Core::Get().Device()->CreateTexture2D(&desc, nullptr, m_depthTexture.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to create depth texture for Basic Pass!\n")
			return false;
	}
	hr = D3D11Core::Get().Device()->CreateShaderResourceView(m_depthTexture.Get(), &shaderDesc, m_depthShader.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to create shader view of depth texture for Basic Pass!\n")
			return false;
	}
	hr = D3D11Core::Get().Device()->CreateDepthStencilView(m_depthTexture.Get(), &depthDesc, m_depthTarget.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to create depth stencil view of depth texture for Basic Pass!\n")
			return false;
	}

	return true;
}

void BasicPass::ClearRenderTargets()
{
	ID3D11ShaderResourceView* temp[D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT] = { 0 };
	D3D11Core::Get().Context()->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT, temp);

	ID3D11RenderTargetView* const m_targets[8] = { nullptr };
	D3D11Core::Get().Context()->OMSetRenderTargets(8, m_targets, nullptr);

	const FLOAT clear[4] = { FLOAT(0), FLOAT(0), FLOAT(0), FLOAT(0) };
	D3D11Core::Get().Context()->ClearRenderTargetView(m_colorTarget.Get(), clear);
	D3D11Core::Get().Context()->ClearRenderTargetView(m_normalsTarget.Get(), clear);
	D3D11Core::Get().Context()->ClearDepthStencilView(m_depthTarget.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void BasicPass::SetLightPassValues()
{
	// Prepare for Light pass input.
	ID3D11ShaderResourceView* const m_views[4] = { m_colorShader.Get(), m_normalsShader.Get(), m_depthShader.Get(), m_worldPositionShader.Get()};
	D3D11Core::Get().Context()->PSSetShaderResources(0, 4, m_views);
}

BasicPass::BasicPass(PipelineManager* pipe)
	:IRenderpass(pipe)
{
}

BasicPass::~BasicPass()
{
}

void BasicPass::Prepass()
{
	ClearRenderTargets();
	D3D11Core::Get().Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11Core::Get().Context()->VSSetShader(m_pipeline->m_baseVertexShader.Get(), nullptr, 0);
	D3D11Core::Get().Context()->PSSetShader(m_pipeline->m_basePixelShader.Get(), nullptr, 0);
	D3D11Core::Get().Context()->IASetInputLayout(m_pipeline->m_defaultInputLayout.Get());
	ID3D11RenderTargetView* const m_targets[] = { m_colorTarget.Get(), m_normalsTarget.Get(), m_worldPositionTarget.Get()};
	D3D11Core::Get().Context()->OMSetRenderTargets(3, m_targets, m_depthTarget.Get());
	DC->RSSetState(m_pipeline->m_rasterState.Get());
	DC->OMSetBlendState(nullptr, NULL, 0xffffffff);
	DC->PSSetSamplers(0, 1, m_pipeline->m_anisotropicSamplerState.GetAddressOf());
}

void BasicPass::Pass(Scene* currentScene)
{
	currentScene->Draw();
	m_gridPass.Prepass();
	m_gridPass.Pass(currentScene);
	m_gridPass.Postpass();
}

void BasicPass::Postpass()
{
	// Reset values for next pass.
	{
		ID3D11RenderTargetView* const m_targets[8] = { nullptr };
		D3D11Core::Get().Context()->OMSetRenderTargets(8, m_targets, nullptr);

		ID3D11ShaderResourceView* temp[D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT] = { 0 };
		D3D11Core::Get().Context()->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT, temp);
	}

	DC->OMSetRenderTargets(1, m_colorTarget.GetAddressOf(), m_depthTarget.Get());
}

void BasicPass::Create()
{
	SetUpTextures();
	SetUpDepthTexture();
	m_gridPass.Create();
}
