#include "D2DGeometry.h"

using namespace Fugui;
using namespace DirectX;

D2DGeometry::D2DGeometry() :
m_pStrokeStyle(nullptr),
m_pSolidColorBrush(nullptr),
m_pRadialGradBrush(nullptr),
m_pLinearGradBrush(nullptr)
{
}


D2DGeometry::~D2DGeometry()
{
	SafeRelease(m_pStrokeStyle); 
	SafeRelease(m_pRadialGradBrush);
	SafeRelease(m_pLinearGradBrush);
	SafeRelease(m_pSolidColorBrush);
}

void Fugui::D2DGeometry::createSolidColorBrush(TD2D_ColorF& c, ID2D_RenderTarget* rt, float width)
{
	m_cColor = c;
	m_fWidth = width; 
	HR(rt->CreateSolidColorBrush(D2D1::ColorF(m_cColor.r, m_cColor.g, m_cColor.b, m_cColor.a), &m_pSolidColorBrush));
}

void Fugui::D2DGeometry::createLinearGradBrush(TD2D_ColorF& c, ID2D_RenderTarget* rt, float width)
{
	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES p;
	ZeroMemory(&p, sizeof(p));
	p.endPoint.y = p.endPoint.x = 1.0f;
	p.startPoint.x = p.startPoint.y = 0.25f;
//	ID2D1GradientStopCollection *g1;
// 	g1->
// 	D2D1_GRADIENT_STOP g[] = {
// 		{0.0f, D2D1::ColorF(0.2f, 1.0f, 0.2f, 1.0f)},
// 		{0.5f, D2D1::ColorF(1.0f, 0.2f, 0.2f, 1.0f)},
// 		{1.0f, D2D1::ColorF(0.2f, 0.2f, 1.0f, 1.0f)}
// 	};
// 
// 	HR(rt->CreateLinearGradientBrush(p, &g, &m_pLinearGradBrush));
}


void Fugui::D2DGeometry::destroySCB()
{
	SafeRelease(m_pSolidColorBrush);
}


void Fugui::D2DGeometry::drawEllipseSCB(ID2D_RenderTarget* rt, float x, float y, float r1, float r2)
{
	TD2D_PointF c = { x, y };
	TD2D_Ellipse e = { c, r1, r2 };
	rt->DrawEllipse(e, m_pSolidColorBrush, m_fWidth);
}

void Fugui::D2DGeometry::drawRectangle(ID2D_RenderTarget* rt, float top, float left, float width, float height)
{
	D2D1_RECT_F rect;
	rect.top = top;
	rect.left = left;
	rect.bottom = rect.top + width;
	rect.right = rect.left + height;
	//rt->DrawRectangle(rect, m_pSolidColorBrush);
	rt->FillRectangle(rect, m_pSolidColorBrush);
}

bool Fugui::D2DGeometry::isSCBcreated()
{
	if (m_pSolidColorBrush)
		return true;
	else
		return false;
}
