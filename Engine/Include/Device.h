#pragma once

#include "PGEngine.h"

PG_BEGIN

class PG_DLL CDevice
{
private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRTView;
	ID3D11Texture2D*		m_pDepthBuffer;
	ID3D11DepthStencilView*	m_pDSView;
	float	m_fClearColor[4];
	RESOLUTION				m_tRS;
	HWND					m_hWnd;

public:
	ID3D11Device* GetDevice()	const;
	ID3D11DeviceContext* GetContext()	const;
	IDXGISwapChain* GetSwapChain()	const;
	RESOLUTION GetResolution()	const;

public:
	bool Init(HWND hWnd, UINT iWidth, UINT iHeight,
		bool bWindowMode);
	void ClearTarget();
	void Present();


private:
	ID2D1RenderTarget*	m_p2DTarget; // 그려줄 대상
	ID2D1Factory*		m_pD2DFactory; // Dx2D 리소스를 만드는거
	IDWriteFactory*		m_pWriteFactory; // 글꼴 관련 객체를 만들때 사용
	IDWriteTextFormat*	m_pTextFormat; // 글꼴의 포맷
	ID2D1SolidColorBrush*	m_pSolidBrush; // 영역에 대한 색상을 지정한다.
	unordered_map<string, IDWriteTextFormat*>	m_mapText;
	unordered_map<string, ID2D1SolidColorBrush*>	m_mapBrush;

public:
	IDWriteTextFormat* GetTextFormat()	const;
	ID2D1RenderTarget* Get2DRenderTarget()	const;
	ID2D1SolidColorBrush* GetSolidBrush()	const;

public:
	bool CreateTextFormat(const string& strKey, const wchar_t* pFontName,
		int iWeight, int iStyle, int iStretch, float fSize, const wchar_t* pLocalName,
		int iHAlign = DWRITE_TEXT_ALIGNMENT_LEADING, int iVAlign = DWRITE_PARAGRAPH_ALIGNMENT_FAR);

	bool CreateTextBrush(const string& strKey,
		float r, float g, float b, float a);

	IDWriteTextFormat* FindTextFormat(const string& strKey);
	ID2D1SolidColorBrush* FindTextBrush(const string& strKey);

	void RenderText(const string& strTextKey, const string& strBrushKey,
		const wchar_t* pText, const Vector2& vStart, const Vector2& vEnd);

	void RenderText(IDWriteTextFormat* pFormat, ID2D1SolidColorBrush* pBrush,
		const wchar_t* pText, const Vector2& vStart, const Vector2& vEnd);


public:
	Vector2 GetWindowDeviceResolution()	const;
	Vector2 GetWindowResolution()	const;

	DECLARE_SINGLE(CDevice)
};

PG_END
