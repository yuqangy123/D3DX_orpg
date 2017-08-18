#pragma once
#include "common1.h"
#include "ZFrustum.h"

enum{LT=0, RT, LB, RB};//左上，右上，左下，右下
enum{Top=0,Bottom,Left,Right};
struct QuadNode
{
	int		nodeID;					
	int		ParentID;
	int		ChildsID[4];		
	int		NeighboursID[4];	
	int		nCorner[4];//方格四个角的值
	int		nCenter;//方格的中心的值
	bool	bCulled;//剔除标示符
	float	nRadius;//中心圆半径
};
class CQuadTree
{
public:
	CQuadTree(/*int ParentID,int nCornerTL,int nCornerTR,int nCornerBL,int nCornerBR*/int nx, int ny);
	~CQuadTree(void);
	bool Create(LandVertex *pMap);
	int GenerateIndex(int nID, DWORD* pIB, LandVertex *pVB, CZFrustum *pFrustum, float fLodRatio);
	inline int GetMaxNodeNum(){return m_nMaxNodeNum;};

protected:
	int CalcCenter(int &LT, int &RT, int &LB, int &RB);
	bool BuildQuadTree(LandVertex *pMap, int nID);//从nID创建树
	bool BuildSubTree(int nID);
	int CreateNode(int nParentID, int &nLT, int &nRT, int &nLB, int &nRB);
	bool BuildNeighboursID();//生成邻接点
	int FindNode(const int &nLT, const int &nRT, const int &nLB, const int &nRB);
	inline void Reset();//重置节点剔除标志为TRUE
	void FrustumCull(int nID, LandVertex *pVB, CZFrustum *pFrustum);//剔除不在平截台体的节点
	bool IsInFrustum(int nID, LandVertex *pVB, CZFrustum *pFrustum);
	int GenFrutumIndex(int nID, int nTris, DWORD* pIB, LandVertex *pVB, CZFrustum *pFrustum, float fLodRatio);
	bool Visible(int nID, LandVertex *pVB, CZFrustum *pFrustum, float fLodRatio);

	// 以摄像机与当前节点的距离为标准求得LOD值
	int _GetLODLevel(int ID,LandVertex* pHeightMap,D3DXVECTOR3* pCamera,float fLODRatio)
	{
		float d = __GetDistance(*(D3DXVECTOR3*)(pHeightMap + m_pDataList[ID].nCenter),*pCamera);
		return max((int)(d * fLODRatio),1);
	}

private:
	QuadNode		*m_pTreeHead;//树主节点
	QuadNode		*m_pDataList;
	int		m_nMaxNodeNum;//最大节点数量
	int	m_nCellSize;//格子尺寸
	int		m_nWidth;//宽度格子数量
	int		m_nDepth;//长度格子数量
	int		m_TotalTreeID;
};
