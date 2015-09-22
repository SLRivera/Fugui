#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;
#include "d3dUtils.h"

struct FxParams {
	//vector<D3DXHANDLE>		techList;

	string					fxFileName;
	ID3DXEffect*		fx;
};

class EffectMngr
{
public:
	UINT createFx(const string& fxFileName);

	inline void setFx(UINT fxId, const D3DXHANDLE& tech);
	inline void setHandle(UINT fxId, const D3DXHANDLE& h);
// 	void setTech(D3DXHANDLE hTech);
// 	void setFxAndTech(UINT id, D3DXHANDLE hTech);

	inline D3DXHANDLE& getHandle(UINT fxId, const string& varName, const D3DXHANDLE& outHandle = 0, const D3DXHANDLE& offset = 0);
	inline D3DXHANDLE& getTechHandle(UINT fxId, const string& techName, D3DXHANDLE& outHandle);

	EffectMngr();
	~EffectMngr();

private:
	vector <FxParams>	m_fxParamsList;

	//vector <ID3DXEffect*>	m_fxList;
	//vector <string>	m_fxNameList;	// used to check to see if the effect has been loaded already

	static UINT m_fxCounter;
};

void EffectMngr::setFx(UINT fxId, const D3DXHANDLE& tech)
{
	m_fxParamsList[fxId].fx->SetTechnique(tech);
}

D3DXHANDLE EffectMngr::getHandle(UINT fxId, const string& varName, D3DXHANDLE& outHandle, D3DXHANDLE& offset /* = 0 */)
{
	outHandle = m_fxParamsList[fxId].fx->GetParameterByName(offset, varName.c_str());
}

D3DXHANDLE EffectMngr::getTechHandle(UINT fxId, const string& techName, D3DXHANDLE& outHandle)
{
	outHandle = m_fxParamsList[fxId].fx->GetTechniqueByName(techName.c_str());
}