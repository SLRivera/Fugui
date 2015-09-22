#pragma once
#include <string>
#include "D3DUtils.h"

namespace Fugui
{ 
	class ModelMngr
	{
	public:
		struct ModelInfo {
			ID3D_Buffer*	vb;
			ID3D_Buffer*	ib;
		};

		int loadModel(char* fbxFileName, char* meshName, ID3D_Device* device, ID3D_DeviceContext* context);
		void render(int idx, ID3D_Device* device, ID3D_DeviceContext* context);
		 
		ModelMngr();
		~ModelMngr();

	protected:
		std::vector<ModelInfo>	m_modelList;
	};

} // end namespace Fugui