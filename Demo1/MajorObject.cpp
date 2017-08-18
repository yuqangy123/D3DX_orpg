#include "StdAfx.h"
#include "MajorObject.h"
 #include "ResFileManager.h"
#include "DXDevice.h"
#include "Deck.h"

CMajorObject::CMajorObject()
{
	m_pDevice = CDXDevice::Instance()->GetD3Device();
	m_pInput = CDXDevice::Instance()->GetD3DInput();
	m_pTerrain = CDXDevice::Instance()->GetScene()->GetCurTerrain();//注意切换地形的时候，该指针也要一起更新
	m_pCamera = CDXDevice::Instance()->GetCamera();

	m_pSkinMesh = NULL;
	m_pPet=NULL;
	m_pAI=NULL;
}

CMajorObject::~CMajorObject(void)
{
	DeleteAndNull(m_pSkinMesh);
	DeleteAndNull(m_pPet);
	DeleteAndNull(m_pAI);
}

HRESULT CMajorObject::Init()
{
	m_pSkinMesh = new CSkinMesh(m_pDevice);
	m_pAI = new CAnimInstance;
	m_pPet = new CPet;
	if(!m_pSkinMesh || !m_pAI || !m_pPet)
		return E_FAIL;

	if(FAILED(m_pSkinMesh->LoadMesh(CResFileManager::Instance()->GetMajorSkinMeshFile().c_str(), 
		CResFileManager::Instance()->GetMajorTextureFile().c_str())))
		return E_FAIL;
	CResFileManager::Instance()->GetMajorAttribute(
		m_vPos, m_vRot, m_vScal, m_dElapsedTime, m_fRunMoveSpeed, m_fWalkMoveSpeed);
	m_vPos.y = m_pTerrain->GetTHeight(m_vPos.x, m_vPos.y);

	m_pAI->Init(m_pSkinMesh);
	m_pAI->SetTimeDelta(m_dElapsedTime);//设置每帧间隔时间
	m_pSkinMesh->GetBoundingB_S(&m_vOrigMax, &m_vOrigMin, &m_vCenter, &m_fRaius);//m_dElapsedTime0.1
	
	D3DXVECTOR3 vPos, vLookat;
	m_pCamera->GetPosVtr(vPos);
	m_pCamera->GetLookAtVtr(vLookat);
	m_vDir = vPos-vLookat;
	m_vDir.y=0;
	D3DXVec3Normalize(&m_vDir,&m_vDir);
	m_vOrigRot=m_vRot;
	m_UP = no_OP;

	if(FAILED(m_pPet->Init()))
		return E_FAIL;
	m_pPet->InitPos(m_vPos, m_vDir);

	Transform();	
	return S_OK;
}

void CMajorObject::Update(double nFpsTime, D3DXVECTOR3 &vDir)
{
	m_dElapsedTime = nFpsTime;
	m_pAI->SetTimeDelta(nFpsTime);//设置动画推移时间
	if (m_pInput->KeyDown(DIK_W))
	{
		if(down_W != m_UP)
		{
			m_vDir = vDir;
			m_vDir.y=0;
			D3DXVec3Normalize(&m_vDir,&m_vDir);
			m_vRot.y = DirToRot(m_vDir, m_vOrigRot).y;

			m_MajorState = SkinMeshState::run_front;
			m_UP = down_W;
		}
	}
	else if (m_pInput->KeyDown(DIK_S))
	{
		if(down_S != m_UP)
		{
			m_vDir = vDir;
			m_vDir.y=0;
			m_vDir.x=-m_vDir.x;
			m_vDir.z=-m_vDir.z;
			D3DXVec3Normalize(&m_vDir,&m_vDir);
			m_vRot.y = DirToRot(m_vDir, m_vOrigRot).y;

			m_MajorState = SkinMeshState::run_back;
			m_UP = down_S;
		}
	}
	else if (m_pInput->KeyDown(DIK_A))
	{
		if(down_A != m_UP)
		{
			D3DXVECTOR3 vY(0,1,0);
			m_vDir = vDir;
			m_vDir.y=0;
			D3DXVec3Cross(&m_vDir, &m_vDir, &vY);//求叉积
			D3DXVec3Normalize(&m_vDir,&m_vDir);
			m_vRot.y = DirToRot(m_vDir, m_vOrigRot).y;

			m_MajorState = SkinMeshState::run_left;
			m_UP = down_A;
		}
	}
	else if (m_pInput->KeyDown(DIK_D))
	{
		if(down_D != m_UP)
		{
			D3DXVECTOR3 vY(0,1,0);
			m_vDir = vDir;
			m_vDir.y=0;
			D3DXVec3Cross(&m_vDir, &vY, &m_vDir);//同上
			D3DXVec3Normalize(&m_vDir,&m_vDir);
			m_vRot.y = DirToRot(m_vDir, m_vOrigRot).y;

			m_MajorState = SkinMeshState::run_right;
			m_UP = down_D;
		}
	}
	else if(m_pInput->KeyHold(DIK_W))
	{
		if(hold_W != m_UP)
		{
			m_vDir = vDir;
			m_vDir.y=0;
			D3DXVec3Normalize(&m_vDir,&m_vDir);
			m_vRot.y = DirToRot(m_vDir, m_vOrigRot).y;

			m_MajorState = SkinMeshState::run_front;
			m_UP = hold_W;
		}
	}
	else if(m_pInput->KeyHold(DIK_S))
	{
		if(hold_S != m_UP)
		{
			m_vDir = vDir;
			m_vDir.y=0;
			m_vDir.x=-m_vDir.x;
			m_vDir.z=-m_vDir.z;
			D3DXVec3Normalize(&m_vDir,&m_vDir);
			m_vRot.y = DirToRot(m_vDir, m_vOrigRot).y;

			m_MajorState = SkinMeshState::run_back;
			m_UP = hold_S;
		}
	}
	else if(m_pInput->KeyHold(DIK_A))
	{
		if(hold_A!= m_UP)
		{
			D3DXVECTOR3 vY(0,1,0);
			m_vDir = vDir;
			m_vDir.y=0;
			D3DXVec3Cross(&m_vDir, &m_vDir, &vY);//求叉积
			D3DXVec3Normalize(&m_vDir,&m_vDir);
			m_vRot.y = DirToRot(m_vDir, m_vOrigRot).y;

			m_MajorState = SkinMeshState::run_left;
			m_UP = hold_A;
		}
	}
	else if(m_pInput->KeyHold(DIK_D))
	{
		if(hold_D!= m_UP)
		{
			D3DXVECTOR3 vY(0,1,0);
			m_vDir = vDir;
			m_vDir.y=0;
			D3DXVec3Cross(&m_vDir, &vY, &m_vDir);//同上
			D3DXVec3Normalize(&m_vDir,&m_vDir);
			m_vRot.y = DirToRot(m_vDir, m_vOrigRot).y;

			m_MajorState = SkinMeshState::run_right;
			m_UP = UOperate::hold_D;
		}
	}
	else if(m_pInput->KeyUp(DIK_W) || m_pInput->KeyUp(DIK_S) || m_pInput->KeyUp(DIK_A) || m_pInput->KeyUp(DIK_D))
	{
		m_MajorState = SkinMeshState::wait;		
		m_UP = UOperate::no_OP;
	}
	else if(m_pInput->MouseDown(0))
	{
		m_vWalkPos = m_pTerrain->CalcMouseClickVtr();
		DrawLine(CDXDevice::Instance()->GetD3Device(), m_vWalkPos, m_vPos, 0xffffffff);
		m_vDir = m_vWalkPos-m_vPos;
		D3DXVec3Normalize(&m_vDir,&m_vDir);
		m_vRot.y = DirToRot(m_vDir, m_vOrigRot).y;

		m_MajorState = SkinMeshState::walk;
		m_UP = UOperate::mouse_leftClick;

	}

	OnUpdateSkinMesh();

	Transform();

	adjustCamera();

	m_pPet->Update(nFpsTime, m_vPos);
}

void CMajorObject::OnUpdateWorldPos()
{
	float fSpeed = .0f;
	if(SkinMeshState::walk == m_MajorState)
		fSpeed = m_fWalkMoveSpeed;
	else 
		fSpeed = m_fRunMoveSpeed;
	D3DXVECTOR3 vDestPos =  m_vPos + m_vDir*fSpeed*m_dElapsedTime;
	vDestPos.y=m_pTerrain->GetTHeight(vDestPos.x, vDestPos.z);
	float fSlope = CalcSlope(vDestPos, m_vPos);
	if(UP_SLOPE >=  fSlope&&
		DOWN_SLOPE	 <= fSlope&&
		!CollisionTest(vDestPos))//优化，这里没有考虑障碍物，只考虑了地面高度
	{
		m_vPos = vDestPos;
		m_pAI->SetMoving(true);
	}
}

void CMajorObject::OnUpdateSkinMesh()
{
	switch(m_MajorState)
	{
	case SkinMeshState::run_front:
	case SkinMeshState::run_back:
	case SkinMeshState::run_left:
	case SkinMeshState::run_right:{
		OnUpdateWorldPos();
		if(UOperate::hold_A == m_UP || UOperate::hold_D == m_UP || UOperate::hold_S== m_UP || UOperate::hold_W == m_UP)
			break;

		if(UOperate::down_A == m_UP || UOperate::down_D == m_UP || UOperate::down_S== m_UP || UOperate::down_W == m_UP)//这里可以优化成位移
		{
			m_pAI->SetAnimName("run");
			m_pAI->ChangeTrack(true);
		}
								  }break;

	case SkinMeshState::walk:{
		OnUpdateWorldPos();
		float fDis = __GetDistance(m_vWalkPos, m_vPos);
		if(fDis <= 1.0f)
		{
			m_MajorState = SkinMeshState::wait;
			break;
		}
		if(SkinMeshState::walk != m_OldMajorState)
		{
			m_pAI->SetAnimName("walk");
			m_pAI->ChangeTrack(true);
		}
							 }break;

	case SkinMeshState::wait:{
		m_pAI->SetAnimName("wait");
		m_pAI->ChangeTrack(false);
							 }break;
	}

	m_OldMajorState = m_MajorState;
}
void CMajorObject::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING,false);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pAI->Render();
	m_pSkinMesh->Render(&m_matWorld);//注意ai跟mesh的render顺序

	m_pPet->Render();
}

void CMajorObject::adjustCamera()
{	
	m_vMeshCenterOfWorld=m_vPos;
	m_vMeshCenterOfWorld.y += (m_vMax.y-m_vMin.y)/2.f;	
	m_pCamera->SetLookAtVtr(m_vMeshCenterOfWorld);

	D3DXVECTOR3 vCameraLookat, vRet, vCameraPos, vDir;
	float fDist(0.0), fCameraDis;
	m_pCamera->GetLookAtVtr(vCameraLookat);
	m_pCamera->GetPosVtr(vCameraPos);
	m_pCamera->GetCameraDistance(fCameraDis);
	vDir = vCameraPos - vCameraLookat;
	if(m_pTerrain->IntersectTerrain(vCameraLookat, vDir, &fDist, &vRet))
	{
		//printf("fDist=%f, Ret:%f,%f,%f\r\n", fDist, vRet.x, vRet.y, vRet.z);
		if(fCameraDis > fDist){
			m_pCamera->SetCameraDistance(fDist-5.0f);
		}
	}
}

bool CMajorObject::CollisionTest(const D3DXVECTOR3 &vDestPos)
{
	CDeck *pDeck = CDXDevice::Instance()->GetScene()->GetDeck();
	D3DXVECTOR3 vMax;
	pDeck->GetBoundMaxV(vMax);
	D3DXVECTOR3 vMin;
	pDeck->GetBoundMinV(vMin);

	if(vMax.x >= m_vMin.x && vMin.x <= m_vMax.x &&
		vMax.y >= m_vMin.y && vMin.y  <= m_vMax.y &&
		vMax.z >= m_vMin.z && vMin.z <= m_vMax.z)
	{
		BOOL bHit=FALSE;
		float fDist=0.0f;
		D3DXMATRIX  matDeckInv, matWorld;
		D3DXVECTOR3 vInvPos(vDestPos), vInvDir(m_vDir);
		D3DXMatrixInverse(&matDeckInv, NULL, &pDeck->GetWorldMat());
		D3DXVec3TransformCoord(&vInvPos, &vInvPos, &matDeckInv);//坐标转换，用于非标点，第四维w = 1投影（pV (x, y, z, 1)）
		D3DXVec3TransformNormal(&vInvDir, &vInvDir, &matDeckInv);//向量转换，用于标向量，第四维w = 0投影（pV (x, y, z, 0)）

		D3DXIntersect(pDeck->GetD3DXMeshObj(), &vInvPos, &vInvDir, &bHit, NULL, NULL, NULL, &fDist, NULL, NULL);
		if	(bHit)
			return true;
	}
	return false;

	/*
	坐标和向量是有区别的：向量的第四维数值是0，坐标的第四维数值是1
	 向量的坐标变换用D3DXVec3TransformNormal,点用D3DXVec3TransformCoord。D3DXVec3Transform和D3DXVec3TransformCoord相似，区别在于后者变换后将w投射回1
	 实际执行计算的时候是：
	 D3DXVec3TransformCoord 
	 VOut.x = V1.x * M.11 + V1.y * M.21 + V1.z * M.31 + M.41
	 VOut.y = V1.x * M.12 + V1.y * M.22 + V1.z * M.32 + M.42
	 VOut.z = V1.x * M.13 + V1.y * M.23 + V1.z * M.33 + M.43　

	 因为是向量，第四维不计算：
	 D3DXVec3TransformNormal 
	 VOut.x = V1.x * M.11 + V1.y * M.21 + V1.z * M.31 
	 VOut.y = V1.x * M.12 + V1.y * M.22 + V1.z * M.32 
	 VOut.z = V1.x * M.13 + V1.y * M.23 + V1.z * M.33
	*/
}