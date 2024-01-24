#include "Header.h"
#include "CubeMapPass.h"
#include "D3D11Context.h"
#include "Debugger.h"
#include "../../Thirdparty/stb_image/stb_image.h"

bool CubeMapPass::CreateBoxBuffers()
{
	float scale = 1.0f;
	sm::Vector3 vertices[] =
	{
		//Frontside
		{-scale,  scale,  scale},	//Top left		0
		{ scale,  scale,  scale},	//Top right		1
		{-scale, -scale,  scale},	//Down left		2
		{ scale, -scale,  scale},	//Down right	3
		//Backside
		{-scale,  scale, -scale},	//Top left		4
		{ scale,  scale, -scale},	//Top right		5
		{-scale, -scale, -scale},	//Down left		6
		{ scale, -scale, -scale}	//Down right	7
	};

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(vertices);
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices;

	HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&desc, &data, m_vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create vertex buffer for cube map\n");
		return false;
	}

	//Will be used to make faces 
	UINT indices[] =
	{
		//Front
		0, 1, 3,
		3, 2, 0,
		//Back
		5, 4, 6,
		6, 7, 5,
		//Left
		4, 0, 2,
		2, 6, 4,
		//Right
		1, 5, 7,
		7, 3, 1,
		//Top
		4, 5, 1,
		1, 0, 4,
		//Bottom
		2, 3, 7,
		7, 6, 2
	};

	data = {};

	m_nrOfIndices = 36U;
	desc.ByteWidth = sizeof(indices);
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	data.pSysMem = indices;

	hr = D3D11Core::Get().Device()->CreateBuffer(&desc, &data, m_indexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create index buffer for cube map\n");
		return false;
	}

	return true;
}

bool CubeMapPass::CreateBoxTexture()
{
	int width;
	int height;
	int comp;
	int channels = STBI_rgb_alpha;
	unsigned char* image[6];

	std::string folder = "Skybox";
	std::string faces[6] =
	{
		TEXTUREPATH + folder + "/skybox_x.png",
		TEXTUREPATH + folder + "/skybox_z.png",
		TEXTUREPATH + folder + "/skybox_y.png",
		TEXTUREPATH + folder + "/skybox_ground.png",
		TEXTUREPATH + folder + "/skybox_z.png",
		TEXTUREPATH + folder + "/skybox_z.png",
	};

	//Load in the images
	for (int i = 0; i < 6; i++)
	{
		image[i] = stbi_load(faces[i].c_str(), &width, &height, &comp, channels);
	}

	D3D11_TEXTURE2D_DESC desc = {};
	desc.ArraySize = 6;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height = height;
	desc.Width = width;
	desc.MipLevels = 1;
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA data[6] = {};
	for (int i = 0; i < 6; i++)
	{
		data[i].pSysMem = &image[i][0];
		data[i].SysMemPitch = static_cast<UINT>(width * channels);
		data[i].SysMemSlicePitch = 0;
	}

	HRESULT hr = D3D11Core::Get().Device()->CreateTexture2D(&desc, data, m_cubeMap.GetAddressOf());

	//Freeing up the images
	for (int i = 0; i < 6; i++)
	{
		stbi_image_free(image[i]);
	}

	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create cube map texture\n");
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2D.MipLevels = 1;

	hr = D3D11Core::Get().Device()->CreateShaderResourceView(m_cubeMap.Get(), &viewDesc, m_cubeMapView.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create resource view for cube map texture\n");
		return false;
	}

	return true;
}

bool CubeMapPass::CreateSampler()
{
	D3D11_SAMPLER_DESC samplerDesc = {};

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(D3D11Core::Get().Device()->CreateSamplerState(&samplerDesc, m_sampler.GetAddressOf())))
	{
		DEBUG_ERROR("Couldnt create sampler for skybox\n");
		return false;
	}
	return true;
}

bool CubeMapPass::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[1] =
	{
		{"SKYPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HRESULT hr = D3D11Core::Get().Device()->CreateInputLayout(layout, 1, m_vertexShader.GetShaderByteCode().c_str(),
		m_vertexShader.GetShaderByteCode().length(), m_inputLayout.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool CubeMapPass::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	desc.FrontCounterClockwise = false;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = true;
	desc.ScissorEnable = false;
	desc.MultisampleEnable = false;
	desc.AntialiasedLineEnable = false;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	HRESULT hr = D3D11Core::Get().Device()->CreateRasterizerState(&desc, m_rasterState.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool CubeMapPass::CreateDepthStencil()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HRESULT hr = D3D11Core::Get().Device()->CreateDepthStencilState(&depthStencilDesc, m_lessEqualState.GetAddressOf());
	if (FAILED(hr))
		return false;

	return true;
}

CubeMapPass::CubeMapPass()
{
}

CubeMapPass::~CubeMapPass()
{
}

void CubeMapPass::Create()
{
	m_vertexShader.Create("SkyBoxVertexShader");
	m_pixelShader.Create("SkyBoxPixelShader");

	CreateBoxBuffers();
	CreateBoxTexture();
	CreateSampler();
	CreateInputLayout();
	CreateRasterizerState();
	CreateDepthStencil();
}

void CubeMapPass::Prepass()
{
	// Shaders
	DC->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	DC->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// Input Assembly
	DC->IASetInputLayout(m_inputLayout.Get());

	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	const UINT stride = sizeof sm::Vector3;
	const UINT offset = 0;
	DC->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	
	// Output
	DC->OMSetDepthStencilState(m_lessEqualState.Get(), 1);

	// Raster state
	DC->RSSetState(m_rasterState.Get());
	
	// Pixel Shader Values
	DC->PSSetSamplers(0, 1, m_sampler.GetAddressOf());
	DC->PSSetShaderResources(0, 1, m_cubeMapView.GetAddressOf());
}

void CubeMapPass::Pass(Scene* currentScene)
{
	DC->DrawIndexed(m_nrOfIndices, 0, 0);
}

void CubeMapPass::Postpass()
{
	// Reset values for next pass.
	{
		ID3D11RenderTargetView* const m_targets[8] = { nullptr };
		D3D11Core::Get().Context()->OMSetRenderTargets(8, m_targets, nullptr);

		ID3D11ShaderResourceView* temp[D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT] = { 0 };
		D3D11Core::Get().Context()->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT, temp);
	}
}
