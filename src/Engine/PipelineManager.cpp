#include "Header.h"
#include "PipelineManager.h"
#include "D3D11Context.h"
#include "Debugger.h"

void PipelineManager::Initialize()
{
    if (!this->CreateRenderTargetView())
        Debugger::Get().Print("Couldnt create RenderTargetView!\n", Debugger::COLOR_RED);
    if (!this->CreateDepthStencilStates())
        DEBUG_ERROR("Couldnt create Depth Stencil States\n")
    if (!this->CreateRasterizerStates())
        DEBUG_ERROR("Couldnt create Rasterizer State\n")
    if (!this->CreateSamplerStates())
        DEBUG_ERROR("Couldnt create Sampler states\n")
    if(!this->CreateShaders())
        DEBUG_ERROR("Couldnt create shaders!\n")
    if(!this->CreateInputLayouts())
        DEBUG_ERROR("Couldnt create Input Layout\n")
    this->SetViewport();
}

PipelineManager::PipelineManager()
{
    m_backBuffer = nullptr;
    m_backBufferAccessView = nullptr;
    m_depthStencilStateLess = nullptr;
    m_depthStencilTexture = nullptr;
    m_depthStencilView = nullptr;
    m_depthBufferSRV = nullptr;
    m_rasterState = nullptr;
    m_blendStateAlphaBlending = nullptr;
    m_anisotropicSamplerState = nullptr;
    m_defaultInputLayout = nullptr;
    m_viewport = {};
}

PipelineManager::~PipelineManager()
{
    if (m_backBuffer)
        m_backBuffer->Release();

    if (m_backBufferAccessView)
        m_backBufferAccessView->Release();

    if (m_depthStencilStateLess)
        m_depthStencilStateLess->Release();

    if (m_depthStencilTexture)
        m_depthStencilTexture->Release();

    if (m_depthStencilView)
        m_depthStencilView->Release();

    if (m_depthBufferSRV)
        m_depthBufferSRV->Release();

    if (m_rasterState)
        m_rasterState->Release();

    if (m_blendStateAlphaBlending)
        m_blendStateAlphaBlending->Release();

    if (m_anisotropicSamplerState)
        m_anisotropicSamplerState->Release();

    if (m_defaultInputLayout)
        m_defaultInputLayout->Release();
}

void PipelineManager::ClearScreen()
{
    const FLOAT clear[4] = { FLOAT(0), FLOAT(0), FLOAT(0), FLOAT(0) };
    D3D11Core::Get().Context()->ClearRenderTargetView(m_backBuffer, clear);
}

bool PipelineManager::CreateRenderTargetView()
{
    ID3D11Texture2D* pBackBuffer = nullptr;

    // Get the pointer to the back buffer.
    if (FAILED(D3D11Core::Get().SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer))))
        return false;

    // Create the renderTargetView with the back buffer pointer.
    HRESULT hr = D3D11Core::Get().Device()->CreateRenderTargetView(pBackBuffer, nullptr, &m_backBuffer);
    D3D11Core::Get().Device()->CreateUnorderedAccessView(pBackBuffer, nullptr, &m_backBufferAccessView);
    D3D11_TEXTURE2D_DESC desc = {};
    pBackBuffer->GetDesc(&desc);
    desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
    // Release pointer to the back buffer.
    pBackBuffer->Release();

    return !FAILED(hr);
}

bool PipelineManager::CreateDepthStencilStates()
{
    // Initialize the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    // Set up the description of the stencil state (Less).
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create m_depthStencilStateLess.
    HRESULT hr = D3D11Core::Get().Device()->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStateLess);
    if (FAILED(hr))
        return false;

    return true;
}

bool PipelineManager::CreateDepthBuffer()
{
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
    depthBufferDesc.Width = D3D11Core::Get().GetWindow()->GetWidth();
    depthBufferDesc.Height = D3D11Core::Get().GetWindow()->GetHeight();
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;
    HRESULT hr = D3D11Core::Get().Device()->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilTexture);
    if (FAILED(hr))
        return false;

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    hr = D3D11Core::Get().Device()->CreateDepthStencilView(m_depthStencilTexture, &depthStencilViewDesc, &m_depthStencilView);
    if (FAILED(hr))
        return false;

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    hr = D3D11Core::Get().Device()->CreateShaderResourceView(m_depthStencilTexture, &shaderResourceViewDesc, &m_depthBufferSRV);
    if (FAILED(hr))
        return false;

    return !FAILED(hr);
}

bool PipelineManager::CreateRasterizerStates()
{
    // Initialize the description of the rasterizer state.
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    // Setup the raster description: how and what polygons will be drawn.
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    // Backface culling refers to the process of discarding back-facing triangles from the pipeline. 
    // This can potentially reduce the amount of triangles that need to be processed by half, hence it will be set as default.
    rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

    // Create the rasterizer state from the description we just filled out.
    HRESULT hr = D3D11Core::Get().Device()->CreateRasterizerState(&rasterizerDesc, &m_rasterState);

    if (FAILED(hr))
        return false;

    return true;
}

bool PipelineManager::CreateSamplerStates()
{
    // Create a sampler state description.
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

    // Setup for Anisotropic SamplerState
    samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerDesc.MinLOD = 0.f;
    samplerDesc.MipLODBias = 0.f;
    samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

    HRESULT hr = D3D11Core::Get().Device()->CreateSamplerState(&samplerDesc, &m_anisotropicSamplerState);
    if (FAILED(hr))
        return false;

    return true;
}

bool PipelineManager::CreateBlendStates()
{
    // Create a blend state description. 
    D3D11_BLEND_DESC blendStateDesc;
    ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

    D3D11_RENDER_TARGET_BLEND_DESC rtbd;
    ZeroMemory(&rtbd, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));

    // Setup Opaque blend state
    rtbd.BlendEnable = 0;
    rtbd.BlendOp = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlend = D3D11_BLEND_ONE;
    rtbd.DestBlend = D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    blendStateDesc.RenderTarget[0] = rtbd;
    blendStateDesc.AlphaToCoverageEnable = 0;
    blendStateDesc.IndependentBlendEnable = 0;

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = FALSE;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;



    HRESULT hr = D3D11Core::Get().Device()->CreateBlendState(&blendDesc, &m_blendStateAlphaBlending);
    if (FAILED(hr))
        return false;

    return true;
}

bool PipelineManager::CreateShaders()
{
    if (!m_basePixelShader.Create("BasicPixelShader"))
        DEBUG_ERROR("Couldnt create BasicPixelShader!\n")
    if (!m_baseVertexShader.Create("BasicVertexShader"))
        DEBUG_ERROR("Couldnt create BasicVertexShader!\n")

	return true;
}

bool PipelineManager::CreateInputLayouts()
{
    HRESULT hr = S_FALSE;

    // Create m_defaultInputLayout.
    std::string shaderByteCode = m_baseVertexShader.GetShaderByteCode();
    D3D11_INPUT_ELEMENT_DESC defaultVertexShaderDesc[] =
    {
        {"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                0,                   D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0,    D3D11_APPEND_ALIGNED_ELEMENT,    D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0,    D3D11_APPEND_ALIGNED_ELEMENT,    D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    if (FAILED(hr = D3D11Core::Get().Device()->CreateInputLayout(defaultVertexShaderDesc, ARRAYSIZE(defaultVertexShaderDesc), shaderByteCode.c_str(), shaderByteCode.length(), &m_defaultInputLayout)))
    {
        DEBUG_ERROR("failed creating m_defaultInputLayout.\n");
        return false;
    }

    return !FAILED(hr);
}

void PipelineManager::SetViewport()
{
    // Initialize the viewport to occupy the entire client area.
    RECT clientRect;
    GetClientRect(D3D11Core::Get().GetWindow()->GetHWnd(), &clientRect);
    const LONG clientWidth = clientRect.right - clientRect.left;
    const LONG clientHeight = clientRect.bottom - clientRect.top;

    ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));

    // Setup the viewport for rendering.
    m_viewport.Width = static_cast<FLOAT>(D3D11Core::Get().GetWindow()->GetWidth());
    m_viewport.Height = static_cast<FLOAT>(D3D11Core::Get().GetWindow()->GetHeight());
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;

    // Direct3D uses a depth buffer range of 0 to 1, hence:
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;

    // Set viewport.
    D3D11Core::Get().Context()->RSSetViewports(1, &m_viewport);
}
