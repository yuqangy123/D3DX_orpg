#include "StdAfx.h"
#include "ZFrustum.h"

CZFrustum::CZFrustum(void)
{
}

CZFrustum::~CZFrustum(void)
{
}

//输入摄像机投影矩阵与碰撞修正值，构建平截台体
bool CZFrustum::BuildFrustum(D3DXMATRIX *pMatViewProj, float fEpsilon)
{
	/*
	float fsize=0.5f;
	m_vtx[0].x = -fsize;	m_vtx[0].y=fsize;	m_vtx[0].z=fsize;//左前上
	m_vtx[1].x = -fsize;	m_vtx[1].y=fsize;	m_vtx[1].z=0.0f;//左后上
	m_vtx[2].x = fsize;		m_vtx[2].y=fsize;	m_vtx[2].z=fsize;//右前上
	m_vtx[3].x = fsize;		m_vtx[3].y=fsize;	m_vtx[3].z=0.0f;//右后上
	m_vtx[4].x = -fsize;	m_vtx[4].y=-fsize;	m_vtx[4].z=fsize;//左前下
	m_vtx[5].x = -fsize;	m_vtx[5].y=-fsize;	m_vtx[5].z=0.0f;//左后下
	m_vtx[6].x = fsize;		m_vtx[6].y=-fsize;	m_vtx[6].z=fsize;//右前下
	m_vtx[7].x = fsize;		m_vtx[7].y=-fsize;	m_vtx[7].z=0.0f;//右后下
	*/

	float fsize=1.0f;
	m_vtx[0].x = -fsize; m_vtx[0].y = -fsize; m_vtx[0].z = 0.0f;
	m_vtx[1].x =  fsize; m_vtx[1].y = -fsize; m_vtx[1].z = 0.0f;
	m_vtx[2].x =  fsize; m_vtx[2].y = -fsize; m_vtx[2].z = fsize;
	m_vtx[3].x = -fsize; m_vtx[3].y = -fsize; m_vtx[3].z = fsize;
	m_vtx[4].x = -fsize; m_vtx[4].y =  fsize; m_vtx[4].z = 0.0f;
	m_vtx[5].x =  fsize; m_vtx[5].y =  fsize; m_vtx[5].z = 0.0f;
	m_vtx[6].x =  fsize; m_vtx[6].y =  fsize; m_vtx[6].z = fsize;
	m_vtx[7].x = -fsize; m_vtx[7].y =  fsize; m_vtx[7].z = fsize;

	D3DXMATRIX MatViewInverse;
	D3DXMatrixInverse(&MatViewInverse, NULL, pMatViewProj);//求投影矩阵的逆矩阵

	for (int n=0; n<8; ++n)
		D3DXVec3TransformCoord(&m_vtx[n], &m_vtx[n], &MatViewInverse);//点与矩阵相乘，得到点的投影变换
	
	//计算获得台体的六个面
	// top
	D3DXPlaneFromPoints(&m_plane[0],m_vtx + 4,m_vtx + 7,m_vtx + 6);
	// bottom
	D3DXPlaneFromPoints(&m_plane[1],m_vtx,m_vtx + 1,m_vtx + 2);
	// near
	D3DXPlaneFromPoints(&m_plane[2],m_vtx,m_vtx + 4,m_vtx + 5);
	// far
	D3DXPlaneFromPoints(&m_plane[3],m_vtx + 2,m_vtx + 6,m_vtx + 7);
	// left
	D3DXPlaneFromPoints(&m_plane[4],m_vtx,m_vtx + 3,m_vtx + 7);
	// right
	D3DXPlaneFromPoints(&m_plane[5],m_vtx + 1,m_vtx + 5,m_vtx + 6);

	m_fEpsilon = fEpsilon;

	m_pos = (m_vtx[0]+m_vtx[5])/2.0f;
	return true;
}

bool CZFrustum::IsIn(D3DXVECTOR3 *pv)
{
	for (int n=0; n<6; ++n)
	{
		if(D3DXPlaneDotCoord(&m_plane[n], pv)>m_fEpsilon)//点与面的关系公式, 判断碰撞修正值
		return false;
	}
	return true;
}

void CZFrustum::Render(IDirect3DDevice9* pDevice)
{
	HRESULT hr = 0;
	int nTris=2*6;
	hr = pDevice->CreateVertexBuffer(
		nTris*3 * sizeof(DEBUGVERTEX),//6个面，每个面2个三角形，每个三角形3个顶点
		0,DEBUG_FVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0);
	if (FAILED(hr))
		return ;

	DEBUGVERTEX *pV, *pTmp;
	if (FAILED(m_pVB->Lock(0,nTris*3 * sizeof(DEBUGVERTEX),(void**)&pV,0)))
		return ;
	

 	pTmp=pV;
	pTmp->position=m_vtx[3];pTmp++->color=0xffff0000;
	pTmp->position=m_vtx[2];pTmp++->color=0xffff0000;
	pTmp->position=m_vtx[7];pTmp++->color=0xffff0000;
	pTmp->position=m_vtx[2];pTmp++->color=0xffff0000;
	pTmp->position=m_vtx[7];pTmp++->color=0xffff0000;
	pTmp->position=m_vtx[6];pTmp++->color=0xffff0000;//前面

	pTmp->position=m_vtx[0];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[1];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[4];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[1];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[4];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[5];pTmp++->color=0xff00ff00;//后面

	pTmp->position=m_vtx[4];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[7];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[5];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[7];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[5];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[6];pTmp++->color=0xffffff00;//上面

	pTmp->position=m_vtx[3];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[0];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[2];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[0];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[2];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[1];pTmp++->color=0xffffff00;//下面

	pTmp->position=m_vtx[3];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[0];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[4];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[3];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[4];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[7];pTmp++->color=0xffffff00;//左面

	pTmp->position=m_vtx[1];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[2];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[5];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[2];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[5];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[6];pTmp++->color=0xff00ff00;//右面

	m_pVB->Unlock();

	pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pDevice->SetFVF(DEBUG_FVF);
	pDevice->SetStreamSource(0, m_pVB, 0, sizeof(DEBUGVERTEX));

	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, nTris);

}