#pragma once
#include "D3DUtils.h"

#define PIXEL_SHADER_MODEL		"ps_5_0"
#define VERTEX_SHADER_MODEL	"vs_5_0"

namespace Fugui
{
	class ShaderBaseInfo
	{
	public:
		virtual int initShader(ID3D_Device* device, TCHAR* vsFileName, TCHAR* psFileName, char* vsEntry, char* psEntry);

		virtual void initInputLayout(ID3D_Device* device) = 0;
		virtual void initSamplerState(ID3D_Device* device) = 0;
		virtual void createBuffers(ID3D_Device* device) = 0;

		virtual void setShader(ID3D_DeviceContext* context);

		ShaderBaseInfo();
		ShaderBaseInfo(const ShaderBaseInfo&);
		virtual ~ShaderBaseInfo() = 0;

	protected:
		ID3D_VShader*		m_pVS;
		ID3D_PShader*		m_pPS;
		ID3D_InputLayout*	m_pInputLayout;
		ID3D_SamplerState*	m_pSamplerState;
		ID3D_Blob*		m_pVSBuffer;

		static int m_nID;
	};

} // end namespace Fugui