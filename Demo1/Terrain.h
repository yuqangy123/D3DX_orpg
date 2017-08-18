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
	int		m_nCol;//列 格子数量
	int		m_nRow;//行 格子数量
	int		m_nVCol;//列 顶点数量
	int		m_nVRow;//行 顶点数量
	int		m_nCellSpace;//格子size
	float		m_nHeightScale;//高度比
	int		m_nWidth;//宽度
	int		m_nDepth;//长度
	int		m_nVertexNum;//节点数量
	int		m_nTriangleNum;//triangle数量
	char	*	m_pHeightmap;//高度图信息
	float	m_fLods;
	IDirect3DVertexBuffer9*			m_pVB;
	IDirect3DIndexBuffer9*			m_pIB;
	ZQuadTree		*m_pQuadTree;
	CZFrustum			*m_pFrustum;
};
