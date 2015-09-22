#include <fstream>
#include "CubeModel.h"

using std::ifstream;
using namespace Fugui;
using namespace DirectX;

CubeModel::CubeModel()
{
}


CubeModel::~CubeModel()
{
}

int CubeModel::loadModelFile(ID3D_Device* device, ID3D_DeviceContext* context, const char* fileName)
{
	ifstream stream(fileName, std::ios::in);
	if (stream.fail())
		return -1;

	char token[64];
	stream.getline(token, 64, ':');
	stream.getline(token, 64);

	m_sName = token;

	stream.getline(token, 64, ':');
	stream.getline(token, 64);
	int numMeshes = atoi(token);

	stream.getline(token, 64, ':');
	stream.getline(token, 64);
	int numMaterials = atoi(token);

	stream.getline(token, 64, ':');
	stream.getline(token, 64);
	int numAnimations = atoi(token);

	stream.getline(token, 64, ':');
	stream.getline(token, 64);
	int numTexturese = atoi(token);

	stream.getline(token, 64, ':');
	stream.getline(token, 64);
	stream.getline(token, 64, ':');
	stream.getline(token, 64);
	int hasNormals = atoi(token);

	stream.getline(token, 64, ':');
	stream.getline(token, 64);
	int hasUVs = atoi(token);

	stream.getline(token, 64, ':');
	stream.getline(token, 64);
	int hasTangents = atoi(token);

	stream.getline(token, 64, ':');
	stream.getline(token, 64);
	m_nNumVerts = atoi(token);

	stream.getline(token, 64, ':');
	stream.getline(token, 64);
	m_nNumPolys = atoi(token);

	m_nNumIndices = m_nNumPolys * 3;

	stream.getline(token, 64);
	for (int i = 0; i < m_nNumVerts; ++i) {
		VertexVSPNT vert;
		stream.getline(token, 64, ':');
		stream.getline(token, 64, ',');

		vert.position.x = (float)atof(token);

		stream.getline(token, 64, ',');
		vert.position.y = (float)atof(token);

		stream.getline(token, 64);		
		vert.position.z = (float)atof(token);

		m_vertexList.push_back(vert);
	}

	stream.getline(token, 64);

	for (int i = 0; i < m_nNumPolys; ++i) {
		stream.getline(token, 64);

		stream.getline(token, 64, ':');
		stream.getline(token, 64);

		int tempIdx = atoi(token);
		m_indexList.push_back(tempIdx);

		stream.getline(token, 64, ':');
		stream.getline(token, 64);
		tempIdx = atoi(token);

		m_indexList.push_back(tempIdx);

		stream.getline(token, 64, ':');
		stream.getline(token, 64);
		tempIdx = atoi(token);

		m_indexList.push_back(tempIdx);
	}

	stream.getline(token, 64);

	for (int i = 0; i < m_nNumVerts; ++i) {
		stream.getline(token, 64, ':');
		stream.getline(token, 64, ',');
		m_vertexList[i].normal.x = (float)atof(token);

		stream.getline(token, 64, ',');
		m_vertexList[i].normal.y = (float)atof(token);

		stream.getline(token, 64);
		m_vertexList[i].normal.z = (float)atof(token);
	}

	stream.getline(token, 64);

	for (int i = 0; i < m_nNumVerts; ++i) {
		stream.getline(token, 64, ':');
		stream.getline(token, 64);

		m_vertexList[i].uv.x = (float)atof(token);

		stream.getline(token, 64, ',');
		stream.getline(token, 64);
		m_vertexList[i].uv.y = (float)atof(token);
	}

	stream.close();

	createVB(device, context);
	createIB(device, context);

	return ++m_nID;
}

void CubeModel::update(float dt)
{
}

void CubeModel::render(ID3D_DeviceContext* context)
{
	UINT stride = sizeof(VertexVSPNT);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	context->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->DrawIndexed(m_nNumIndices, 0, 0);
}

void CubeModel::shutdown()
{
}

void Fugui::CubeModel::createVB(ID3D_Device* device, ID3D_DeviceContext* context)
{
	TD3D_BufferDesc desc;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.ByteWidth = sizeof(VertexVSPNT) * m_nNumVerts;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	HR(device->CreateBuffer(&desc, NULL, &m_pVB));

	D3D11_MAPPED_SUBRESOURCE mres;

	HR(context->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mres));
	VertexVSPNT* verts = (VertexVSPNT*)mres.pData;

	for (int i = 0; i < m_nNumVerts; ++i) {
		verts[i].position = m_vertexList[i].position;
		verts[i].normal = m_vertexList[i].normal;
		verts[i].uv = m_vertexList[i].uv;
	}
	context->Unmap(m_pVB, 0);	
}

void Fugui::CubeModel::createIB(ID3D_Device* device, ID3D_DeviceContext* context)
{
	TD3D_BufferDesc desc;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(DWORD) * m_nNumIndices;

	HR(device->CreateBuffer(&desc, NULL, &m_pIB));

	D3D11_MAPPED_SUBRESOURCE mres;
	HR(context->Map(m_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mres));

	DWORD* idxs = (DWORD*)mres.pData;
	for (int i = 0; i < m_nNumIndices; ++i) 
		idxs[i] = m_indexList[i];
	
	context->Unmap(m_pIB, 0);	
}
