#pragma once

#include <string>
#include "D3DUtils.h"

namespace Fugui
{
	class D3DApp
	{
	public:
		virtual void onResetDevice();
		virtual void onLostDevice();
		virtual void initD3D(const real width, const real height, const bool fullscreen, UINT createDeviceFlags);
	
		virtual void initApp(TCHAR* title, HINSTANCE hinst, UINT x, UINT y, UINT dx, UINT dy, bool fs);
		virtual void updateApp(real dt) = 0;
		virtual void renderApp() = 0;

		inline void beginScene();
		inline void endScene();

		void runApp();
		void shutdownApp();
		void loadCursor(TCHAR* fileName);

		inline bool isAppRunning();
		inline const IDXGI_SwapChain* getSwapChain();

		D3DApp();
		D3DApp(TCHAR* title, HINSTANCE hinst, UINT x, UINT y, UINT dx, UINT dy, bool fs);
		~D3DApp();

	protected:
		// windows variables and internal functionality 	//////////////////////////////////////////////////////////////////////////
		std::wstring	m_sClassName;
		std::wstring	m_sWindowTitle;

		HWND			m_hWnd;
		HINSTANCE	m_hInstance;

		MSG				m_msg;

		HICON		m_hIconSmall;
		HICON		m_hIconLarge;

		HCURSOR	m_hCursor;

		UINT			m_nWidth;
		UINT			m_nHeight;

		bool			m_bFullScreen;
		bool			m_bAppRun;

		void createWindow(UINT x, UINT y, UINT dx, UINT dy);
		void destroyWindow();

		// directx variables and internal functionality	//////////////////////////////////////////////////////////////////////////
		std::wstring	m_sVideoCardName;

		TD3D_Viewport				m_tScreenViewport;
		D3D_FEATURE_LEVEL		m_tFeatureLevel;

		DirectX::XMFLOAT4A	m_vBackgroundColor;

		DirectX::XMFLOAT2A	m_vOutputSize;

		ID3D_Device*					m_pD3DDevice;
		ID3D_DeviceContext*		m_pD3DDeviceContext;
		ID3D_RenderTargetView*	m_pD3DRenderTargetView;
		ID3D_DepthStencilView*	m_pDepthStencilView;
		ID3D_Texture2d*				m_pDepthStencilBuffer;
		ID3D_DepthStencilState*	m_pDepthStencilState;
		ID3D_RasterizerState*		m_pRasterizerState;		

		IDXGI_SwapChain*		m_pDXGISwapChain;

		UINT	m_nVideoCardMem;
		UINT m_nRefreshRateNumerator;
		UINT m_nRefreshRateDenominator;

		void getDeviceInfo();
		void createDeviceResources(UINT createDeviceFlags);
		void createDepthBuffer();
	};

	// inline function definitions	//////////////////////////////////////////////////////////////////////////
	bool D3DApp::isAppRunning() { return m_bAppRun; }

	void D3DApp::beginScene() 
	{
		m_pD3DDeviceContext->ClearRenderTargetView(m_pD3DRenderTargetView, (FLOAT*)&m_vBackgroundColor);
		m_pD3DDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);
	}

	void D3DApp::endScene()
	{
		HR(m_pDXGISwapChain->Present(0, 0));
	}

	const IDXGI_SwapChain* D3DApp::getSwapChain() { return m_pDXGISwapChain; }

}	// end namespace Fugui