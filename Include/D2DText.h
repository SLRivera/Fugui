#pragma once
#include <dwrite.h>
#include <string>
#include "D3DUtils.h"

#pragma comment(lib, "Dwrite.lib")

namespace Fugui
{
	class D2DText
	{
	public:
		void initText(ID2D_RenderTarget* rt, TCHAR* font, float size, DirectX::XMFLOAT3& color = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DWRITE_FONT_WEIGHT w = DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL);
		void setTextRect(TD2D_RectF& bounds);
		void setText(const TCHAR* str);
		void renderText(ID2D_RenderTarget* rt);

		D2DText();
		~D2DText();

	protected:
		TD2D_RectF		m_drawBounds;

		ID2D_SolidColorBrush*		m_pBrush;

		ID_WriteFactory*		m_pWriteFactory;
		ID_WriteTxtFrmt*	m_pWriteTxtFrmt;

		std::wstring		m_sString;
	};

} // end namespace Fugui