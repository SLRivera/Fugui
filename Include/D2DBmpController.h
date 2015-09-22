#pragma once
#include <vector>
using std::vector;
#include "D3DUtils.h"

namespace Fugui
{
	class D2DBmpController
	{
	public:
		struct BmpInfo 
		{
			TD2D_RectF		clipRect;
			ID2D_Bitmap*		bmp;
			real		width,
						height;
		};

		inline vector<BmpInfo>&		getBmpList();

		int loadBmpFromFile(ID2D_RenderTarget* d2dRT, IWIC_ImagingFactory* wicFactory, wchar_t* bmpName, real width, real height, TD2D_RectF* clip = NULL);
		void drawBmp(ID2D_RenderTarget* d2dRT, int id, TD2D_RectF& pos, TD2D_RectF* clip = NULL, real a = 1.0f);
		void drawBackground(ID2D_RenderTarget* d2dRT, int id, real width, real height, real a = 1.0f);

		D2DBmpController();
		~D2DBmpController();

	private:
		vector<BmpInfo>		m_bmpList;
		static int					m_bmpId;
	};
}	// end namespace RivQui

vector<Fugui::D2DBmpController::BmpInfo>& Fugui::D2DBmpController::getBmpList()	{ return m_bmpList; }

