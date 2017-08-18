#include "StdAfx.h"
#include "Pet.h"
#include "DXDevice.h"

CPet::CPet()
{
	m_pAI=NULL;
	m_pSkinMesh=NULL;
	m_fDistanceWithMajor=3.0f;
}

CPet::~CPet(void)
{
	DeleteAndNull(m_pAI);
	DeleteAndNull(m_pSkinMesh);
}

HRESULT CPet::Init()
{
	m_pDevice=CDXDevice::Instance()->GetD3Device();
	m_pTerrain = CDXDevice::Instance()->GetScene()->GetCurTerrain();
	m_pSkinMesh = new CSkinMesh(m_pDevice);
	m_pAI  = new CAnimInstance;
	if(!m_pSkinMesh || !m_pAI)
		return E_FAIL;

	if(FAILED(m_pSkinMesh->LoadMesh(CResFileManager::Instance()->GetPetSkinMeshFile().c_str(), 
		CResFileManager::Instance()->GetMajorTextureFile().c_str())))
		return E_FAIL;
	CResFileManager::Instance()->GetPetAttribute(
		m_vPos, m_vRot, m_vScal, m_dElapsedTime, m_fMoveSpeed, m_fMaxDistanceWithMajor);

	m_pAI->Init(m_pSkinMesh);
	m_pAI->SetTimeDelta(m_dElapsedTime);//设置每帧间隔时间

	m_pSkinMesh->GetBoundingB_S(&m_vOrigMax, &m_vOrigMin, &m_vCenter, &m_fRaius);//m_dElapsedTime0.1
	m_vOrigRot = m_vRot;
		
	Transform();

	m_State = wait;
	return S_OK;
}

void CPet::InitPos(D3DXVECTOR3 vMajorPos, D3DXVECTOR3 vMajorDir)
{
	D3DXVECTOR3 vMajorDirTemp = vMajorDir;
	D3DXVECTOR3 vFont,vBack,vLeft,vRight;
	D3DXMATRIX	matTrans;
	
	D3DXMatrixTranslation(&matTrans, vMajorPos.x, vMajorPos.y, vMajorPos.z);
	D3DXVec3TransformNormal(&vMajorDirTemp, &vMajorDirTemp, &matTrans);
	vFont = vMajorDirTemp*m_fMaxDistanceWithMajor;
	vFont.y = m_pTerrain->GetTHeight(vFont.x, vFont.z);
	
	vMajorDirTemp = vMajorDir;
	vMajorDirTemp.x=-vMajorDirTemp.x;
	vMajorDirTemp.z=-vMajorDirTemp.z;
	D3DXVec3TransformNormal(&vMajorDirTemp, &vMajorDirTemp, &matTrans);
	vBack = vMajorDirTemp*m_fMaxDistanceWithMajor;
	vBack.y = m_pTerrain->GetTHeight(vBack.x, vBack.z);

	vMajorDirTemp = vMajorDir;
	D3DXVec3Cross(&vMajorDirTemp, &vMajorDirTemp, &D3DXVECTOR3(0,1,0));
	D3DXVec3TransformNormal(&vMajorDirTemp, &vMajorDirTemp, &matTrans);
	vLeft = vMajorDirTemp*m_fMaxDistanceWithMajor;
	vLeft.y = m_pTerrain->GetTHeight(vLeft.x, vLeft.z);

	vMajorDirTemp = vMajorDir;
	D3DXVec3Cross(&vMajorDirTemp, &D3DXVECTOR3(0,1,0), &vMajorDirTemp);
	D3DXVec3TransformNormal(&vMajorDirTemp, &vMajorDirTemp, &matTrans);
	vRight = vMajorDirTemp*m_fMaxDistanceWithMajor;
	vRight.y = m_pTerrain->GetTHeight(vRight.x, vRight.z);

	float fSlopeFont, fSlopeBack, fSlopeLeft, fSlopeRight;
	fSlopeFont = CalcSlope(vFont, vMajorPos);
	fSlopeBack = CalcSlope(vBack, vMajorPos);
	fSlopeLeft = CalcSlope(vLeft, vMajorPos);
	fSlopeRight = CalcSlope(vRight, vMajorPos);

	if(UP_SLOPE >= fSlopeBack && DOWN_SLOPE <= fSlopeBack)
		m_vPos = vBack;
	else if(UP_SLOPE >= fSlopeLeft && DOWN_SLOPE <= fSlopeLeft)
		m_vPos = vLeft;
	else if(UP_SLOPE >= fSlopeRight && DOWN_SLOPE <= fSlopeRight)
		m_vPos = vRight;
	else if(UP_SLOPE >= fSlopeFont && DOWN_SLOPE <= fSlopeFont)
		m_vPos = vFont;
	else 
		m_vPos = D3DXVECTOR3(0,0,0);

	m_vDir = vMajorPos - m_vPos ;
	D3DXVec3Normalize(&m_vDir,&m_vDir);
	m_vRot.y = DirToRot(m_vDir, m_vOrigRot).y;
	Transform();
}

void CPet::Render()
{
// 	D3DXVECTOR3 vBroundMax = m_vMax, vBroundMin = m_vMin, vCenter;
// 	vBroundMax.y=0;vBroundMin.y=0;
// 	vCenter = (vBroundMax+vBroundMin)/2;
// 	
// 	vCenter 
// 
// 	vBroundMax.y = m_pTerrain->GetTHeight(vBroundMax.x, vBroundMax.z);
// 	/*vBroundMin.x=vCenter.x;*/
// 	vBroundMin.y = m_pTerrain->GetTHeight(vBroundMin.x, vBroundMin.z);
// 	
// 	DrawLine(m_pDevice, vBroundMin, vBroundMax, 0x00ffffff);

	m_pDevice->SetRenderState(D3DRS_LIGHTING,false);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	
	m_pAI->Render();
	m_pSkinMesh->Render(&m_matWorld);//注意ai跟mesh的render顺序
}

void CPet::Update(double nFpsTime, D3DXVECTOR3 vMajorPos)
{
	m_dElapsedTime = nFpsTime;
	m_pAI->SetTimeDelta(m_dElapsedTime);
	m_fDistanceWithMajor = __GetDistance(vMajorPos, m_vPos);

	if(m_fDistanceWithMajor > m_fMaxDistanceWithMajor)
	{
		m_vDir = vMajorPos - m_vPos;
		m_vDir.y=0;
		D3DXVec3Normalize(&m_vDir,&m_vDir);

		D3DXVECTOR3 vDestPos = m_vPos + m_vDir*m_fMoveSpeed*m_dElapsedTime;
		vDestPos.y = m_pTerrain->GetTHeight(vDestPos.x, vDestPos.z);
		m_vPos = vDestPos;
		m_vRot.y = DirToRot(m_vDir, m_vOrigRot).y;//改变方向
		m_pAI->SetMoving(true);

		if(SkinMeshState::run_front != m_State){
			m_State = SkinMeshState::run_front;
			m_pAI->SetAnimName("run");
			m_pAI->ChangeTrack(true);
		}		
	}
	else
	{
		if(SkinMeshState::wait != m_State){
			m_State = SkinMeshState::wait;
			m_pAI->SetAnimName("wait");
			m_pAI->ChangeTrack(false);
		}	
	}

	Transform();
}

void CPet::Transform()
{
	CBaseObject::Transform();

	D3DXVECTOR3 vBroundMax = m_vMax, vBroundMin = m_vMin, vCenter;
	vBroundMax.y = m_pTerrain->GetTHeight(vBroundMax.x, vBroundMax.z);
	vBroundMin.y = m_pTerrain->GetTHeight(vBroundMin.x, vBroundMin.z);
	
	float fSlope = CalcSlope(vBroundMax, vBroundMin);
	
	D3DXMATRIX matWorld;
	D3DXMatrixRotationYawPitchRoll(&matWorld, fSlope, 0.0f ,0.0f);
	m_matWorld = matWorld*m_matWorld;
}