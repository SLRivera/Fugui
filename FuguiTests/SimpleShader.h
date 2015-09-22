#pragma once
#include "ShaderBaseInfo.h"

namespace Fugui
{
	class SimpleShader : public ShaderBaseInfo
	{
	public:
		struct cbMatrices
		{
			DirectX::XMFLOAT4X4 view;
			DirectX::XMFLOAT4X4 projection;
			DirectX::XMFLOAT4X4 world;
		};

		void setBuffers(ID3D_DeviceContext* context, DirectX::XMFLOAT4X4& v, DirectX::XMFLOAT4X4& w, DirectX::XMFLOAT4X4& p);

		virtual void createBuffers(ID3D_Device* device);
		virtual void initInputLayout(ID3D_Device* device);
		virtual void initSamplerState(ID3D_Device* device);

		SimpleShader();
		virtual ~SimpleShader();

	private:
		ID3D_Buffer*	m_cbMatrices;
	};

} // namespace Fugui