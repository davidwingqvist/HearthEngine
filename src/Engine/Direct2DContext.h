#pragma once
#include <d2d1.h>
#include<wincodec.h>
#include <string>

class D2D1Core
{
private:

	ID2D1Factory* m_factory;
	ID2D1RenderTarget* m_renderTarget;
	ID2D1HwndRenderTarget* m_hwndTarget;
	IDXGISurface* m_surface;

	ID2D1SolidColorBrush* m_solidBrush;

	IWICImagingFactory* m_imageFactory;


	D2D1Core();
	~D2D1Core();

	HRESULT LoadBitMap(
		const LPCWSTR& filePath,
		ID2D1Bitmap** bitMap);

public:

	static auto& Get()
	{
		static D2D1Core instance;
		return instance;
	}


	bool Initialize();

	void Begin();

	void DrawF(const D2D1_RECT_F& rect);

	// Draw picture with default settings
	void DrawP(ID2D1Bitmap* texture = nullptr, const float& opacity = (1.0f));

	// Draw picture combined with 2d object
	void DrawP(const object2D& object, ID2D1Bitmap* texture = nullptr);

	void Commit();


	const bool CreateImage(const std::string& filename, ID2D1Bitmap** pointer = nullptr);

};

