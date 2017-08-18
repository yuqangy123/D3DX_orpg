#pragma once
#include "common1.h"
/*************
平截台体类
*************/

class CZFrustum
{
public:
	CZFrustum(void);
	~CZFrustum(void);
	bool BuildFrustum(D3DXMATRIX *pMatViewProj, float fEpsilon);//输入摄像机投影矩阵，构建平截台体
	bool IsIn(D3DXVECTOR3 *pv);
	D3DXVECTOR3 GetPos(){return m_pos;}
	void Render(IDirect3DDevice9* pDevice);

private:
	D3DXVECTOR3 m_vtx[8];             // 平截台体的8个顶点
	D3DXPLANE m_plane[6];             // 平截台体6个面
	float m_fEpsilon;                 // 碰撞判断修正值,使碰撞边界稍稍增大
	D3DXVECTOR3		m_pos;

	IDirect3DVertexBuffer9* m_pVB;
};


// float fsize=0.5f;
// m_vtx[0].x = -fsize;	m_vtx[0].y=fsize;	m_vtx[0].z=fsize;//左前上
// m_vtx[1].x = -fsize;	m_vtx[1].y=fsize;	m_vtx[1].z=0.0f;//左后上
// m_vtx[2].x = fsize;		m_vtx[2].y=fsize;	m_vtx[2].z=fsize;//右前上
// m_vtx[3].x = fsize;		m_vtx[3].y=fsize;	m_vtx[3].z=0.0f;//右后上
// m_vtx[4].x = -fsize;	m_vtx[4].y=-fsize;	m_vtx[4].z=fsize;//左前下
// m_vtx[5].x = -fsize;	m_vtx[5].y=-fsize;	m_vtx[5].z=0.0f;//左后下
// m_vtx[6].x = fsize;		m_vtx[6].y=-fsize;	m_vtx[6].z=fsize;//右前下
// m_vtx[7].x = fsize;		m_vtx[7].y=-fsize;	m_vtx[7].z=0.0f;//右后下
