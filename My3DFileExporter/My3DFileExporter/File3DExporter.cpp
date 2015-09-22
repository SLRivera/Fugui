//#include <fstream>
#include "File3DExporter.h"
using std::ifstream;

File3DExporter* g_3dExporter = nullptr;

unsigned int File3DExporter::m_nModelID = -1;

File3DExporter::File3DExporter() :
m_pScene(nullptr),
m_pCurrBinorms(nullptr),
m_pCurrMesh(nullptr),
m_pCurrNormals(nullptr),
m_pCurrTangents(nullptr),
m_pCurrUVs(nullptr),
m_pCurrVerts(nullptr),
m_nNumAnimations(0),
m_nNumBones(0),
m_nNumMaterials(0),
m_nNumMeshes(0),
m_nNumTextures(0),
m_nNumVerts(0)
{
}


File3DExporter::~File3DExporter()
{
	
}

void File3DExporter::initExporter()
{
	if (nullptr == g_3dExporter)
		g_3dExporter = new File3DExporter;
}

void File3DExporter::shutdownExporter()
{
	if (g_3dExporter) {
		delete g_3dExporter;
		g_3dExporter = nullptr;
	}
}

unsigned int File3DExporter::load3dModel(const char* fileName, bool convLeftHand, bool flipUv, bool flipWindingOrder)
{
	ifstream test(fileName);
	if (test.fail())
		return -1;
	test.close();

	unsigned int flags = 0;// */ | aiProcess_JoinIdenticalVertices | aiProcess_ValidateDataStructure;
	
	if (convLeftHand)
		flags |= aiProcess_ConvertToLeftHanded ;
	if (flipUv)
		flags |= aiProcess_TransformUVCoords; 
	if (flipWindingOrder)
		flags |= aiProcess_FlipWindingOrder;

	flags = aiProcess_JoinIdenticalVertices  | aiProcess_Triangulate |
				/*aiProcess_OptimizeMeshes | */aiProcess_ConvertToLeftHanded | aiProcess_FindInstances
				/*aiProcess_SortByPType*/ /*| aiProcess_CalcTangentSpace*/;

	m_pScene = m_importer.ReadFile(fileName, flags);
	
	// grab scene file data
	m_nNumMeshes = m_pScene->mNumMeshes;
	m_nNumMaterials = m_pScene->mNumMaterials;
	m_nNumAnimations = m_pScene->mNumAnimations;
	m_nNumTextures = m_pScene->mNumTextures;
	m_nNumMaterials = m_pScene->mNumMaterials;

	m_bHasMaterials =  m_pScene->HasMaterials();
	m_bHasTextures = m_pScene->HasTextures();
	
	return ++m_nModelID;
}


void File3DExporter::export3dFileData(const char* outputName)
{
	m_outStream.open(outputName, std::ios::out | std::ios::trunc /*| std::ios::binary*/);
	if (m_outStream.fail())
		return;

	m_outStream << "My 3D File:" << outputName << '\n';

	m_outStream << "Number of Meshes:" << m_nNumMeshes << '\n';
	m_outStream << "Number of Materials:" << m_nNumMaterials << '\n';
	m_outStream << "Number of Animations:" << m_nNumAnimations << '\n';
	m_outStream << "Number of Textures:" << m_nNumTextures << '\n';

	//export mesh info
	for (unsigned int i = 0; i < m_nNumMeshes; ++i) {
		m_outStream << "Mesh " << i << " Info:\n";

		m_pCurrMesh = m_pScene->mMeshes[i];

		m_bHasNormals = m_pCurrMesh->HasNormals();
		m_bHasUVs = m_pCurrMesh->HasTextureCoords(0);
		m_bHasFaces = m_pCurrMesh->HasFaces();
		m_bHasTangents = m_pCurrMesh->HasTangentsAndBitangents();

		m_nNumVerts = m_pCurrMesh->mNumVertices;
		m_nNumFaces = m_pCurrMesh->mNumFaces;

		// write mesh header info
		m_outStream << "Has Normals:" << m_bHasNormals << '\n';
		m_outStream << "Has UVs:" << m_bHasUVs << '\n';
		m_outStream << "Has Tangents:" << m_bHasTangents << '\n';
		m_outStream << "Vertex Count:" << m_nNumVerts << '\n';
		m_outStream << "Face Count:" << m_nNumFaces << '\n';
		 
		// all meshes have the following two
		getVertices(i);
		getFaces(i);
		if (m_bHasNormals)
			getNormals(i);
		if (m_bHasUVs)
			getUVs(i);
		if (m_bHasMaterials)
			getMaterials(i);
		if (m_bHasTangents) {
			getTangents(i);
			getBinorms(i);
		}
	}

	m_outStream.close();
}

void File3DExporter::getVertices(int meshID)
{
	m_outStream << "Vertices for Mesh:" << meshID << ":\n";

	for (unsigned int i = 0; i < m_nNumVerts; ++i) {
		m_outStream << i << ':' << m_pCurrMesh->mVertices[i].x << ',' << m_pCurrMesh->mVertices[i].y << ',' << m_pCurrMesh->mVertices[i].z << '\n';
	}

	//m_outStream << '\n';
}

void File3DExporter::getNormals(int meshID)
{
	m_outStream << "Normals for Mesh:" << meshID << ":\n";

	for (unsigned int i = 0; i < m_nNumVerts; ++i) 
		m_outStream << i << ':' << m_pCurrMesh->mNormals[i].x << ',' << m_pCurrMesh->mNormals[i].y << ',' << m_pCurrMesh->mNormals[i].z << '\n';

	//m_outStream << '\n';
}

void File3DExporter::getUVs(int meshID)
{
	m_outStream << "TexCoords for Mesh:" << meshID << '\n';

	for (unsigned int i = 0; i < m_nNumVerts; ++i)
		m_outStream << i << ':' << m_pCurrMesh->mTextureCoords[0][i].x << ',' << m_pCurrMesh->mTextureCoords[0][i].y << '\n';

	//m_outStream << '\n';
}

void File3DExporter::getFaces(int meshID)
{
	m_outStream << "Faces for Mesh:" << meshID << '\n';

	unsigned int numIndeces = 0;

//	m_outStream << "Faces Count:" << m_nNumFaces << '\n';

	for (unsigned int i = 0; i < m_nNumFaces; ++i) {
		numIndeces = m_pCurrMesh->mFaces[i].mNumIndices;
		m_outStream << "Face " << i << ": \n";
		for (unsigned int j = 0; j < numIndeces; ++j) {
			m_outStream << j << ':' << m_pCurrMesh->mFaces[i].mIndices[j] << '\n';
		}
	}

//	m_outStream << '\n';
}

void File3DExporter::getMaterials(int meshID)
{
	m_outStream << "Materials for Mesh:" << meshID << '\n';

	for (unsigned int i = 0; i < m_nNumMaterials; ++i) {
		aiMaterial* currMat = m_pScene->mMaterials[i];

		m_outStream << "Material:" << i << '\n';
		unsigned int numDiffuseTex = currMat->GetTextureCount(aiTextureType_DIFFUSE);
		unsigned int numBumpTex = currMat->GetTextureCount(aiTextureType_NORMALS);
		unsigned int numMatColors = currMat->GetTextureCount(aiTextureType_NONE);
		m_outStream << "Diffuse Texture Count:" << numDiffuseTex << '\n';

		aiString fileName;
		for (unsigned int j = 0; j < numDiffuseTex; ++j)
			if (AI_SUCCESS == currMat->GetTexture(aiTextureType_DIFFUSE, j, &fileName))
				m_outStream << "Texture:" << j << ':' << fileName.C_Str() << '\n';
	}

	//m_outStream << '\n';
}

void File3DExporter::getTangents(int meshID)
{
	m_outStream << "Tangents for Mesh:" << meshID << '\n';

	for (unsigned int i = 0; i < m_nNumVerts; ++i) 
		m_outStream << i << ':' << m_pCurrMesh->mTangents[i].x << ',' << m_pCurrMesh->mTangents[i].y << ',' << m_pCurrMesh->mTangents[i].z << '\n';

//	m_outStream << '\n';
}

void File3DExporter::getBinorms(int meshID)
{
	m_outStream << "BiNormals for Mesh:" << meshID << '\n';

	for (unsigned int i = 0; i < m_nNumVerts; ++i)
		m_outStream << i << ':' << m_pCurrMesh->mBitangents[i].x << ',' << m_pCurrMesh->mBitangents[i].y << ',' << m_pCurrMesh->mBitangents[i].z << '\n';

	//m_outStream << '\n';
}
