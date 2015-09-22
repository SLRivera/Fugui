#include "D3DUtils.h"
using std::vector;
using namespace DirectX;
using namespace Fugui;

void GenTriGrid(int numVertRows,
	int numVertCols,
	float dx,
	float dz,
	const XMFLOAT3& center,
	vector<XMFLOAT3>& verts,
	vector<DWORD>& indices)
{
	int numVertices = numVertRows * numVertCols;
	int numCellRows = numVertRows - 1;
	int numCellCols = numVertCols - 1;

	int numTris = numCellRows * numCellCols * 2;

	float width = (float)numCellCols * dx;
	float depth = (float)numCellRows * dz;

	//===========================================
	// Build vertices.

	// We first build the grid geometry centered about the origin and on
	// the xz-plane, row-by-row and in a top-down fashion.  We then translate
	// the grid vertices so that they are centered about the specified 
	// parameter 'center'.

	verts.resize(numVertices);

	// Offsets to translate grid from quadrant 4 to center of 
	// coordinate system.
	float xOffset = -width * 0.5f;
	float zOffset = depth * 0.5f;

	int k = 0;
	for (float i = 0; i < numVertRows; ++i)
	{
		for (float j = 0; j < numVertCols; ++j)
		{
			// Negate the depth coordinate to put in quadrant four.  
			// Then offset to center about coordinate system.
			verts[k].x = j * dx + xOffset;
			verts[k].z = -i * dz + zOffset;
			verts[k].y = 0.0f;

			// Translate so that the center of the grid is at the
			// specified 'center' parameter.
			XMMATRIX T;
			T = XMMatrixTranslation(center.x, center.y, center.z);

			XMVECTOR v;
			v = XMLoadFloat3(&verts[k]);
			v = XMVector3TransformCoord(v, T);

// 			XMVectorGetXPtr(&verts[k].x, v);
// 			XMVectorGetYPtr(&verts[k].y, v);
			XMStoreFloat3(&verts[0], v);

			++k; // Next vertex
		}
	}

	//===========================================
	// Build indices.

	indices.resize(numTris * 3);

	// Generate indices for each quad.
	k = 0;
	for (DWORD i = 0; i < (DWORD)numCellRows; ++i)
	{
		for (DWORD j = 0; j < (DWORD)numCellCols; ++j)
		{
			indices[k] = i    * numVertCols + j;
			indices[k + 1] = i    * numVertCols + j + 1;
			indices[k + 2] = (i + 1) * numVertCols + j;

			indices[k + 3] = (i + 1) * numVertCols + j;
			indices[k + 4] = i    * numVertCols + j + 1;
			indices[k + 5] = (i + 1) * numVertCols + j + 1;

			// next quad
			k += 6;
		}
	}
}

float Fugui::getRandomFloat(const float min, const float max)
{
	if (min >= max) // bad input
		return min;

	// Get random float in [0, 1] interval.
	float f = (rand() % 10001) * 0.0001f;

	return (f * (max - min)) + min;
}

void Fugui::getRandomVec(XMFLOAT3& vOut)
{
	vOut.x = getRandomFloat(-1.0f, 1.0f);
	vOut.y = getRandomFloat(-1.0f, 1.0f);
	vOut.z = getRandomFloat(-1.0f, 1.0f);

	// Project onto unit sphere.
	XMVECTOR v;
	v = XMLoadFloat3(&vOut);
	v = XMVector3Normalize(v);
	XMStoreFloat3(&vOut, v);
	//XMVectorGetYPtr(&vOut.y, v);
	//XMVectorGetZPtr(&vOut.z, v);
}