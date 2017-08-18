#include "StdAfx.h"
#include "ZFrustum.h"

CZFrustum::CZFrustum(void)
{
}

CZFrustum::~CZFrustum(void)
{
}

//���������ͶӰ��������ײ����ֵ������ƽ��̨��
bool CZFrustum::BuildFrustum(D3DXMATRIX *pMatViewProj, float fEpsilon)
{
	m_fEpsilon = fEpsilon;
	int i;
	D3DXMATRIX matInv;

	m_vtx[0].x = -1.0f; m_vtx[0].y = -1.0f; m_vtx[0].z = 0.0f;
	m_vtx[1].x =  1.0f; m_vtx[1].y = -1.0f; m_vtx[1].z = 0.0f;
	m_vtx[2].x =  1.0f; m_vtx[2].y = -1.0f; m_vtx[2].z = 1.0f;
	m_vtx[3].x = -1.0f; m_vtx[3].y = -1.0f; m_vtx[3].z = 1.0f;
	m_vtx[4].x = -1.0f; m_vtx[4].y =  1.0f; m_vtx[4].z = 0.0f;
	m_vtx[5].x =  1.0f; m_vtx[5].y =  1.0f; m_vtx[5].z = 0.0f;
	m_vtx[6].x =  1.0f; m_vtx[6].y =  1.0f; m_vtx[6].z = 1.0f;
	m_vtx[7].x = -1.0f; m_vtx[7].y =  1.0f; m_vtx[7].z = 1.0f;

	// ���view*proj�������
	D3DXMatrixInverse(&matInv,NULL,pMatViewProj);
	// ��8������ת��������ռ�
	for (i = 0;i < 8;i++)
	{
		D3DXVec3TransformCoord(&m_vtx[i],&m_vtx[i],&matInv);//���������ˣ�wΪ1
		//D3DXVECTOR3 v = m_vtx[i]; //��
	}
	// ���������Ĵ�������(���ý�������,��׼ȷ�����λ��)
	m_pos = (m_vtx[0] + m_vtx[5]) / 2.0f;

	// ͨ��8���������������,����ƽ��̨��ƽ��,���߷�������
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

	return true;
}

bool CZFrustum::IsIn(D3DXVECTOR3 *pv)
{
	for (int n=3; n<6; ++n)
	{
		if(D3DXPlaneDotCoord(&m_plane[n], pv)>m_fEpsilon)//������Ĺ�ϵ��ʽ, �ж���ײ����ֵ
		return false;
	}
	return true;
}

bool CZFrustum::IsInSphere(D3DXVECTOR3* pv,float radius)
{
	for(int i=3;i<6;i++)
	{
		if (D3DXPlaneDotCoord(&m_plane[i],pv) > radius + m_fEpsilon)
			return false;
	}

	return true;
}

void CZFrustum::Render(IDirect3DDevice9* pDevice)
{
	HRESULT hr = 0;
	int nTris=2*6;
	hr = pDevice->CreateVertexBuffer(
		nTris*3 * sizeof(DEBUGVERTEX),//6���棬ÿ����2�������Σ�ÿ��������3������
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
	pTmp->position=m_vtx[6];pTmp++->color=0xffff0000;//ǰ��

	pTmp->position=m_vtx[0];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[1];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[4];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[1];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[4];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[5];pTmp++->color=0xff00ff00;//����

	pTmp->position=m_vtx[4];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[7];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[5];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[7];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[5];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[6];pTmp++->color=0xffffff00;//����

	pTmp->position=m_vtx[3];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[0];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[2];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[0];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[2];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[1];pTmp++->color=0xffffff00;//����

	pTmp->position=m_vtx[3];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[0];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[4];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[3];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[4];pTmp++->color=0xffffff00;
	pTmp->position=m_vtx[7];pTmp++->color=0xffffff00;//����

	pTmp->position=m_vtx[1];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[2];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[5];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[2];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[5];pTmp++->color=0xff00ff00;
	pTmp->position=m_vtx[6];pTmp++->color=0xff00ff00;//����

	m_pVB->Unlock();

	pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pDevice->SetFVF(DEBUG_FVF);
	pDevice->SetStreamSource(0, m_pVB, 0, sizeof(DEBUGVERTEX));

	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, nTris);

}