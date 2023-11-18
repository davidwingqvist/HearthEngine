#include "Header.h"
#include "Direct2DContext.h"
#include "D3D11Context.h"
#include <assert.h>
#include "Debugger.h"



D2D1Core::D2D1Core()
{
	m_factory = nullptr;
	m_renderTarget = nullptr;
	m_hwndTarget = nullptr;
	m_surface = nullptr;
	m_solidBrush = nullptr;
	m_imageFactory = nullptr;
}

D2D1Core::~D2D1Core()
{
	if (m_imageFactory)
		m_imageFactory->Release();
	if (m_hwndTarget)
		m_hwndTarget->Release();
	if (m_surface)
		m_surface->Release();
	if (m_solidBrush)
		m_solidBrush->Release();
	if (m_factory)
		m_factory->Release();
	if (m_renderTarget)
		m_renderTarget->Release();
}

HRESULT D2D1Core::LoadBitMap(const LPCWSTR& filePath, ID2D1Bitmap** bitMap)
{
	HRESULT hr;
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* fDecoder = nullptr;
	IWICFormatConverter* convert = nullptr;

	/*
		Creating Bitmap pipeline.
	*/
	hr = m_imageFactory->CreateDecoderFromFilename(
		filePath,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&decoder
	);
	if (SUCCEEDED(hr))
	{
		hr = decoder->GetFrame(0, &fDecoder);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_imageFactory->CreateFormatConverter(&convert);
	}
	if (SUCCEEDED(hr))
	{
		hr = convert->Initialize(
			fDecoder,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_renderTarget->CreateBitmapFromWicBitmap(
			convert,
			NULL,
			bitMap
		);

		// Release
		if (convert)
			convert->Release();
		if (fDecoder)
			fDecoder->Release();
		if (decoder)
			decoder->Release();

		if (SUCCEEDED(hr))
			return S_OK;
		else
			return E_FAIL;
	}

	//LOG_WARNING("Creating: bitmap texture failed.");

	// Release if anything failed.
	if (convert)
		convert->Release();
	if (fDecoder)
		fDecoder->Release();
	if (decoder)
		decoder->Release();

	return E_FAIL;
}

bool D2D1Core::Initialize()
{
	D2D1_FACTORY_OPTIONS options = {};
#ifdef _DEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	// Create a factory for D2D1, if it fails we LOG_ERROR and return false.
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, options, &m_factory);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Creating D2D1Core factory failed!\n")
		return false;
	}

	D3D11Core::Get().SwapChain()->GetBuffer(0, IID_PPV_ARGS(&m_surface));

	FLOAT dpi = static_cast<FLOAT>(GetDpiForWindow(D3D11Core::Get().GetWindow()->GetHWnd()));
	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpi,
			dpi
		);

	// Create a render target for the surface we created with D3D11 swapchain.
	hr = m_factory->CreateDxgiSurfaceRenderTarget(m_surface, props, &m_renderTarget);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed creating dxgi surface render target!\n")
		return false;
	}

	RECT rc;
	GetClientRect(D3D11Core::Get().GetWindow()->GetHWnd(), &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	hr = m_factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(D3D11Core::Get().GetWindow()->GetHWnd(),
			D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&m_hwndTarget);

	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed creating Hwnd render target!\n")
		return false;
	}

	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_imageFactory));

	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed create Co-instance!\n")
		return false;
	}

	// Setup a default solid color brush to be used when rendering.
	hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &m_solidBrush);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to create default solid color brush!\n")
		return false;
	}


	return true;
}

void D2D1Core::Begin()
{
	this->m_renderTarget->BeginDraw();
}

void D2D1Core::DrawF(const D2D1_RECT_F& rect)
{
	this->m_renderTarget->DrawRectangle(rect, m_solidBrush);
}

void D2D1Core::DrawP(ID2D1Bitmap* texture, const float& opacity)
{
	if (texture == nullptr)
		return;

	m_renderTarget->SetTransform(D2D1::IdentityMatrix());

	D2D1_SIZE_F size = texture->GetSize();

	m_renderTarget->DrawBitmap(texture);
}

void D2D1Core::DrawP(const object2D& object, ID2D1Bitmap* texture)
{
	if (texture == nullptr)
		return;

	m_renderTarget->SetTransform(D2D1::IdentityMatrix());

	D2D1_SIZE_F size = texture->GetSize();

	m_renderTarget->DrawBitmap(texture, { object.x, object.y, object.x + object.width, object.y + object.height });
}

void D2D1Core::Commit()
{
	this->m_renderTarget->EndDraw();
}

const bool D2D1Core::CreateImage(const std::string& filename, ID2D1Bitmap** pointer)
{
	/*
		Setup searchpath, convert char* to WCHAR*
		Load Bitmap from file.
	*/
	std::string searchPath = UIPATH;
	searchPath.append(filename);
	const char* t = searchPath.c_str();
	const WCHAR* pwcsName;

	// conversion
	int nChars = MultiByteToWideChar(CP_ACP, 0, t, -1, NULL, 0);
	pwcsName = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, t, -1, (LPWSTR)pwcsName, nChars);

	HRESULT hr = LoadBitMap(pwcsName, pointer);

	if(FAILED(hr))
		DEBUG_ERROR("Couldnt open '" + filename + "' image!\n")

	delete[] pwcsName;
	return (SUCCEEDED(hr));
}
