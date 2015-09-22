#pragma once
#include "ModelBase.h"

namespace Fugui
{
	class ModelPlanar : public ModelBase
	{
	public:
		struct SimpleVert
		{
			DirectX::XMFLOAT3 position;
		};

		ModelPlanar();
		virtual ~ModelPlanar();

		virtual int loadFBXFrmt(ID3D_Device* device, ID3D_DeviceContext* context, const char* fileName, const char* modelName);
		virtual void update(float dt);
		virtual void render(ID3D_DeviceContext* context);
		virtual void shutdown();

	private:

		virtual void createVB(ID3D_Device* device);
		virtual void createIB(ID3D_Device* device);
	};

} // end namespace Fugui