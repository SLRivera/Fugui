#pragma once
#include <string>
#include "D3DUtils.h"

namespace Fugui
{
	class ModelBase
	{	
	public:
		virtual int loadModelFile(ID3D_Device* device, ID3D_DeviceContext* context, const char* fileName) = 0;
		virtual void update (float dt) = 0;
		virtual void render(ID3D_DeviceContext* context) = 0;
		virtual void shutdown() = 0;

		ModelBase();
		virtual ~ModelBase() = 0;
			
	protected:
		DirectX::XMFLOAT4X4	m_mWorld;

		DirectX::XMFLOAT3		m_vPosition;
		DirectX::XMFLOAT3		m_vScale;
		DirectX::XMFLOAT3		m_vRotation;

		std::string	m_sName;

		ID3D_Buffer*		m_pVB;
		ID3D_Buffer*		m_pIB;
		ID3D_ShaderResourceView*	m_pDiffuseTex;

		int		m_nNumVerts;
		int		m_nNumIndices;
		int		m_nNumPolys;

		static int m_nID;

		virtual void createVB(ID3D_Device* device, ID3D_DeviceContext* context) = 0;
		virtual void createIB(ID3D_Device* device, ID3D_DeviceContext* context) = 0;
	};

} // end namespace Fugui
