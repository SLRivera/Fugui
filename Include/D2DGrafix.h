#pragma once
#include "D3DUtils.h"
#include "D2DBmpController.h"

namespace Fugui
{
	class D2DGrafix
	{
	public:
		void initD2D(IDXGI_SwapChain* swapChain);
		void onResetDevice();
		void onLostDevice();

		inline D2DBmpController& getBmpCtrlr();

		D2DGrafix();
		~D2DGrafix();

	protected:
		D2DBmpController		m_bmpCtrlr;

		TD2D_RenderTargetProperties	m_tRenderTargetProps;

	//	ID2D_Device*					m_pD2DDevice;
		ID2D_Factory*					m_pD2DFactory;
		//ID2D_DeviceContext*		m_pD2DDeviceContext;
		ID2D_RenderTarget*		m_pD2DRenderTarget;
	//	ID2D_Bitmap*					m_pTargetBitmap;

		//ID_WriteFactory*				m_pWriteFactory;
		IWIC_ImagingFactory*		m_pWicImagingFactory;
	};

	D2DBmpController& D2DGrafix::getBmpCtrlr() { return m_bmpCtrlr; }

} // end namespace Fugui