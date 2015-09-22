#include "FbxFileReader.h"

using namespace DirectX;
using namespace Fugui;

FbxFileReader*	g_fbx = nullptr;

FbxFileReader::FbxFileReader()
{
	m_meshList.resize(0);
}


FbxFileReader::~FbxFileReader()
{
	m_pManager->Destroy();
}

int Fugui::FbxFileReader::loadFbxFile(const char* fileName, const char* sceneName)
{
	m_pManager = FbxManager::Create();

	m_pIOSettings = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(m_pIOSettings);

	FbxImporter* imp = FbxImporter::Create(m_pManager, "");
	if (false == imp->Initialize(fileName, -1, m_pManager->GetIOSettings())) {
		printf("FBX Importer creation failed.\n");
		printf("Error: %s\n", imp->GetStatus().GetErrorString());
		exit(EXIT_FAILURE);
	}

	m_pScene = FbxScene::Create(m_pManager, sceneName);
	if (m_pScene) {
		if (false == imp->Import(m_pScene))
			exit(EXIT_FAILURE);
	}
	else
		exit(EXIT_FAILURE);
	
	imp->Destroy();

	m_pRoot = m_pScene->GetRootNode();
	if (NULL == m_pRoot)
		exit(EXIT_FAILURE);

	return m_meshList.size();
}

void Fugui::FbxFileReader::initReader()
{
	if (nullptr == g_fbx)
		g_fbx = new FbxFileReader;
}

void Fugui::FbxFileReader::shutdownReader()
{
	if (g_fbx) {
		delete g_fbx;
		g_fbx = nullptr;
	}
}

void FbxFileReader::getMeshData(FbxNode* pNode)
{
	FbxMeshInfo newMesh;
	FbxMesh* mesh = (FbxMesh*)pNode->GetNodeAttribute();

	m_meshList.push_back(newMesh);
	m_meshList.back().sMeshName = mesh->GetNameOnly();
	m_meshList.back().nVertexCnt = mesh->GetControlPointsCount();
	m_meshList.back().nPolygonCnt = mesh->GetPolygonCount();
	m_meshList.back().nVertIdxCnt = mesh->GetPolygonVertexCount();
	//m_meshList.back().nMaterialCnt = mesh->Get

	FbxVector4* ctrlPoints = mesh->GetControlPoints();
	for (int i = 0; i < m_meshList.back().nVertexCnt; ++i) {
		XMFLOAT4 currVert;
		currVert.x = (float)ctrlPoints[i][0];
		currVert.y = (float)ctrlPoints[i][1];
		currVert.z = (float)ctrlPoints[i][2] * -1.0f;
		currVert.w = 1.0f;

		m_meshList.back().vertexList.push_back(currVert);
	}

	for (int i = 0; i < m_meshList.back().nPolygonCnt; ++i) {
		int startIdx = mesh->GetPolygonVertexIndex(i);
		int vertPerPoly = mesh->GetPolygonSize(i);
		int* vertIdxs = &mesh->GetPolygonVertices()[startIdx];
		for (int j = 0; j < vertPerPoly; ++j) {
			m_meshList.back().vertIndicesList.push_back(vertIdxs[j]);
		}
	}

// 	newMesh.meshName = mesh->GetName();
// 	newMesh.polygonCnt = mesh->GetPolygonCount();
// 	int test = mesh->GetPolygonVertexCount();
// 	int* test2 = mesh->GetPolygonVertices();
// 	int test3 = mesh->GetPolygonSize(0);
// 	newMesh.vertexCnt = mesh->GetControlPointsCount();
// 	 
// 	FbxVector4* ctrlPoints = mesh->GetControlPoints();
// 	newMesh.vertices.reserve(newMesh.vertexCnt);
// 	for (int i = 0; i < newMesh.vertexCnt; ++i) {
// 		newMesh.vertices.push_back(ctrlPoints[i]);
// 		newMesh.vertices[i][2] *= -1.0f;
// 	}
// 	
// 	newMesh.vertIdxCnt = mesh->GetPolygonVertexCount();;
// 	//newMesh.vertIndices.reserve(newMesh.vertIdxCnt);
// 	for (int i = 0; i < newMesh.polygonCnt; ++i) {
// 		int idx0 = mesh->GetPolygonVertex(i, 0);
// 		int idx1 = mesh->GetPolygonVertex(i, 1);
// 		int idx2 = mesh->GetPolygonVertex(i, 2);
// 		newMesh.vertIndices.push_back(idx0);
// 		newMesh.vertIndices.push_back(idx2);
// 		newMesh.vertIndices.push_back(idx1);
// 	}
// 
// // 	newMesh.materialCnt = mesh->GetElementMaterialCount();
// // 	newMesh.materials.reserve(newMesh.materialCnt);
// // 	for (int i = 0; i < newMesh.materialCnt; ++i) {
// // 		FbxGeometryElementMaterial* material = mesh->GetElementMaterial(i);
// // 		material->GetDirectArray().GetAt(i).gete
// // 	}
// 
// 	m_meshList.push_back(newMesh);
}

void FbxFileReader::getFbxFileData()
{
	if (NULL == m_pRoot)
		exit(EXIT_FAILURE);

	FbxNode* node = m_pRoot;
	for (int i = 0; i < node->GetChildCount(); i++)
		getFbxFileData(node->GetChild(i));
}

void FbxFileReader::getFbxFileData(FbxNode* node)
{
	if (NULL == node)
		return;

	FbxNodeAttribute::EType nodeType;
	nodeType = node->GetNodeAttribute()->GetAttributeType();

	switch (nodeType)
	{
		case FbxNodeAttribute::eMesh:
			getMeshData(node);
			break;
		default: return;
	} // end switch(nodeType)
}
