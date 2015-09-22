#include "D2DBmpController.h"

int Fugui::D2DBmpController::m_bmpId = 0;

Fugui::D2DBmpController::D2DBmpController()
{
	m_bmpList.clear();
}


Fugui::D2DBmpController::~D2DBmpController()
{
	for (unsigned int i = 0; i < m_bmpList.size(); ++i)
		SafeRelease(m_bmpList[i].bmp);
}

int Fugui::D2DBmpController::loadBmpFromFile(ID2D_RenderTarget* d2dRT, IWIC_ImagingFactory* wicFactory, wchar_t* bmpName, real width, real height, TD2D_RectF* clip /*= NULL*/)
{
	IWIC_BmpDecoder*			pDecoder = nullptr;
	IWIC_BmpFrameDecode*	pSource = nullptr;
	IWIC_FormatConverter*	pConverter = nullptr;
	BmpInfo							bmpInfo;

	ZeroMemory(&bmpInfo, sizeof(BmpInfo));

	try{
		if (FAILED(wicFactory->CreateDecoderFromFilename(
						bmpName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder)))
			throw L"D2D BMP Decoder creation failed.\nFile Not found!";

		if (FAILED(pDecoder->GetFrame(0, &pSource)))
			throw L"D2D BMP Decoder get frame failed.";

		if (FAILED(wicFactory->CreateFormatConverter(&pConverter)))
			throw L"D2D BMP converter creation failed.";

		if (FAILED(pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
						NULL, 0.0f, WICBitmapPaletteTypeMedianCut)))
			throw L"D2D BMP Converter Initialization failed.";

		if (FAILED(d2dRT->CreateBitmapFromWicBitmap(pConverter, NULL, &bmpInfo.bmp)))
			throw L"Creation of BMp from WIC_BMP failed.";

		bmpInfo.height	= height;
		bmpInfo.width		= width;
		if (clip)
			memcpy(&bmpInfo.clipRect, clip, sizeof(TD2D_RectF));
		m_bmpList.push_back(bmpInfo);
	}
	catch (LPCWSTR str) {
		THROW_MSG_BOX(str);
		return -1;
	}

	SafeRelease(pConverter);
	SafeRelease(pSource);
	SafeRelease(pDecoder);

	return m_bmpId++;
}

void Fugui::D2DBmpController::drawBmp(ID2D_RenderTarget* d2dRT, int id, TD2D_RectF& pos, TD2D_RectF* clip, real a /*= 1.0f*/)
{
	d2dRT->DrawBitmap(m_bmpList[id].bmp, pos, (FLOAT)a, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, clip);

	return;
}

void Fugui::D2DBmpController::drawBackground(ID2D_RenderTarget* d2dRT, int id, real width, real height, real a /*= 1.0f*/)
{
	d2dRT->SetTransform(D2D1::Matrix3x2F::Translation(0.0f, 0.0f));
	d2dRT->DrawBitmap(m_bmpList[id].bmp, D2D1::RectF(0.0f, 0.0f, width, height), (FLOAT)a, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	return;
}

