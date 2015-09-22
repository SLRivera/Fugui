#include "D2DGrafix.h"

using namespace Fugui;

D2DGrafix::D2DGrafix() :
m_pD2DFactory(nullptr),
m_pWicImagingFactory(nullptr),
//m_pD2DDevice(nullptr),
//m_pD2DDeviceContext(nullptr),
m_pD2DRenderTarget(nullptr)
//m_pTargetBitmap(nullptr),
//m_pWriteFactory(nullptr)
{
}


D2DGrafix::~D2DGrafix()
{
	SafeRelease(m_pD2DRenderTarget);
	SafeRelease(m_pWicImagingFactory);
	SafeRelease(m_pD2DFactory);
}

void Fugui::D2DGrafix::initD2D(IDXGI_SwapChain* swapChain)
{
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory));
	HR(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
									IID_IWICImagingFactory,
									reinterpret_cast<void**>(&m_pWicImagingFactory)));

	//TD2D_RenderTargetProperties			rtp;
	ZeroMemory(&m_tRenderTargetProps, sizeof(TD2D_RenderTargetProperties));
	m_tRenderTargetProps.dpiX = m_tRenderTargetProps.dpiY = 0.0f;
	m_tRenderTargetProps.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
	m_tRenderTargetProps.usage = D2D1_RENDER_TARGET_USAGE_FORCE_BITMAP_REMOTING;
	m_tRenderTargetProps.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	m_tRenderTargetProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE);

	IDXGI_Surface*	backBuffer = nullptr;

	HR(swapChain->GetBuffer(0, __uuidof(IDXGI_Surface), (void**)&backBuffer));
	HR(m_pD2DFactory->CreateDxgiSurfaceRenderTarget(backBuffer, m_tRenderTargetProps, &m_pD2DRenderTarget));

	SafeRelease(backBuffer);
}
