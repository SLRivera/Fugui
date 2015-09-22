#include "ShaderBaseInfo.h"

using namespace Fugui;

int ShaderBaseInfo::m_nID = -1;

ShaderBaseInfo::ShaderBaseInfo() :
m_pVSBuffer(nullptr),
m_pInputLayout(nullptr),
m_pPS(nullptr),
m_pVS(nullptr),
m_pSamplerState(nullptr)
{
}


ShaderBaseInfo::~ShaderBaseInfo()
{
	SafeRelease(m_pSamplerState);
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pPS);
	SafeRelease(m_pVS);
	SafeRelease(m_pVSBuffer);
}

int ShaderBaseInfo::initShader(ID3D_Device* device, TCHAR* vsFileName, TCHAR* psFileName, char* vsEntry, char* psEntry)
{
	ID3D_Blob*	err = nullptr;
	ID3D_Blob*	psBuff = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	// create the vertex shader
	if (FAILED(D3DCompileFromFile(vsFileName, NULL, NULL, vsEntry, VERTEX_SHADER_MODEL, flags, 0, &m_pVSBuffer, &err))) {
		if (m_pVSBuffer)
			SafeRelease(m_pVSBuffer);
		if (err) {
			OutputDebugStringA((char*)err->GetBufferPointer());
			SafeRelease(err);
			HR(E_FAIL);
		}
		return -1;
	}

	SafeRelease(err);

	HR(device->CreateVertexShader(m_pVSBuffer->GetBufferPointer(), m_pVSBuffer->GetBufferSize(), NULL, &m_pVS));

	// create the pixel shader
	if (FAILED(D3DCompileFromFile(psFileName, NULL, NULL, psEntry, PIXEL_SHADER_MODEL, flags, 0, &psBuff, &err))) {
		if (psBuff)
			SafeRelease(psBuff);
		if (err) {
			OutputDebugStringA((char*)err->GetBufferPointer());
			SafeRelease(err);
			HR(E_FAIL);
		}
		return -1;
	}

	SafeRelease(err);

	HR(device->CreatePixelShader(psBuff->GetBufferPointer(), psBuff->GetBufferSize(), NULL, &m_pPS));

	return ++m_nID;
}

void ShaderBaseInfo::setShader(ID3D_DeviceContext* context)
{
	context->IASetInputLayout(m_pInputLayout);
	context->VSSetShader(m_pVS, NULL, NULL);
	context->PSSetShader(m_pPS, NULL, NULL);
	context->PSSetSamplers(0, 1, &m_pSamplerState);
}
