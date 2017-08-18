#include "StdAfx.h"
#include "BaseObject.h"

CBaseObject::CBaseObject(void)
{
}

CBaseObject::~CBaseObject(void)
{
}

void CBaseObject::LoadResFile()
{

}

void CBaseObject::Transform()
{
	D3DXMATRIX	m_matTrans, m_matRot, m_matScal;
	D3DXMatrixTranslation(&m_matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixRotationYawPitchRoll(&m_matRot, m_vRot.y, m_vRot.x, m_vRot.z);
	D3DXMatrixScaling(&m_matScal, m_vScal.x, m_vScal.y, m_vScal.z);
	m_matWorld=m_matRot*m_matScal*m_matTrans;//顺序不可变：旋转矩阵*缩放矩阵*平移矩阵

	m_matOrigWorld = /*m_matScal * m_matTrans*/m_matWorld;

	D3DXVec3TransformCoord(&m_vMax, &m_vOrigMax, &m_matOrigWorld);
	D3DXVec3TransformCoord(&m_vMin, &m_vOrigMin, &m_matOrigWorld);
}