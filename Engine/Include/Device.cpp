#include "Device.h"

PG_USING

DEFINITION_SINGLE(CDevice)

CDevice::CDevice()	:
	m_pDevice(NULL),
	m_pContext(NULL),
	m_pSwapChain(NULL),
	m_pRTView(NULL),
	m_pDepthBuffer(NULL),
	m_pDSView(NULL),
	m_p2DTarget(NULL),
	m_pD2DFactory(NULL),
	m_pWriteFactory(NULL),
	m_pTextFormat(NULL),
	m_pSolidBrush(NULL)
{
	memset(m_fClearColor, 0, sizeof(float) * 4);
	m_fClearColor[2] = 1.f;
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_pDepthBuffer);
	SAFE_RELEASE(m_pDSView);

	SAFE_RELEASE(m_pRTView);
	SAFE_RELEASE(m_pSwapChain);

	if (m_pContext)
		m_pContext->ClearState();

	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);

	Safe_Release_Map(m_mapText);
	Safe_Release_Map(m_mapBrush);
	SAFE_RELEASE(m_pSolidBrush);
	SAFE_RELEASE(m_p2DTarget);
	SAFE_RELEASE(m_pTextFormat);
	SAFE_RELEASE(m_pWriteFactory);
	SAFE_RELEASE(m_pD2DFactory);
}

ID3D11Device * CDevice::GetDevice() const
{
	return m_pDevice;
}

ID3D11DeviceContext * CDevice::GetContext() const
{
	return m_pContext;
}

IDXGISwapChain * CDevice::GetSwapChain() const
{
	return m_pSwapChain;
}

RESOLUTION CDevice::GetResolution() const
{
	return m_tRS;
}

bool CDevice::Init(HWND hWnd, UINT iWidth, UINT iHeight,
	bool bWindowMode)
{
	m_hWnd = hWnd;
	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	UINT	iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
	iFlag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_FEATURE_LEVEL	eLevel1 = D3D_FEATURE_LEVEL_11_0;
	if (FAILED(D3D11CreateDevice(NULL,
		D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0,
		D3D11_SDK_VERSION,&m_pDevice, &eLevel1, &m_pContext)))
		return false;


	DXGI_SWAP_CHAIN_DESC	tDesc = {};
	
	tDesc.BufferDesc.Width = iWidth;
	tDesc.BufferDesc.Height = iHeight;
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tDesc.BufferCount = 1;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.OutputWindow = hWnd;

	UINT quality = 0;
	HRESULT hr;
	hr = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quality);
	m_pDevice->Release();
	m_pContext->Release();

	/*tDesc.SampleDesc.Quality = quality - 1;
	tDesc.SampleDesc.Count = 4;*/

	tDesc.SampleDesc.Quality = 0;
	tDesc.SampleDesc.Count = 1;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	tDesc.Windowed = bWindowMode; 

	D3D_FEATURE_LEVEL	eLevel = D3D_FEATURE_LEVEL_11_0;
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE, NULL, iFlag, NULL, 0,
		D3D11_SDK_VERSION, &tDesc, &m_pSwapChain,
		&m_pDevice, &eLevel, &m_pContext)))
		return false;

	// ¹é¹öÆÛ¸¦ ¾ò¾î¿Â´Ù.
	ID3D11Texture2D*	pBackBuffer = NULL;

	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(void**)&pBackBuffer);

	// ¾ò¾î¿Â ¹é¹öÆÛ¸¦ ÀÌ¿ëÇØ¼­ ·»´õ¸µ Å¸°Ù ºä¸¦ ¸¸µé¾îÁØ´Ù.
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL,
		&m_pRTView);

	SAFE_RELEASE(pBackBuffer);

	// ±íÀÌ¹öÆÛ¿ë ÅØ½ºÃÄ¸¦ ¸¸µç´Ù.
	D3D11_TEXTURE2D_DESC	tDepthDesc = {};

	tDepthDesc.Width = iWidth;
	tDepthDesc.Height = iHeight;
	tDepthDesc.MipLevels = 1;
	tDepthDesc.ArraySize = 1;
	tDepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, 4, &quality);

	/*tDepthDesc.SampleDesc.Quality = quality - 1;
	tDepthDesc.SampleDesc.Count = 4;*/

	tDepthDesc.SampleDesc.Quality = 0;
	tDepthDesc.SampleDesc.Count = 1;

	tDepthDesc.Usage = D3D11_USAGE_DEFAULT;
	tDepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(m_pDevice->CreateTexture2D(&tDepthDesc, NULL,
		&m_pDepthBuffer)))
		return false;

	// »ý¼ºÇÑ ±íÀÌ¹öÆÛ¸¦ ÀÌ¿ëÇØ¼­ DepthStencilView¸¦ ¸¸µé¾îÁØ´Ù.
	if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthBuffer,
		NULL, &m_pDSView)))
		return false;

	// À§¿¡¼­ »ý¼ºÇÑ ·»´õÅ¸°Ùºä¿Í ±íÀÌ½ºÅÙ½Çºä¸¦ ·»´õ¸µ ÆÄÀÌÇÁ¶óÀÎÀÇ
	// Ãâ·Âº´ÇÕ±â ´Ü°è¿¡ ¹­¾îÁØ´Ù.
	m_pContext->OMSetRenderTargets(1, &m_pRTView, m_pDSView);

	// ºäÆ÷Æ® ¼³Á¤. 
	D3D11_VIEWPORT	tVP = {};

	tVP.Width = iWidth;
	tVP.Height = iHeight;
	tVP.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &tVP);

	D2D1_FACTORY_OPTIONS tOption;
	tOption.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, tOption,
		&m_pD2DFactory)))
		return false;

	IDXGISurface* pBackBufferSurface = NULL;

	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBufferSurface));

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m_pD2DFactory->CreateDxgiSurfaceRenderTarget(pBackBufferSurface, props, &m_p2DTarget)))
		return false;

	SAFE_RELEASE(pBackBufferSurface);

	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pWriteFactory),
		(IUnknown**)&m_pWriteFactory)))
		return false;

	CreateTextFormat("¹ÙÅÁ", L"¹ÙÅÁ", DWRITE_FONT_WEIGHT_DEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED, 18.F, L"ko");

	CreateTextFormat("³ª´®°íµñ", L"³ª´®°íµñ Light", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED, 18.F, L"ko");

	CreateTextFormat("¸¼Àº°íµñ", L"¸¼Àº °íµñ", DWRITE_FONT_WEIGHT_DEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED, 18.F, L"ko");

	CreateTextFormat("¸¼Àº°íµñ35", L"¸¼Àº °íµñ", DWRITE_FONT_WEIGHT_DEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED, 30.F, L"ko");

	CreateTextFormat("µ¸¿ò", L"µ¸¿ò", DWRITE_FONT_WEIGHT_DEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED, 30.F, L"ko");

	CreateTextFormat("µ¸¿ò25", L"µ¸¿ò", DWRITE_FONT_WEIGHT_DEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED, 25.F, L"ko");

	CreateTextBrush("White", 1.f, 1.f, 1.f, 1.f);
	CreateTextBrush("Black", 0.f, 0.f, 0.f, 1.f);
	CreateTextBrush("Yellow", 1.f, 1.f, 0.f, 1.f);
	CreateTextBrush("Blue", 0.f, 0.f, 1.f, 0.5f);
	CreateTextBrush("Red", 1.f, 0.f, 0.f, 1.0f);


	return true;
}

void CDevice::ClearTarget()
{
	m_pContext->ClearRenderTargetView(m_pRTView, m_fClearColor);
	m_pContext->ClearDepthStencilView(m_pDSView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Present()
{
	m_pSwapChain->Present(0, 0);
}

IDWriteTextFormat * CDevice::GetTextFormat() const
{
	return m_pTextFormat;
}

ID2D1RenderTarget * CDevice::Get2DRenderTarget() const
{
	return m_p2DTarget;
}

ID2D1SolidColorBrush * CDevice::GetSolidBrush() const
{
	return m_pSolidBrush;
}
bool CDevice::CreateTextFormat(const string & strKey, const wchar_t * pFontName, int iWeight, int iStyle, int iStretch, float fSize, const wchar_t * pLocalName, int iHAlign, int iVAlign)
{
	IDWriteTextFormat* pFormat = FindTextFormat(strKey);

	if (pFormat)
		return false;

	if (FAILED(m_pWriteFactory->CreateTextFormat(pFontName, NULL, (DWRITE_FONT_WEIGHT)iWeight,
		(DWRITE_FONT_STYLE)iStyle, (DWRITE_FONT_STRETCH)iStretch, fSize, pLocalName, &pFormat)))
		return false;


	// ¹®ÀÚ °£°Ý
	pFormat->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)iHAlign);

	//´Ü¶ô ¸ÂÃã
	pFormat->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)iVAlign);

	m_mapText.insert(make_pair(strKey, pFormat));

	return true;
}

bool CDevice::CreateTextBrush(const string & strKey, float r, float g, float b, float a)
{
	ID2D1SolidColorBrush* pBrush = FindTextBrush(strKey);

	if (pBrush)
		return false;

	if (FAILED(m_p2DTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &pBrush)))
		return false;

	m_mapBrush.insert(make_pair(strKey, pBrush));

	return true;
}

IDWriteTextFormat * CDevice::FindTextFormat(const string & strKey)
{
	unordered_map<string, IDWriteTextFormat*>::iterator iter = m_mapText.find(strKey);

	if (iter == m_mapText.end())
		return NULL;

	return iter->second;
}

ID2D1SolidColorBrush * CDevice::FindTextBrush(const string & strKey)
{
	unordered_map<string, ID2D1SolidColorBrush*>::iterator iter = m_mapBrush.find(strKey);

	if (iter == m_mapBrush.end())
		return NULL;

	return iter->second;
}

void CDevice::RenderText(const string & strTextKey, const string & strBrushKey, const wchar_t * pText, const Vector2 & vStart, const Vector2 & vEnd)
{
	m_p2DTarget->BeginDraw();

	IDWriteTextFormat* pFormat = FindTextFormat(strTextKey);
	ID2D1SolidColorBrush* pBrush = FindTextBrush(strBrushKey);

	m_p2DTarget->DrawTextW(pText, lstrlen(pText), pFormat, D2D1::RectF(vStart.x, vStart.y, vEnd.x, vEnd.y), pBrush);

	m_p2DTarget->EndDraw();
}

void CDevice::RenderText(IDWriteTextFormat * pFormat, ID2D1SolidColorBrush * pBrush, const wchar_t * pText, const Vector2 & vStart, const Vector2 & vEnd)
{
	m_p2DTarget->BeginDraw();

	m_p2DTarget->DrawTextW(pText, lstrlen(pText), pFormat, D2D1::RectF(vStart.x, vStart.y, vEnd.x, vEnd.y), pBrush);

	m_p2DTarget->EndDraw();
}


Vector2 CDevice::GetWindowDeviceResolution() const
{
	RECT	rc;
	GetClientRect(m_hWnd, &rc);
	Vector2	v;
	v.x = m_tRS.iWidth / (float)(rc.right - rc.left);
	v.y = m_tRS.iHeight / (float)(rc.bottom - rc.top);

	return v;
}

Vector2 CDevice::GetWindowResolution() const
{
	RECT	rc;
	GetClientRect(m_hWnd, &rc);

	return Vector2(rc.right - rc.left, rc.bottom - rc.top);
}
