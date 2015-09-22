#include "D3DApp.h"
#include "Camera.h"
#include "SimpleShader.h"
//#include "ModelPlanar.h"
#include "DXInput.h"
#include "D2DGrafix.h"
#include "D2DGeometry.h"
#include "D2DText.h"
#include "File3DExporter.h"
#include "CubeModel.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "../Debug/D3DApp.lib")
#pragma comment(lib, "../Debug/D2DGraphics.lib")
#pragma comment(lib, "../Debug/DXInput.lib")
#pragma comment(lib, "../Debug/My3DFileExporter.lib")
#else
#pragma comment(lib, "../Release/D3DApp.lib")
#pragma comment(lib, "../Release/D2DGraphics.lib")
#pragma comment(lib, "../Release/DXInput.lib")
#pragma comment(lib, "../Release/My3DFileExporter.lib")
#endif

using namespace DirectX;
using namespace Fugui;

//extern DXInput* g_input;

class TestApp : public D3DApp, public D2DGrafix
{
public:
	SimpleShader ss;
	D2DGeometry circle;
	D2DText		text;
	CubeModel  cube;
	//File3DExporter exporter;
//	ModelPlanar plane;
	Camera cam;
	int grassBmp;
	int psIdx;
	int vsIdx;

	TestApp(TCHAR* title, HINSTANCE hinst, int x, int y, int dx, int dy, bool fs) :
		D3DApp(title, hinst, x, y, dx, dy, fs)
	{
		g_input->init(m_hWnd, hinst, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		cam.pos() = XMFLOAT4(0.0f, 0.0f, -50.0f, 1.0f);
		cam.setLens(3.14f * 0.25, m_vOutputSize.x / m_vOutputSize.y, 0.00010f, 1000.0f);

		ss.initShader(m_pD3DDevice, L"VSTests.hlsl", L"PSTests.hlsl", "main", "main");
		ss.initInputLayout(m_pD3DDevice);
		ss.initSamplerState(m_pD3DDevice);
		ss.createBuffers(m_pD3DDevice);

	//	plane.loadFBXFrmt(m_pD3DDevice, m_pD3DDeviceContext, "TestPyramid.fbx", "Box");

		initD2D(m_pDXGISwapChain);
 		grassBmp = m_bmpCtrlr.loadBmpFromFile(m_pD2DRenderTarget, m_pWicImagingFactory, L"Assets\\grass.dds", 10.0f, 0.0f);
// 		circle.createSolidColorBrush(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f), m_pD2DRenderTarget, 1.0f);
		text.initText(m_pD2DRenderTarget, L"Verdana", 10.0f);
		circle.isSCBcreated();
		g_3dExporter->initExporter();
		g_3dExporter->load3dModel("Assets\\PlayerShip.obj", false, false, false);
		g_3dExporter->export3dFileData("Assets\\Batmobile.fug3d");

		cube.loadModelFile(m_pD3DDevice, m_pD3DDeviceContext, "Assets\\Batmobile.fug3d");
	}
	  
	virtual void onResetDevice()
	{
	}

	virtual void onLostDevice()
	{
	}
	float rads = 0.0f;
	virtual void updateApp(real dt)
	{
		g_input->pollBothTypes();
		cam.update(dt, 0.0f);
		rads += 0.5f * dt;
	}

	virtual void renderApp()
	{
		//ss.setShader(m_pD3DDeviceContext);

		XMFLOAT4X4 world;
		XMStoreFloat4x4(&world, XMMatrixIdentity());
		XMStoreFloat4x4(&world, XMMatrixRotationX(0.57f) * XMMatrixRotationY(rads) * XMMatrixTranslation(0.0f, 0.0f, 5.0f));
		

		ss.setBuffers(m_pD3DDeviceContext, cam.view(), world, cam.proj());
		ss.setShader(m_pD3DDeviceContext);

		cube.render(m_pD3DDeviceContext);

	//	m_pD2DRenderTarget->BeginDraw();
//  		for (int i = 0; i < 5; ++i)
//  			m_bmpCtrlr.drawBmp(m_pD2DRenderTarget, grassBmp, D2D1::RectF(10.0f*i, i*10.0f, 100,100));
// 		circle.drawEllipseSCB(m_pD2DRenderTarget, 200.0f, 500.0f, 150.0f, 250.0f);
	//	text.renderText(m_pD2DRenderTarget);
	//	HR(m_pD2DRenderTarget->EndDraw());
	}

};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	TestApp test(L"My Test App", hInstance, 200, 100, 1024, 768, false);
	if (test.isAppRunning())
		test.runApp();
}