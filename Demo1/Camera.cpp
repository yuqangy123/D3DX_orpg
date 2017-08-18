#include "StdAfx.h"
#include "Camera.h"

CCamera::CCamera(IDirect3DDevice9* p):m_pDevice(p)
{
	m_bTest=false;
}

CCamera::~CCamera(void)
{
}

void CCamera::AdjustCamera()
{
	D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLookat, &m_vUp);
	m_pDevice->SetTransform( D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH( &m_matProj, m_fovy, 1.0f, 1.0f, m_far);

	m_pDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );
}


void CCamera::Update()
{
	if(m_pInput->KeyDown(DIK_O) )
	{
		/*
		m_matProjTest = m_matProj;
		m_matProjTest = m_matProj;
		m_bTest=true;
		*/
			m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	if( m_pInput->KeyDown(DIK_P) )
	{
			m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	if(m_pInput->GetZ()>0)
	{
		m_R = ((m_R + m_MoveSpeed >= m_MaxR) ? m_MaxR : m_R+m_MoveSpeed);
	}
	if(m_pInput->GetZ()<0)
	{
		m_R = ((m_R - m_MoveSpeed <= m_MinR) ? m_MinR : m_R-m_MoveSpeed);
	}

	LONG lX=0, lY=0;
	if(m_pInput->MouseHold(1))
	{
		lX=m_pInput->GetX();
		lY=m_pInput->GetY();
	}

	AdjustAngle((float)lX/100.0f, (float)lY/100.0f);
}

void CCamera::AdjustAngle(float fX, float fZ)
{
	m_fPitch+=fZ;
	m_fYaw+=fX;

	if(m_fPitch <= D3DX_PI/100.0f)
		m_fPitch=D3DX_PI/100.0f;
	else if(m_fPitch >= D3DX_PI/2.3)
		m_fPitch = D3DX_PI/2.3f;

	if(m_fYaw >= D3DX_PI*2.0f)
		m_fYaw=D3DX_PI/89.0f;
	else if(m_fYaw <= 0.0f)
		m_fYaw=D3DX_PI*2.0f;

	
// 	m_vEye.x=m_vLookat.x+m_R*sin(m_fPitch)*cos(m_fYaw);
// 	m_vEye.z=m_vLookat.z+m_R*sin(m_fPitch)*sin(m_fYaw);
// 	m_vEye.y=m_vLookat.y+m_R*cos(m_fPitch);
// 	

	m_vEye.x = m_vLookat.x + m_R*cosf(m_fPitch)*cosf(m_fYaw);
	m_vEye.y =m_vLookat.y + m_R*sinf(m_fPitch);
	m_vEye.z = m_vLookat.z + m_R*cosf(m_fPitch)*sinf(m_fYaw);
}

HRESULT CCamera::InitCamera(CScene *pScene, CInput *pInput, int nWidth, int nHeight)
{
	
	//原始坐标-》世界坐标-》视图坐标-》光照，深度检测，裁剪-》投影转换-》视口变换-》光栅化
	// For our world matrix, we will just leave it as the identity
	m_pInput=pInput;

	CResFileManager::Instance()->GetCameraAttribute(m_fPitch, m_fYaw, m_fovy,
		m_far, m_R, m_MaxR, m_MinR, m_MoveSpeed, 
		m_vUp, m_vLookat);

	AdjustAngle(0, 0);
	
	D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLookat, &m_vUp);
	m_pDevice->SetTransform( D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH( &m_matProj, m_fovy, m_fAspect, 1.0f, m_far);
	m_pDevice->SetTransform(D3DTS_PROJECTION,&m_matProj);

	m_pScene = pScene;
	return S_OK;
}

void CCamera::GetViewAndProjMat(D3DXMATRIX &matView, D3DXMATRIX &matProj)
{
	if(m_bTest)
	{
		matView = m_matViewTest;
		matProj = m_matProjTest;
	}
	else
	{
		matView = m_matView;
		matProj = m_matProj;
	}
};

void CCamera::SetCameraDistance(const float &fDist)
{
	if(0 > fDist){
		float fDefDist = 5.0f;
		m_vEye.x = m_vLookat.x + fDefDist*cosf(m_fPitch)*cosf(m_fYaw);
		m_vEye.y = m_vLookat.y + fDefDist*sinf(m_fPitch);
		m_vEye.z = m_vLookat.z + fDefDist*cosf(m_fPitch)*sinf(m_fYaw);
	}
	else{
		m_vEye.x = m_vLookat.x + fDist*cosf(m_fPitch)*cosf(m_fYaw);
		m_vEye.y = m_vLookat.y + fDist*sinf(m_fPitch);
		m_vEye.z = m_vLookat.z + fDist*cosf(m_fPitch)*sinf(m_fYaw);
	}
}