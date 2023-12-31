#include "Header.h"
#include "D3D11Context.h"

D3D11Core::D3D11Core()
    :m_device(nullptr),m_context(nullptr),m_swapChain(nullptr)
{
    m_window = nullptr;
}

D3D11Core::~D3D11Core()
{
    if (m_device)
        m_device->Release();
    if (m_context)
        m_context->Release();
    if (m_swapChain)
        m_swapChain->Release();
}

void D3D11Core::Initialize(Window* pWindow)
{
    if (this->m_window == nullptr)
        this->m_window = pWindow;

    this->CreateContext();
}

ID3D11Device* D3D11Core::Device() const
{
    return this->m_device;
}

ID3D11DeviceContext* D3D11Core::Context() const
{
    return this->m_context;
}

IDXGISwapChain* D3D11Core::SwapChain() const
{
    return this->m_swapChain;
}

Window* D3D11Core::GetWindow() const
{
    return this->m_window;
}

void D3D11Core::Present()
{
    m_swapChain->Present(0, 0);
}

bool D3D11Core::CreateContext()
{
    // Initialize the swap chain description.
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    // Set the handle for the window to render to.
    swapChainDesc.OutputWindow = this->m_window->GetHWnd();

    // Set to windowed mode.
    swapChainDesc.Windowed = TRUE;

    // Set the client dimensions of the back buffer.
    swapChainDesc.BufferDesc.Width = this->m_window->GetWidth();
    swapChainDesc.BufferDesc.Height = this->m_window->GetHeight();

    // Set to a duo back buffer.
    swapChainDesc.BufferCount = 2;

    // Set the refresh rate of the back buffer.
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

    // Set regular 32-bit surface for the back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // Set the scan line ordering and scaling to unspecified.                             
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Set the usage of the back buffer.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_SHADER_INPUT;

    // Turn multisampling off.
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    // Discard the contents of the back buffer after IDXGISwapChain1::Present() call.
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    // Set this flag to enable an application to switch modes by calling IDXGISwapChain::ResizeTarget.
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // Creates a m_device that supports the debug layer. 
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // Set the feature level to dx 11.
    D3D_FEATURE_LEVEL featureLevels[]
    {
        D3D_FEATURE_LEVEL_11_0
    };

    // Create the m_device, swap chain and m_device context.
    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION,
        &swapChainDesc, &this->m_swapChain, &this->m_device, nullptr, &this->m_context);

    return !FAILED(hr);
}
