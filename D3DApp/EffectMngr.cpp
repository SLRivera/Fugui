#include "EffectMngr.h"

UINT EffectMngr::m_fxCounter = 0;

EffectMngr::EffectMngr()
{
}


EffectMngr::~EffectMngr()
{
}

UINT EffectMngr::createFx(const string& fxFileName)
{
	for (UINT i = 0; i < m_fxParamsList.size(); ++i) {
		if (fxFileName == m_fxParamsList[i].fxFileName)
			return -1;

	}

	ID3DXBuffer* errs = NULL;
	FxParams newFx;
	HR(D3DXCreateEffectFromFile(gd3dDevice, fxFileName.c_str(),
											NULL, NULL, D3DXSHADER_DEBUG, 0, 
											&newFx.fx, &errs));

	if (errs) {
		HR(E_FAIL);
		return -1;
	}

	m_fxParamsList.push_back(newFx);
	return m_fxParamsList.size() - 1;
}
