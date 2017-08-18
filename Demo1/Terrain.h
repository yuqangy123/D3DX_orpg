#pragma once
#include "common1.h"
#include <fstream>
#include "QuadTree.h"

class CTerrain
{
public:
	CTerrain(IDirect3DDevice9* pDevice);
	~CTerrain(void);
	HRESULT Init(CZFrustum *pFrustum);
	void Render();
	void Update();
	inline int GetCellSpace(){return m_nCellSpace;};
	inline int GetRenderTriNum(){return m_nTriangleNum;};
	float GetTHeight(float x, float z);
	bool IntersectTerrain(D3DXVECTOR3 &vRayOrigin, D3DXVECTOR3 &vRayDir, float *fDist, D3DXVECTOR3 *vRet);
	D3DXVECTOR3 CalcMouseClickVtr();

protected:
	HRESULT ComputeIndex();
	HRESULT ComputeVertex();
	HRESULT LoadHeightMap();
	HRESULT LoadTexture();

private:
	IDirect3DDevice9*		m_pDevice;
	LPDIRECT3DTEXTURE9		m_pTexture;
	int		m_nCol;//�� ��������
	int		m_nRow;//�� ��������
	int		m_nVCol;//�� ��������
	int		m_nVRow;//�� ��������
	int		m_nCellSpace;//����size
	float		m_nHeightScale;//�߶ȱ�
	int		m_nWidth;//���
	int		m_nDepth;//����
	int		m_nVertexNum;//�ڵ�����
	int		m_nTriangleNum;//triangle����
	char	*	m_pHeightmap;//�߶�ͼ��Ϣ
	float	m_fLods;
	IDirect3DVertexBuffer9*			m_pVB;
	IDirect3DIndexBuffer9*			m_pIB;
	ZQuadTree		*m_pQuadTree;
	CZFrustum			*m_pFrustum;
};
