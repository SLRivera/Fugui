#pragma once
//#include <fbxsdk.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#pragma comment(lib, "libfbxsdk-md.lib")

namespace Fugui
{
	class FbxFileReader
	{
	public:
		struct FbxMaterialInfo {
			std::wstring		sMtrlName;
			std::wstring		sBmpFileName;
			DirectX::XMFLOAT4		vDiffuse;
			DirectX::XMFLOAT4		vAmbient;
			DirectX::XMFLOAT4		vSpecular;
			DirectX::XMFLOAT4		vEmmissive;
			float	fTransparecyFactor;
			float	fSpecularFactor;
			bool	bHasBmp;
		};

		struct FbxMeshInfo {
			std::vector<FbxMaterialInfo>		materialsList;
			std::string									sMeshName;
			std::vector<DirectX::XMFLOAT4>	vertexList;
			std::vector<DirectX::XMFLOAT4>	normalsList;
			std::vector<DirectX::XMFLOAT4>	tangentsList;
			std::vector<DirectX::XMFLOAT4>  binormsList;
			std::vector<DirectX::XMFLOAT2>	uvsList;
			std::vector<int>		vertIndicesList;
			std::vector<int>		normIndicesList;
			std::vector<int>		uvIndicesList;

			int		nVertexCnt;
			int		nVertIdxCnt;
			int		nPolygonCnt;
			int		nMaterialCnt;
		};

		std::vector<FbxMeshInfo>& getMeshes() { return m_meshList; }

		static void initReader();
		static void shutdownReader();

		int loadFbxFile(const char* fileName, const char* sceneName);
		void getFbxFileData();

		~FbxFileReader();

	private:
		std::vector<FbxMeshInfo>	m_meshList;
		FbxManager*		m_pManager;
		FbxIOSettings*	m_pIOSettings;
		FbxScene*			m_pScene;
		FbxNode*			m_pRoot;

		void getMeshData(FbxNode* node);
		void getFbxFileData(FbxNode* node);

		FbxFileReader();
		FbxFileReader(const FbxFileReader&);
	};

}

extern Fugui::FbxFileReader* g_fbx;
