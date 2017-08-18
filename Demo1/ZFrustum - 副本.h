#pragma once
#include "common1.h"
/*************
ƽ��̨����
*************/

class CZFrustum
{
public:
	CZFrustum(void);
	~CZFrustum(void);
	bool BuildFrustum(D3DXMATRIX *pMatViewProj, float fEpsilon);//���������ͶӰ���󣬹���ƽ��̨��
	bool IsIn(D3DXVECTOR3 *pv);
	D3DXVECTOR3 GetPos(){return m_pos;}
	void Render(IDirect3DDevice9* pDevice);

private:
	D3DXVECTOR3 m_vtx[8];             // ƽ��̨���8������
	D3DXPLANE m_plane[6];             // ƽ��̨��6����
	float m_fEpsilon;                 // ��ײ�ж�����ֵ,ʹ��ײ�߽���������
	D3DXVECTOR3		m_pos;

	IDirect3DVertexBuffer9* m_pVB;
};


// float fsize=0.5f;
// m_vtx[0].x = -fsize;	m_vtx[0].y=fsize;	m_vtx[0].z=fsize;//��ǰ��
// m_vtx[1].x = -fsize;	m_vtx[1].y=fsize;	m_vtx[1].z=0.0f;//�����
// m_vtx[2].x = fsize;		m_vtx[2].y=fsize;	m_vtx[2].z=fsize;//��ǰ��
// m_vtx[3].x = fsize;		m_vtx[3].y=fsize;	m_vtx[3].z=0.0f;//�Һ���
// m_vtx[4].x = -fsize;	m_vtx[4].y=-fsize;	m_vtx[4].z=fsize;//��ǰ��
// m_vtx[5].x = -fsize;	m_vtx[5].y=-fsize;	m_vtx[5].z=0.0f;//�����
// m_vtx[6].x = fsize;		m_vtx[6].y=-fsize;	m_vtx[6].z=fsize;//��ǰ��
// m_vtx[7].x = fsize;		m_vtx[7].y=-fsize;	m_vtx[7].z=0.0f;//�Һ���
