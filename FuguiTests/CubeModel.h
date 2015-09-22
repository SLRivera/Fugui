#pragma once
#include <vector>
#include "D3DUtils.h"
#include "ModelBase.h"

namespace Fugui
{
	class CubeModel : public ModelBase
	{
	public:
		CubeModel();
		virtual ~CubeModel();

		virtual int loadModelFile(ID3D_Device* device, ID3D_DeviceContext* context, const char* fileName);
		virtual void update(float dt);
		virtual void render(ID3D_DeviceContext* context);
		virtual void shutdown();

	protected:
		std::vector<VertexVSPNT>	m_vertexList;
		std::vector<DWORD>			m_indexList;

		virtual void createVB(ID3D_Device* device, ID3D_DeviceContext* context);
		virtual void createIB(ID3D_Device* device, ID3D_DeviceContext* context);
	};

} //end namespace Fugui