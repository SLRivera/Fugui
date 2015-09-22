#include "D2DText.h"

using namespace Fugui;

D2DText::D2DText() :
m_pBrush(nullptr),
m_pWriteFactory(nullptr),
m_pWriteTxtFrmt(nullptr)
{
}


D2DText::~D2DText()
{
	SafeRelease(m_pBrush);
	SafeRelease(m_pWriteTxtFrmt);
	SafeRelease(m_pWriteFactory);
}

void D2DText::initText(ID2D_RenderTarget* rt, TCHAR* font, float size, DirectX::XMFLOAT3& color /*= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)*/, DWRITE_FONT_WEIGHT w /*= DWRITE_FONT_WEIGHT_REGULAR*/, DWRITE_FONT_STYLE style /*= DWRITE_FONT_STYLE_NORMAL*/, DWRITE_FONT_STRETCH stretch /*= DWRITE_FONT_STRETCH_NORMAL*/)
{
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(ID_WriteFactory), reinterpret_cast<IUnknown**>(&m_pWriteFactory)));
	HR(m_pWriteFactory->CreateTextFormat(font, NULL, w, style, stretch, size, L"en-us", &m_pWriteTxtFrmt));

	HR(m_pWriteTxtFrmt->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED));
	HR(m_pWriteTxtFrmt->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));

	HR(rt->CreateSolidColorBrush(D2D1::ColorF(color.x, color.y, color.z, 1.0f), &m_pBrush));
	
	m_drawBounds.top = m_drawBounds.left = 0.0f;
	m_drawBounds.right = 100.0f;
	m_drawBounds.bottom = 32.0f;

	m_sString = L"String Not Set!\n";
}

void D2DText::setText(const TCHAR* str)
{
	m_sString = str;
}

void D2DText::setTextRect(TD2D_RectF& bounds)
{
	m_drawBounds = bounds;
}

void D2DText::renderText(ID2D_RenderTarget* rt)
{
	rt->DrawTextW(m_sString.c_str(), m_sString.size(), m_pWriteTxtFrmt, &m_drawBounds, m_pBrush);
}
