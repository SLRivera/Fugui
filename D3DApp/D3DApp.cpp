#include "D3DApp.h"
#include "Timer.h"
using namespace Fugui;

LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

D3DApp::D3DApp() :
m_sClassName(L"NotSet"),
m_sVideoCardName(L"NotSet"),
m_sWindowTitle(L"NotSet"),
m_hWnd(nullptr),
m_hInstance(nullptr),
m_nHeight(0),
m_nWidth(0),
m_vOutputSize(0.0f, 0.0f),
m_vBackgroundColor(0.0f, 0.0f, 0.0f, 1.0f),
m_bFullScreen(false),
m_bAppRun(false),
m_pD3DDevice(nullptr),
m_pD3DDeviceContext(nullptr),
m_pD3DRenderTargetView(nullptr),
m_pDepthStencilView(nullptr),
m_pDepthStencilBuffer(nullptr),
m_pDepthStencilState(nullptr),
m_pRasterizerState(nullptr),
m_pDXGISwapChain(nullptr),
m_hCursor(nullptr),
m_hIconLarge(nullptr),
m_hIconSmall(nullptr),
m_nVideoCardMem(0),
m_nRefreshRateDenominator(0),
m_nRefreshRateNumerator(0)
{
}

D3DApp::D3DApp(TCHAR* title, HINSTANCE hinst, UINT x, UINT y, UINT dx, UINT dy, bool fs) :
m_sClassName(title),
m_sVideoCardName(L"NotSet"),
m_sWindowTitle(title),
m_hWnd(nullptr),
m_hInstance(hinst),
m_nHeight(dy),
m_nWidth(dx),
m_vOutputSize((float)dx, (float)dy),
m_vBackgroundColor(0.0f, 0.0f, 0.450f, 1.0f),
m_bFullScreen(fs),
m_bAppRun(false),
m_pD3DDevice(nullptr),
m_pD3DDeviceContext(nullptr),
m_pD3DRenderTargetView(nullptr),
m_pDepthStencilView(nullptr),
m_pDepthStencilBuffer(nullptr),
m_pDepthStencilState(nullptr),
m_pRasterizerState(nullptr),
m_pDXGISwapChain(nullptr),
m_hIconLarge(nullptr),
m_hIconSmall(nullptr),
m_hCursor(nullptr),
m_nVideoCardMem(0),
m_nRefreshRateDenominator(0),
m_nRefreshRateNumerator(0)
{
	initApp(title, hinst, x, y, m_nWidth, m_nHeight, m_bFullScreen);
}

D3DApp::~D3DApp()
{
	destroyWindow();}


void Fugui::D3DApp::initApp(TCHAR* title, HINSTANCE hinst, UINT x, UINT y, UINT dx, UINT dy, bool fs)
{
	createWindow(x, y, dx, dy);
	initD3D((real)dx, (real)dy, fs, D3D11_CREATE_DEVICE_SINGLETHREADED);
	m_bAppRun = true;
}

void D3DApp::runApp()
{
	Timer::Get()->setTimer();
	real dt = (real)0.0;

	while (m_bAppRun) {

		if (PeekMessage(&m_msg, m_hWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);

			if (WM_QUIT == m_msg.message)
				m_bAppRun = false;
		}

		dt = (real)Timer::Get()->getDeltaTime();
		updateApp(dt);
		beginScene();
		renderApp();
		endScene();

		Timer::Get()->updateTime();
	}

	shutdownApp();
}

void D3DApp::shutdownApp()
{
	HR(m_pDXGISwapChain->SetFullscreenState(FALSE, NULL));

	SafeRelease(m_pRasterizerState);
	SafeRelease(m_pDepthStencilState);
	SafeRelease(m_pDepthStencilView);
	SafeRelease(m_pDepthStencilBuffer);
	SafeRelease(m_pD3DDeviceContext);
	SafeRelease(m_pDXGISwapChain);
	SafeRelease(m_pD3DRenderTargetView);
	SafeRelease(m_pD3DDevice);
}

void D3DApp::initD3D(const real width, const real height, const bool fullscreen, UINT createDeviceFlags)
{
#if defined(_DEBUG) || defined(DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif	//	_DEBUG | DEBUG

	createDeviceFlags |= D3D10_CREATE_DEVICE_BGRA_SUPPORT;

	m_bFullScreen = fullscreen;
	m_vOutputSize.x = width;
	m_vOutputSize.y = height;

	getDeviceInfo();
	createDeviceResources(createDeviceFlags);
}

void D3DApp::onResetDevice()
{

}

void D3DApp::onLostDevice()
{

}

void Fugui::D3DApp::createWindow(UINT x, UINT y, UINT dx, UINT dy)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbClsExtra			= NULL;
	wc.cbSize				= sizeof(WNDCLASSEX);
	wc.cbWndExtra		= NULL;
	wc.hbrBackground	= CreateSolidBrush(RGB(0, 0, 0));
	wc.hInstance			= m_hInstance;
	wc.lpszClassName	= m_sClassName.c_str();
	wc.lpszMenuName	= NULL;
	wc.hIcon				= m_hIconLarge;
	wc.hIconSm			= m_hIconSmall;
	wc.hCursor				= m_hCursor;
	wc.lpfnWndProc		= (WNDPROC)wndProc;
	wc.style					= CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	try {
		if (false == RegisterClassEx(&wc))
			throw L"Registering the class failed!";

		if (m_bFullScreen) {
			DEVMODE dm;
			ZeroMemory(&dm, sizeof(DEVMODE));
			dm.dmSize				= sizeof(DEVMODE);
			dm.dmPelsWidth		= (unsigned long)m_nWidth;
			dm.dmPelsHeight	= (unsigned long)m_nHeight;
			dm.dmBitsPerPel		= 32;
			dm.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&dm, CDS_FULLSCREEN);

			x = y = 0;
		}
	}
	catch (TCHAR* str) {
		THROW_MSG_BOX(str);
		return;
	} // end try

	try {
		m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
												m_sClassName.c_str(),
												m_sWindowTitle.c_str(),
												WS_POPUPWINDOW | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX,
												x, y,
												m_nWidth, m_nHeight,
												NULL, NULL,
												m_hInstance,
												NULL);
		if (NULL == m_hWnd)
			throw L"Creation of window failed!";

	}
	catch (TCHAR* str) {
		UnregisterClass(m_sClassName.c_str(), m_hInstance);
		THROW_MSG_BOX(str);
		return;
	} // end try

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	ShowCursor(FALSE);
}

void D3DApp::loadCursor(TCHAR* fileName)
{
	m_hCursor = LoadCursorFromFileW(fileName);
	if (m_hCursor) {
		m_hCursor = SetCursor(m_hCursor);
		ShowCursor(TRUE);
	}
}

void D3DApp::destroyWindow()
{
	if (m_hCursor)
		DestroyCursor(m_hCursor);
	DestroyWindow(m_hWnd);
	UnregisterClass(m_sClassName.c_str(), m_hInstance);
}

void D3DApp::getDeviceInfo()
{
	TDXGI_AdapterDesc ad;

	TDXGI_ModeDesc*   md = nullptr;
	IDXGI_Factory*		pDXGIFactory = nullptr;
	IDXGI_Adapter*		pDXGIAdapter = nullptr;
	IDXGI_Output*		pDXGIOutput = nullptr;

	UINT numModes = 0;

	ZeroMemory(&ad, sizeof(IDXGI_Adapter));

	HR(CreateDXGIFactory(__uuidof(IDXGI_Factory), (void**)&pDXGIFactory));
	HR(pDXGIFactory->EnumAdapters(0, &pDXGIAdapter));
	HR(pDXGIAdapter->EnumOutputs(0, &pDXGIOutput));
	HR(pDXGIOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
																DXGI_ENUM_MODES_INTERLACED,
																&numModes, NULL));
	md = new TDXGI_ModeDesc[numModes];
	if (nullptr == md) {
		HR(E_FAIL);
		return; 
	}

	HR(pDXGIOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
																DXGI_ENUM_MODES_INTERLACED,
																&numModes, md));
	for (UINT i = 0; i < numModes; ++i) {
		if (md[i].Width == (unsigned int)m_vOutputSize.x) {
			if (md[i].Height == (unsigned int)m_vOutputSize.y) {
				m_nRefreshRateNumerator = md[i].RefreshRate.Numerator;
				m_nRefreshRateDenominator = md[i].RefreshRate.Denominator;
			}
		}
	}

	HR(pDXGIAdapter->GetDesc(&ad));

	int frac = 1 / 1024;
	m_nVideoCardMem = ad.DedicatedVideoMemory * frac * frac;
	m_sVideoCardName = ad.Description;

	SafeDeleteArray(md);

	SafeRelease(pDXGIOutput);
	SafeRelease(pDXGIAdapter);
	SafeRelease(pDXGIFactory);
}

void D3DApp::createDeviceResources(UINT createDeviceFlags)
{
	TD3D_DriverType drvTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	TD3D_FeatureLevel featureLvls[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	TDXGI_SwapChainDesc		scDesc;
	TD3D_RasterizerDesc		rDesc;

	ID3D_Texture2d*		backBuffer = nullptr;

	UINT numDriverTypes = ARRAYSIZE(drvTypes);
	UINT numFeatureLvls = ARRAYSIZE(featureLvls);

	ZeroMemory(&scDesc, sizeof(TDXGI_SwapChainDesc));
	scDesc.BufferCount = 1;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.Height = (UINT)m_vOutputSize.y;
	scDesc.BufferDesc.Width = (UINT)m_vOutputSize.x;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.OutputWindow = m_hWnd;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.Windowed = !m_bFullScreen;
//  	scDesc.BufferDesc.RefreshRate.Numerator = m_nRefreshRateNumerator;
//  	scDesc.BufferDesc.RefreshRate.Denominator = m_nRefreshRateDenominator;
// 	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr = E_FAIL;
	for (UINT i = 0; hr != S_OK && i < numDriverTypes; i++) {
		hr = D3D_CreateDeviceAndSwapChain(NULL,
														drvTypes[i],
														NULL,
														createDeviceFlags,
														featureLvls,
														numFeatureLvls,
														D3D11_SDK_VERSION,
														&scDesc,
														&m_pDXGISwapChain,
														&m_pD3DDevice,
														&m_tFeatureLevel,
														&m_pD3DDeviceContext);
	}

	HR(m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D_Texture2d), (void**)&backBuffer));
	HR(m_pD3DDevice->CreateRenderTargetView(backBuffer, NULL, &m_pD3DRenderTargetView));

	SafeRelease(backBuffer);

	createDepthBuffer();

	m_pD3DDeviceContext->OMSetRenderTargets(1, &m_pD3DRenderTargetView, m_pDepthStencilView);

	ZeroMemory(&rDesc, sizeof(TD3D_RasterizerDesc));
	rDesc.AntialiasedLineEnable = true;
	rDesc.CullMode = D3D11_CULL_NONE;
	rDesc.DepthBias = 0;
	rDesc.DepthBiasClamp = 0.0f;
	rDesc.DepthClipEnable = true;
	rDesc.FillMode = D3D11_FILL_WIREFRAME;
	rDesc.FrontCounterClockwise = false;
	rDesc.MultisampleEnable = false;
	rDesc.ScissorEnable = false;
	rDesc.SlopeScaledDepthBias = 0.0f;

	HR(m_pD3DDevice->CreateRasterizerState(&rDesc, &m_pRasterizerState));
	m_pD3DDeviceContext->RSSetState(m_pRasterizerState);

	ZeroMemory(&m_tScreenViewport, sizeof(TD3D_Viewport));
	m_tScreenViewport.Height = (FLOAT)m_vOutputSize.y;
	m_tScreenViewport.Width = (FLOAT)m_vOutputSize.x;
	m_tScreenViewport.MinDepth = 0.0f;
	m_tScreenViewport.MaxDepth = 1.0f;
	m_tScreenViewport.TopLeftX = 0.0f;
	m_tScreenViewport.TopLeftY = 0.0f;

	m_pD3DDeviceContext->RSSetViewports(1, &m_tScreenViewport);
}

void D3DApp::createDepthBuffer()
{
	TD3D_Texture2dDesc				depthBufferDesc;
	TD3D_DepthStencilDesc			depthStencilDesc;
	TD3D_DepthStencilViewDesc		depthStencilViewDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = (UINT)m_vOutputSize.x;
	depthBufferDesc.Height = (UINT)m_vOutputSize.y;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	HR(m_pD3DDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer));

	ZeroMemory(&depthStencilDesc, sizeof(TD3D_DepthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HR(m_pD3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState));
	m_pD3DDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(TD3D_DepthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(m_pD3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView));
}



LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (msg)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	case WM_QUIT:
	{
		PostQuitMessage(0);
	}
	break;
	case WM_CLOSE:
	{
		PostQuitMessage(0);
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wparam)
		{
		case VK_ESCAPE:
		{
			PostQuitMessage(0);
		}
		break;
		default: break;
		}
	}
	break;

	default: return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return 0;
}
