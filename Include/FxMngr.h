#pragma once
#include <vector>
#include <string>
#include "D3DUtils.h"

namespace Fugui
{
	struct CbVSNeverChanges {
		DirectX::XMFLOAT4X4		mView;
	};
	const static UINT g_nCbVSNeverChangesIdx = 0;

	struct CbVSChangesOnResize {
		DirectX::XMFLOAT4X4		mProj;
	};
	const static UINT g_nCbVSChangesOnResizeIdx = 1;

	struct CbVSChangesPerFrame {
		DirectX::XMFLOAT4X4		mWorld;
		DirectX::XMFLOAT4A	vClipNear;
		DirectX::XMFLOAT4A	vClipFar;
		DirectX::XMFLOAT4A	vClipLeft;
		DirectX::XMFLOAT4A	vClipRight;
		DirectX::XMFLOAT4A	vClipTop;
		DirectX::XMFLOAT4A	vClipBottom;
	};
	const static UINT g_nCbVSChangesPerFrameIdx = 2;

	struct CbPSPerPrimitive {
		DirectX::XMFLOAT4A	mtrlDiffuseColor;
		DirectX::XMFLOAT4A	mtrlAmbientColor;
		DirectX::XMFLOAT4A	mtrlSpecularColor;
		float	fSpecularExponent;
	};
	const static UINT g_nCbPSPerPrimitiveIdx = 0;

	struct CbPSChangesPerFrame {

		DirectX::XMFLOAT4A	lightDiffuseColor;
		DirectX::XMFLOAT4A	lightAmbientColor;
		DirectX::XMFLOAT4A	lightSpecularColor;
		DirectX::XMFLOAT3A	vEyePos;
		DirectX::XMFLOAT3A	vLightPos;
		DirectX::XMFLOAT3A	vLightDir;
	};
	const static UINT g_nCbPSChangesPerFrameIdx = 1;

	class FxMngr
	{
	public:
		struct PSData {
			ID3D_PShader*	pPS;
			ID3D_SamplerState* pSamplerState;
		};

		struct VSData {
			ID3D_VShader*	pVS;
		};

		enum VertexTypes
		{
			VertexP = 0,				// position data 
			VertexPT,			// position / texture data
			VertexPN,			// position / normal data
			VertexPNT,			// position / normal / texture data
			VertexPNTBT		// position / normal / tangent / binormal / texture data
		};

		void createSamplerState(UINT idx, ID3D_Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode, UINT anisotropy, D3D11_COMPARISON_FUNC compFunc);
		void createCBs(ID3D_Device* device);

		void setCbVSNeverChanges(ID3D_DeviceContext* context, DirectX::XMFLOAT4X4& view);
		void setCbVSChangesOnResize(ID3D_DeviceContext* context, DirectX::XMFLOAT4X4& projection);
		void setCbVSChangesPerFrame(ID3D_DeviceContext* context, DirectX::XMFLOAT4X4& world);
		void setCbPSPerPrimitive(ID3D_DeviceContext* context, DirectX::XMFLOAT4A& diffuse, DirectX::XMFLOAT4A& ambient, DirectX::XMFLOAT4A& specular, float specExponent);
		void setCbPSChangesPerFrame(ID3D_DeviceContext* context, CbPSChangesPerFrame& info);

		UINT createPSFx(ID3D_Device* device, TCHAR* fxFilename, char* entryName, char* shaderModel);
		UINT createVSFx(ID3D_Device* device, TCHAR* fxFilename, char* entryName, char* shaderModel, VertexTypes type);
		
		ID3D_InputLayout* getVertexDesc(VertexTypes type);

		inline const ID3D_PShader*	getPS(UINT idx) const;
		inline const ID3D_VShader*	getVS(UINT idx) const;
		inline ID3D_SamplerState* getPSSampler(UINT idx);
		inline const ID3D_InputLayout* getVertDesc(VertexTypes type) const;
		inline const ID3D_InputLayout* getVertDesc(UINT idx) const;
		
		FxMngr(); 
		~FxMngr();

	protected:
		std::vector<PSData>	m_PSList;
		std::vector<VSData>	m_VSList;

		//ID3D_Buffer*		cbPSNeverChanges;
		ID3D_Buffer*		m_pCbPSPerPrimitive;
		ID3D_Buffer*		m_pCbPSChangesPerFrame;

		ID3D_Buffer*		m_pCbVSNeverChanges;
		ID3D_Buffer*		m_pCbVSChangesOnResize;
		ID3D_Buffer*		m_pCbVSChangesPerFrame;

		static ID3D11InputLayout*	m_pVP;
		static ID3D11InputLayout*	m_pVPT;
		static ID3D11InputLayout*	m_pVPN;
		static ID3D11InputLayout*	m_pVPNT;
		static ID3D11InputLayout*	m_pVPNTBT;

		static UINT 	m_nPSCounter;
		static UINT		m_nVSCounter;
	
		void initVertexDesc(ID3D_Device* device, ID3D_Blob* shader, VertexTypes type);
	};

	ID3D_SamplerState* FxMngr::getPSSampler(UINT idx) { return m_PSList[idx].pSamplerState; }
	const ID3D_PShader*	FxMngr::getPS(UINT idx) const { return m_PSList[idx].pPS; }
	const ID3D_VShader*	FxMngr::getVS(UINT idx) const { return m_VSList[idx].pVS; }
	const ID3D_InputLayout* FxMngr::getVertDesc(VertexTypes type) const 
	{
		switch (type)
		{
		case VertexP: return m_pVP;
		case VertexPT: return m_pVP;
		case VertexPN: return m_pVPN;
		case VertexPNT: return m_pVPNT;
		case VertexPNTBT: return m_pVPNTBT;
		default: return nullptr;
		}
	}
	//const ID3D_InputLayout* FxMngr::getVertDesc(UINT idx) const { return m_PSList[idx].pSamplerState; }

} // end namespace Fugui
