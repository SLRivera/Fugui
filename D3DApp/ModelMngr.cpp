#include "ModelMngr.h"
//#include "../FbxReader/FbxFileReader.h"

using namespace Fugui;
using namespace DirectX;

ModelMngr::ModelMngr()
{
}


ModelMngr::~ModelMngr()
{
}

int Fugui::ModelMngr::loadModel(char* fbxFileName, char* meshName, ID3D_Device* device, ID3D_DeviceContext* context)
{
// 	FbxFileReader fbx;
// 
// 	fbx.loadFbxFile(fbxFileName, meshName);
// 	fbx.getFbxFileData();
 	static int ID = 0;
// 
// 	for (UINT i = 0; i < fbx.getMeshes().size(); ++i) {
// 		ModelInfo newModel;
// 
// 		TD3D_BufferDesc desc;
// 		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
// 		desc.ByteWidth = sizeof(VertexVSPNTBT) * fbx.getMeshes()[i].nVertexCnt;
// 		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
// 		desc.MiscFlags = 0;
// 		//desc.StructureByteStride = 0;
// 		desc.Usage = D3D11_USAGE_DYNAMIC;
// 
// 		HR(device->CreateBuffer(&desc, NULL, &newModel.vb));
// 
//  		VertexVSPNTBT* verts = NULL;
//  		D3D11_MAPPED_SUBRESOURCE mr;
//  
//  		HR(context->Map(newModel.vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr));
//  		verts = (VertexVSPNTBT*)mr.pData;
//  
//  		for (int j = 0; j < fbx.getMeshes()[i].nVertexCnt; ++j) {
//  		//	verts[j].position = XMFLOAT3(fbx.getMeshes()[i].vertices[j][0], fbx.getMeshes()[i].vertices[j][1], fbx.getMeshes()[i].vertices[j][2]);
//  		}
//  		context->Unmap(newModel.vb, 0);
// 
// 		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
// 		desc.ByteWidth = sizeof(DWORD) * fbx.getMeshes()[i].vVertIdxCnt;
// 
// 		HR(device->CreateBuffer(&desc, NULL, &newModel.ib));
// 		HR(context->Map(newModel.ib, NULL, D3D11_MAP_WRITE_DISCARD, 0, &mr));
// 
// 		DWORD* idxs = (DWORD*)mr.pData;
// 		for (int j = 0; j < fbx.getMeshes()[i].vVertIdxCnt; ++j) {
// 			idxs[j] = fbx.getMeshes()[i].vertIndices[j];
// 		}
// 
// 		m_modelList.push_back(newModel);
// 	} // end for(i)
	
	return ID++;
}

void Fugui::ModelMngr::render(int idx, ID3D_Device* device, ID3D_DeviceContext* context)
{

}
