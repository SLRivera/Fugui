#pragma once

#include <d3d11.h>
#include <d2d1_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <dxgi.h>
#include <wincodec.h>
#include <dshow.h>
#include <vector>
#include "DxErr.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")


namespace Fugui
{
	// helpful defines
#define SafeRelease(p)	if(p) {p->Release(); p = nullptr;}
#define SafeDelete(p)		if(p) {delete p; p = nullptr;}
#define SafeDeleteArray(p)	if(p) {delete [] p; p = nullptr;}
#define D3D_CreateDevice							D3D11CreateDevice
#define D3D_CreateDeviceAndSwapChain		D3D11CreateDeviceAndSwapChain
#define THROW_MSG_BOX(str)		MessageBoxW( NULL, str, L"Excemption Raised!", MB_OK | MB_ICONERROR | MB_TOPMOST )
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                      \
	{                                                  \
		HRESULT hr = x;                                \
		if (FAILED(hr))                                 \
						{                                              \
			DXTraceW(__FILEW__, (DWORD)__LINE__, hr, NULL/*TEXT(x)*/, TRUE); \
						}                                              \
	}
#endif

#else
#ifndef HR
#define HR(x) x;
#endif
#endif 

#if 1
#define SINGLE_PRECISION
	typedef float real;
#define REAL_MAX	FLT_MAX;
#define REAL_SQRT		sqrtf
#define REAL_ABS		fabsf
#define REAL_SIN		sinf
#define REAL_COS		cosf
#define REAL_EXP		expf
#define REAL_POW		powf
#define REAL_FMOD	fmodf
#define REAL_EPSILON	FLT_EPSILON
#define REAL_PI			3.14159f
#else
#define DOUBLE_PRECISION
	typedef double real;
#define REAL_MAX	DBL_MAX;
#define REAL_SQRT		sqrt
#define REAL_ABS		fabs
#define REAL_SIN		sin
#define REAL_COS(val)		cos(val)
#define REAL_EXP		exp
#define REAL_POW		pow
#define REAL_FMOD	fmod
#define REAL_EPSILON	DBL_EPSILON
#define REAL_PI			3.14159265358979
#endif // 0

	// vertex shader layout descs
	struct VertexVSP {
		DirectX::XMFLOAT3		position;
	};

	struct VertexVSPT {
		DirectX::XMFLOAT3		position;
		DirectX::XMFLOAT2		uv;
	};

	struct VertexVSPN {
		DirectX::XMFLOAT3		position;
		DirectX::XMFLOAT3		normal;
	};

	struct VertexVSPNT {
		DirectX::XMFLOAT3		position;
		DirectX::XMFLOAT3		normal;
		DirectX::XMFLOAT2		uv;
	};

	struct VertexVSPNTBT {
		DirectX::XMFLOAT3		position;
		DirectX::XMFLOAT3		normal;
		DirectX::XMFLOAT3		tangent;
		DirectX::XMFLOAT3		binormal;
		DirectX::XMFLOAT2		uv;
	};

	void GenTriGrid(int numVertRows,
						int numVertCols,		
						float dx,
						float dz,
						const DirectX::XMFLOAT3& center,
						std::vector<DirectX::XMFLOAT3>& verts,
						std::vector<DWORD>& indices);

	//////////////////////////////////////////////////////////////////////////
	// randomness
	float getRandomFloat(const float min, const float max);
	void getRandomVec(DirectX::XMFLOAT3& vOut);

	// DirectX typedefs //////////////////////////////////////////////////////////////////////////
	typedef IDXGISwapChain		IDXGI_SwapChain;
	typedef IDXGIFactory			IDXGI_Factory;
	typedef IDXGIOutput			IDXGI_Output;
	typedef IDXGIAdapter			IDXGI_Adapter;
	typedef IDXGISurface			IDXGI_Surface;

	typedef DXGI_ADAPTER_DESC			TDXGI_AdapterDesc;
	typedef DXGI_MODE_DESC				TDXGI_ModeDesc;
	typedef DXGI_SWAP_CHAIN_DESC		TDXGI_SwapChainDesc;

	typedef ID3D11RenderTargetView	ID3D_RenderTargetView;
	typedef ID3D11DepthStencilView	ID3D_DepthStencilView;
	typedef ID3D11DepthStencilState	ID3D_DepthStencilState;
	typedef ID3D11RasterizerState		ID3D_RasterizerState;
	typedef ID3D11Device					ID3D_Device;
	typedef ID3D11DeviceContext		ID3D_DeviceContext;
	typedef ID3D11Buffer				ID3D_Buffer;
	typedef ID3D11Texture1D			ID3D_Texture1d;
	typedef ID3D11Texture2D			ID3D_Texture2d;
	typedef ID3D11Texture3D			ID3D_Texture3d;
	typedef ID3D11VertexShader		ID3D_VShader;
	typedef ID3D11PixelShader		ID3D_PShader;
	typedef ID3D11SamplerState	ID3D_SamplerState;
	typedef ID3D11InputLayout		ID3D_InputLayout;
	typedef ID3D11ShaderResourceView	ID3D_ShaderResourceView;

	typedef	ID3DBlob			ID3D_Blob;

	typedef D3D11_VIEWPORT				TD3D_Viewport;
	typedef D3D11_RASTERIZER_DESC		TD3D_RasterizerDesc;
	typedef D3D_DRIVER_TYPE				TD3D_DriverType;
	typedef D3D_FEATURE_LEVEL			TD3D_FeatureLevel;
	typedef D3D11_BUFFER_DESC			TD3D_BufferDesc;
	typedef D3D11_TEXTURE2D_DESC						TD3D_Texture2dDesc;
	typedef D3D11_DEPTH_STENCIL_DESC				TD3D_DepthStencilDesc;
	typedef D3D11_DEPTH_STENCIL_VIEW_DESC		TD3D_DepthStencilViewDesc;
	typedef D3D11_SUBRESOURCE_DATA					TD3D_SubResourceData;
	typedef D3D11_SAMPLER_DESC			TD3D_SamplerDesc;
	typedef D3D11_USAGE						ED3D_UsageFlags;
	typedef D3D11_INPUT_ELEMENT_DESC		TD3D_InputElemDesc;
	typedef D3D11_MAPPED_SUBRESOURCE		TD3D_MappedSubResource;

	typedef ID2D1Factory				ID2D_Factory;
	typedef ID2D1Device				ID2D_Device;
	typedef ID2D1DeviceContext		ID2D_DeviceContext;
	typedef ID2D1HwndRenderTarget	ID2D_HwndRenderTarget;
	typedef ID2D1RenderTarget		ID2D_RenderTarget;
	typedef ID2D1Bitmap				ID2D_Bitmap;
	typedef ID2D1SolidColorBrush	ID2D_SolidColorBrush;
	typedef ID2D1RadialGradientBrush		ID2D_RadialGradientBrush;
	typedef ID2D1LinearGradientBrush		ID2D_LinearGradientBrush;
	typedef ID2D1Brush					ID2D_Brush;
	typedef ID2D1Geometry			ID2D_Geometry;
	typedef ID2D1RectangleGeometry	ID2D_RectangleGeo;
	typedef ID2D1RoundedRectangleGeometry ID2D_RoundedRectangleGeo;
	typedef ID2D1EllipseGeometry	ID2D_EllipseGeo;
	typedef ID2D1StrokeStyle			ID2D_StrokeStyle;

	typedef IDWriteFactory				ID_WriteFactory;
	typedef IDWriteTextFormat		ID_WriteTxtFrmt;

	typedef D2D1_RENDER_TARGET_PROPERTIES	TD2D_RenderTargetProperties;
	typedef D2D1_ROUNDED_RECT						TD2D_RoundedRect;
	typedef D2D1_RECT_F									TD2D_RectF;
	typedef D2D1_ELLIPSE									TD2D_Ellipse;
	typedef D2D1_POINT_2F								TD2D_PointF;
	typedef D2D1_VECTOR_2F							TD2D_Vec2;
	typedef D2D1_COLOR_F								TD2D_ColorF;
	typedef D2D1_HWND_RENDER_TARGET_PROPERTIES	TD2D_HwndRenderTargetProperties;

	typedef IWICImagingFactory			IWIC_ImagingFactory;
	typedef IWICBitmapDecoder			IWIC_BmpDecoder;
	typedef IWICBitmapFrameDecode	IWIC_BmpFrameDecode;
	typedef IWICStream					IWIC_Stream;
	typedef IWICFormatConverter		IWIC_FormatConverter;
	typedef IWICBitmapScaler			IWIC_BmpScaler;

	typedef IGraphBuilder			IDS_GraphBuilder;
	typedef IMediaControl			IDS_MediaControl;
	typedef IMediaEvent			IDS_MediaEvent;
	typedef IVideoWindow			IDS_VideoWindow;

}	// end namespace Fugui