#pragma once
#include "D3DUtils.h"


namespace Fugui
{
	class D2DGeometry
	{
	public:
// 		void setColor(float r, float b, float g, float a);
// 		void setColor(DirectX::XMFLOAT4A& c);

		void createSolidColorBrush(TD2D_ColorF& c, ID2D_RenderTarget* rt, float width);
		void createLinearGradBrush(TD2D_ColorF& c, ID2D_RenderTarget* rt, float width);
		void createRadialGradBrush(TD2D_ColorF& c, ID2D_RenderTarget* rt, float width);

		void destroySCB();
		bool isSCBcreated();

		void drawEllipseSCB(ID2D_RenderTarget* rt, float x, float y, float r1, float r2);
		void drawLine(ID2D_RenderTarget* rt, float x1, float y1, float x2, float y2);
		void drawRectangle(ID2D_RenderTarget* rt, float top, float left, float width, float height);
		void drawRoundedRectangle(ID2D_RenderTarget* rt, float top, float left, float width, float height);

		D2DGeometry();
		~D2DGeometry();

	protected:
		TD2D_ColorF			m_cColor;

		ID2D_SolidColorBrush*			m_pSolidColorBrush;
		ID2D_LinearGradientBrush*	m_pLinearGradBrush;
		ID2D_RadialGradientBrush*	m_pRadialGradBrush;

		ID2D_StrokeStyle*			m_pStrokeStyle;

		float		m_fWidth;
	};

} // end namespace Fugui