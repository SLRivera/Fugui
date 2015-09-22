#include "SimpleShader.h"

using namespace Fugui;
using namespace DirectX;

SimpleShader::SimpleShader() : ShaderBaseInfo()
{
	m_cbMatrices = nullptr;
}


SimpleShader::~SimpleShader()
{
	SafeRelease(m_cbMatrices);
}

void SimpleShader::initInputLayout(ID3D_Device* device)
{
	TD3D_InputElemDesc desc[3];
	desc[0].SemanticName = "POSITION";
	desc[0].AlignedByteOffset = 0;
	desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc[0].InputSlot = 0;
	desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc[0].InstanceDataStepRate = 0;
	desc[0].SemanticIndex = 0;

	desc[1].SemanticName = "NORMAL";
	desc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	desc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc[1].InputSlot = 0;
	desc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc[1].InstanceDataStepRate = 0;
	desc[1].SemanticIndex = 0;

	desc[2].SemanticName = "TEXCOORD";
	desc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	desc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	desc[2].InputSlot = 0;
	desc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc[2].InstanceDataStepRate = 0;
	desc[2].SemanticIndex = 0;

	HR(device->CreateInputLayout(desc, 3, m_pVSBuffer->GetBufferPointer(), m_pVSBuffer->GetBufferSize(), &m_pInputLayout));
	SafeRelease(m_pVSBuffer);
}

void Fugui::SimpleShader::initSamplerState(ID3D_Device* device)
{
	TD3D_SamplerDesc desc;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.BorderColor[0] = 0.0f;
	desc.BorderColor[1] = 0.0f;
	desc.BorderColor[2] = 0.0f;
	desc.BorderColor[3] = 0.0f;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 0;
	desc.MinLOD = 0.0f;
	desc.MipLODBias = 0.0f;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&desc, &m_pSamplerState));
}

void SimpleShader::setBuffers(ID3D_DeviceContext* context, DirectX::XMFLOAT4X4& v, DirectX::XMFLOAT4X4& w, DirectX::XMFLOAT4X4& p)
{
	TD3D_MappedSubResource mr;
	HR(context->Map(m_cbMatrices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr));

	cbMatrices* m = (cbMatrices*)mr.pData;
	//	XMMATRIX temp = XMMatrixTranspose(XMLoadFloat4x4(&p));
	XMStoreFloat4x4(&m->projection, XMMatrixTranspose(XMLoadFloat4x4(&p)));
	//	temp = XMMatrixTranspose(XMLoadFloat4x4(&w));
	XMStoreFloat4x4(&m->world, XMMatrixTranspose(XMLoadFloat4x4(&w)));
	//	temp = XMMatrixTranspose(XMLoadFloat4x4(&v));
	XMStoreFloat4x4(&m->view, XMMatrixTranspose(XMLoadFloat4x4(&v)));

// 	m->projection = p;
// 	m->view = v;
// 	m->world = w;
	context->Unmap(m_cbMatrices, 0);
	context->VSSetConstantBuffers(0, 1, &m_cbMatrices);
}

void SimpleShader::createBuffers(ID3D_Device* device)
{
	TD3D_BufferDesc desc;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(cbMatrices);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	HR(device->CreateBuffer(&desc, NULL, &m_cbMatrices));
}

