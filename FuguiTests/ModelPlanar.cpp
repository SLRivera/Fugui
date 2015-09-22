//#include "../FBXReader/FbxFileReader.h"
#include "ModelPlanar.h"

using namespace DirectX;
using namespace Fugui;

ModelPlanar::ModelPlanar()
{
}


ModelPlanar::~ModelPlanar()
{
}

int ModelPlanar::loadFBXFrmt(ID3D_Device* device, ID3D_DeviceContext* context, const char* fileName, const char* modelName)
{
	// get the model info
// 	FbxFileReader::initReader();
// 	g_fbx->loadFbxFile(fileName, modelName);
// 	g_fbx->getFbxFileData();
// 
// 	m_nNumVerts = g_fbx->getMeshes()[0].nVertexCnt;
// 	m_nNumIndices = g_fbx->getMeshes()[0].nVertIdxCnt;
// 	// TODO: this may increase load times slightly, consider moving all the code into this one function
// 	createVB(device);
// 	createIB(device);
// 
// 	FbxFileReader::shutdownReader();
	return ++m_nID;
}

void ModelPlanar::update(float dt)
{
}

void ModelPlanar::render(ID3D_DeviceContext* context)
{
	UINT stride = sizeof(SimpleVert);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
 	context->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
 	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 	context->DrawIndexed(3/*m_nNumIndices*/, 0, 0);
	//context->Draw(4, 0);
}

void ModelPlanar::shutdown()
{
}

void Fugui::ModelPlanar::createVB(ID3D_Device* device)
{
	// create the vertex buffer
	TD3D_BufferDesc desc;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//desc.ByteWidth = sizeof(SimpleVert) * m_nNumVerts;
	desc.ByteWidth = sizeof(SimpleVert) * 3;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	SimpleVert vData[3];
	vData[0].position = { 0.0f, 1.0f, 0.0f };
	vData[1].position = { 1.0f, 0.0f, 0.0f };
	vData[2].position = { -1.0f, 0.0f, 0.0f };
	/*vData[3].position = { -1.0f, -1.0f, 0.0f };*/
	// 	SimpleVert* vData = new SimpleVert[m_nNumVerts];
// 	if (nullptr == vData)
// 		return;
// 
// 	for (int i = 0; i < m_nNumVerts; ++i) {
// 		vData[i].position.x = g_fbx->getMeshes()[0].vertexList[i].x;
// 		vData[i].position.y = g_fbx->getMeshes()[0].vertexList[i].y;
// 		vData[i].position.z = g_fbx->getMeshes()[0].vertexList[i].z;
// 	}
// 
	TD3D_SubResourceData srd;
	srd.SysMemPitch = 0;
	srd.SysMemSlicePitch = 0;
	srd.pSysMem = vData;

	HR(device->CreateBuffer(&desc, &srd, &m_pVB));
	//SafeDeleteArray(vData);
}

void Fugui::ModelPlanar::createIB(ID3D_Device* device)
{
	// create the index buffer
	TD3D_BufferDesc desc;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = sizeof(UINT) * 3;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	UINT idxData[3] = { 1,2,0};
// 	int* idxData = new int[m_nNumIndices];
// 	if (nullptr == idxData)
// 		return;
// 
// 	for (int i = 0; i < m_nNumIndices; ++i) {
// 		idxData[i] = g_fbx->getMeshes()[0].vertIndicesList[i];
// 	}

	TD3D_SubResourceData srd;
	srd.SysMemPitch = 0;
	srd.SysMemSlicePitch = 0;
	srd.pSysMem = idxData;

	HR(device->CreateBuffer(&desc, &srd, &m_pIB));
//	SafeDeleteArray(idxData);
}


