#include "FxMngr.h"

using namespace Fugui;
using namespace DirectX;

ID3D11InputLayout*	FxMngr::m_pVP = nullptr;
ID3D11InputLayout*	FxMngr::m_pVPT = nullptr;
ID3D11InputLayout*	FxMngr::m_pVPN = nullptr;
ID3D11InputLayout*	FxMngr::m_pVPNT = nullptr;
ID3D11InputLayout*	FxMngr::m_pVPNTBT = nullptr;

UINT	FxMngr::m_nPSCounter = 0;
UINT	FxMngr::m_nVSCounter = 0;

FxMngr::FxMngr()
{
	m_PSList.resize(0);
	m_VSList.resize(0);
}
 

FxMngr::~FxMngr()
{
	// TODO: perform cleanup
}

UINT Fugui::FxMngr::createPSFx(ID3D_Device* device, TCHAR* fxFilename, char* entryName, char* shaderModel)
{
	ID3D_Blob* errBlob = nullptr;
	ID3D_Blob* shaderBuff = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS/* | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY*/;

#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(fxFilename, NULL, NULL, entryName, shaderModel, flags, 0, &shaderBuff, &errBlob))) {
		if (shaderBuff)
			SafeRelease(shaderBuff);
		if (errBlob) {
			SafeRelease(errBlob);
			HR(E_FAIL);
		}
		return (UINT)INTMAX_MAX;
	}

	SafeRelease(errBlob);

	PSData newPS;
	HR(device->CreatePixelShader(shaderBuff->GetBufferPointer(), shaderBuff->GetBufferSize(), NULL, &newPS.pPS));

	SafeRelease(shaderBuff);

	m_PSList.push_back(newPS);

	return m_nPSCounter++;
}

UINT Fugui::FxMngr::createVSFx(ID3D_Device* device, TCHAR* fxFilename, char* entryName, char* shaderModel, VertexTypes type)
{
	ID3D_Blob* errBlob = nullptr;
	ID3D_Blob* shaderBuff = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS/* | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY*/;

#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(fxFilename, NULL, NULL, entryName, shaderModel, flags, 0, &shaderBuff, &errBlob))) {
		if (shaderBuff)
			SafeRelease(shaderBuff);
		if (errBlob) {			
			OutputDebugStringA((char*)errBlob->GetBufferPointer());
			SafeRelease(errBlob);
			HR(E_FAIL);
		}
		return (UINT)INTMAX_MAX;
	}

	SafeRelease(errBlob);

	VSData newVS;
	HR(device->CreateVertexShader(shaderBuff->GetBufferPointer(), shaderBuff->GetBufferSize(), NULL, &newVS.pVS));

	initVertexDesc(device, shaderBuff, type);

	SafeRelease(shaderBuff);

	m_VSList.push_back(newVS);

	return m_nVSCounter++;
}

void Fugui::FxMngr::createSamplerState(UINT idx, ID3D_Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode, UINT anisotropy, D3D11_COMPARISON_FUNC compFunc)
{
	TD3D_SamplerDesc	samplerDesc;
	samplerDesc.AddressU = mode;
	samplerDesc.AddressV = mode;
	samplerDesc.AddressW = mode;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = compFunc;
	samplerDesc.Filter = filter;
	samplerDesc.MaxAnisotropy = anisotropy;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MipLODBias = 0.0f;

	HR(device->CreateSamplerState(&samplerDesc, &m_PSList[idx].pSamplerState));
}

void Fugui::FxMngr::setCbVSChangesOnResize(ID3D_DeviceContext* context, DirectX::XMFLOAT4X4& projection)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	HR(context->Map(m_pCbVSChangesOnResize, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr));
	CbVSChangesOnResize* nc = (CbVSChangesOnResize*)mr.pData;
	nc->mProj = projection;
	context->Unmap(m_pCbVSChangesOnResize, 0);
	context->VSSetConstantBuffers(g_nCbVSChangesOnResizeIdx, 1, &m_pCbVSChangesOnResize);
}

void Fugui::FxMngr::setCbVSChangesPerFrame(ID3D_DeviceContext* context, DirectX::XMFLOAT4X4& world)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	HR(context->Map(m_pCbVSChangesPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr));
	CbVSChangesPerFrame* nc = (CbVSChangesPerFrame*)mr.pData;
	nc->mWorld = world;
	context->Unmap(m_pCbVSChangesPerFrame, 0);
	context->VSSetConstantBuffers(g_nCbVSChangesPerFrameIdx, 1, &m_pCbVSChangesPerFrame);
}

void FxMngr::setCbPSPerPrimitive(ID3D_DeviceContext* context, DirectX::XMFLOAT4A& diffuse, DirectX::XMFLOAT4A& ambient, DirectX::XMFLOAT4A& specular, float specExponent)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	HR(context->Map(m_pCbPSPerPrimitive, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr));
	CbPSPerPrimitive* nc = (CbPSPerPrimitive*)mr.pData;
	nc->fSpecularExponent = specExponent;
	nc->mtrlAmbientColor = ambient;
	nc->mtrlDiffuseColor = diffuse;
	nc->mtrlSpecularColor = specular;
	context->Unmap(m_pCbPSPerPrimitive, 0);
	context->PSSetConstantBuffers(g_nCbPSPerPrimitiveIdx, 1, &m_pCbPSPerPrimitive);
}

void Fugui::FxMngr::setCbPSChangesPerFrame(ID3D_DeviceContext* context, CbPSChangesPerFrame& info)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	HR(context->Map(m_pCbPSChangesPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr));
	CbPSChangesPerFrame* nc = (CbPSChangesPerFrame*)mr.pData;
	*nc = info;
	context->Unmap(m_pCbPSChangesPerFrame, 0);
	context->PSSetConstantBuffers(g_nCbPSChangesPerFrameIdx, 1, &m_pCbPSChangesPerFrame);
}

void Fugui::FxMngr::setCbVSNeverChanges(ID3D_DeviceContext* context, DirectX::XMFLOAT4X4& view)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	HR(context->Map(m_pCbVSNeverChanges, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr));
	CbVSNeverChanges* nc = (CbVSNeverChanges*)mr.pData;
	nc->mView = view;
	context->Unmap(m_pCbVSNeverChanges, 0);
	context->VSSetConstantBuffers(g_nCbVSNeverChangesIdx, 1, &m_pCbVSNeverChanges);
}

void FxMngr::createCBs(ID3D_Device* device)
{
	TD3D_BufferDesc		cb;

	cb.ByteWidth = sizeof(CbVSNeverChanges);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	//cb.StructureByteStride = 0;

	HR(device->CreateBuffer(&cb, NULL, &m_pCbVSNeverChanges));

	cb.ByteWidth = sizeof(CbVSChangesOnResize);
	HR(device->CreateBuffer(&cb, NULL, &m_pCbVSChangesOnResize));

	cb.ByteWidth = sizeof(CbVSChangesPerFrame);
	HR(device->CreateBuffer(&cb, NULL, &m_pCbVSChangesPerFrame));

	cb.ByteWidth = sizeof(CbPSPerPrimitive);
	HR(device->CreateBuffer(&cb, NULL, &m_pCbPSPerPrimitive));

	cb.ByteWidth = sizeof(CbPSChangesPerFrame);
	HR(device->CreateBuffer(&cb, NULL, &m_pCbPSChangesPerFrame));
}

void FxMngr::initVertexDesc(ID3D_Device* device, ID3D_Blob* shader, VertexTypes type)
{
	if (nullptr == device || nullptr == shader)
		return;

	switch (type)
	{
	case VertexP:
	{
		if (nullptr == m_pVP) {
			const D3D11_INPUT_ELEMENT_DESC layout[] =
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };

			device->CreateInputLayout(layout, ARRAYSIZE(layout), shader->GetBufferPointer(),
				shader->GetBufferSize(), &m_pVP);
		}
	} // end VertexP
	break;
	case VertexPT:
	{
		if (nullptr == m_pVPT) {
			const D3D11_INPUT_ELEMENT_DESC layout[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			device->CreateInputLayout(layout, ARRAYSIZE(layout), shader->GetBufferPointer(),
				shader->GetBufferSize(), &m_pVPT);
		}
	} // end case VertexPT
	break;
	case VertexPN:
	{
		if (nullptr == m_pVPN) {
			const D3D11_INPUT_ELEMENT_DESC layout[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			device->CreateInputLayout(layout, ARRAYSIZE(layout), shader->GetBufferPointer(),
				shader->GetBufferSize(), &m_pVPN);
		}
	} // end case VertexPN
	break;
	case VertexPNT:
	{
		if (nullptr == m_pVPNT) {
			const D3D11_INPUT_ELEMENT_DESC layout[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			device->CreateInputLayout(layout, ARRAYSIZE(layout), shader->GetBufferPointer(),
				shader->GetBufferSize(), &m_pVPNT);
		}
	} // end case VertexPNT
	break;
	case VertexPNTBT:
	{
		if (nullptr == m_pVPNTBT) {
			const D3D11_INPUT_ELEMENT_DESC layout[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			device->CreateInputLayout(layout, ARRAYSIZE(layout), shader->GetBufferPointer(),
													shader->GetBufferSize(), &m_pVPNTBT);
		}
	} // end case VertexPNTBT
	break;
	} // end switch(type)
}

ID3D11InputLayout* FxMngr::getVertexDesc(VertexTypes type)
{
	switch (type)
	{
	case VertexP:
	{
		if (m_pVP)
			return m_pVP;
		else
			return nullptr;
	} // end case VertexP
	case VertexPT:
	{
		if (m_pVPT)
			return m_pVPT;
		else
			return nullptr;
	} // end case VertexPT
	case VertexPN:
	{
		if (m_pVPN)
			return m_pVPN;
		else
			return nullptr;
	} // end case VertexPN
	case VertexPNT:
	{
		if (m_pVPNT)
			return m_pVPNT;
		else
			return nullptr;
	} // end case VertexPNT
	case VertexPNTBT:
	{
		if (m_pVPNTBT)
			return m_pVPNTBT;
		else
			return nullptr;
	} // end case VertexPNTBT
	default: return nullptr;
	} // end switch(type)
}

