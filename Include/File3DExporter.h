#pragma once
#include <vector>
#include <map>
#include <fstream>
#include <Importer.hpp>
#include "PostProcess.h"
#include "Scene.h"
#include "DefaultLogger.hpp"
#include "LogStream.hpp"

#pragma comment(lib, "assimp.lib")

class File3DExporter
{
public:
	struct ExpFileData {

	};

	void initExporter();
	void shutdownExporter();
	void export3dFileData(const char* outputName);

	// this function assumes that there is only one mesh in the file being loaded
	unsigned int load3dModel(const char* fileName, bool convLeftHand, bool flipUv, bool flipWindingOrder);

	// this function assumes that there are more than one mesh in the file
	// returns an array for the IDs of the meshes found in the scene, n=0 being the first mesh found, etc.
	unsigned int* load3dScene(const char* sceneName, bool convLftHand, bool convUv);

	~File3DExporter();

private:
	Assimp::Importer	m_importer;

	std::ofstream			m_outStream;

	const aiScene*		m_pScene;

	aiMesh*			m_pCurrMesh; 
	aiVector3D*	m_pCurrVerts;
	aiVector3D*	m_pCurrNormals;
	aiVector3D*	m_pCurrTangents;
	aiVector3D*	m_pCurrBinorms;
	aiVector2D*	m_pCurrUVs;

	aiVector3D		m_vSceneMin;
	aiVector3D		m_vSceneMax;
	aiVector3D		m_vScenenCenter;

	unsigned int		m_nNumVerts;
	unsigned int		m_nNumFaces;
	unsigned int		m_nNumIndices;
	unsigned int		m_nNumMaterials;
	unsigned int		m_nNumBones;
	unsigned int		m_nNumMeshes;
	unsigned int		m_nNumAnimations;
	unsigned int		m_nNumTextures;

	bool	m_bHasNormals;
	bool	m_bHasTangents;
	bool	m_bHasBinorms;
	bool	m_bHasUVs;
	bool	m_bHasFaces;
	bool	m_bHasMaterials;
	bool	m_bHasTextures;

	static unsigned int	m_nModelID;

	//----------------------------------------------------------------------
	// One mesh per file functions
	//----------------------------------------------------------------------
	void getVertices(int meshID);
	void getNormals(int meshID);
	void getFaces(int meshID);
	void getTangents(int meshID);
	void getBinorms(int meshID);
	void getUVs(int meshID);
	void getMaterials(int meshID);
	void getTextures(int meshID);

	//----------------------------------------------------------------------
	// Multiple meshes per file functions
	//----------------------------------------------------------------------
	File3DExporter();
};

extern File3DExporter* g_3dExporter;