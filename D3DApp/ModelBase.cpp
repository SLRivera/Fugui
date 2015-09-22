#include "ModelBase.h"

using namespace Fugui;

int ModelBase::m_nID = -1;

ModelBase::ModelBase() :
m_pDiffuseTex(nullptr),
m_pIB(nullptr),
m_pVB(nullptr)
{
}


ModelBase::~ModelBase()
{
	SafeRelease(m_pDiffuseTex);
	SafeRelease(m_pIB);
	SafeRelease(m_pVB);
}




