#pragma once
#include "common1.h"
#include "ZFrustum.h"

enum{LT=0, RT, LB, RB};//���ϣ����ϣ����£�����
enum{Top=0,Bottom,Left,Right};
struct QuadNode
{
	int		nodeID;					
	int		ParentID;
	int		ChildsID[4];		
	int		NeighboursID[4];	
	int		nCorner[4];//�����ĸ��ǵ�ֵ
	int		nCenter;//��������ĵ�ֵ
	bool	bCulled;//�޳���ʾ��
	float	nRadius;//����Բ�뾶
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
	bool BuildQuadTree(LandVertex *pMap, int nID);//��nID������
	bool BuildSubTree(int nID);
	int CreateNode(int nParentID, int &nLT, int &nRT, int &nLB, int &nRB);
	bool BuildNeighboursID();//�����ڽӵ�
	int FindNode(const int &nLT, const int &nRT, const int &nLB, const int &nRB);
	inline void Reset();//���ýڵ��޳���־ΪTRUE
	void FrustumCull(int nID, LandVertex *pVB, CZFrustum *pFrustum);//�޳�����ƽ��̨��Ľڵ�
	bool IsInFrustum(int nID, LandVertex *pVB, CZFrustum *pFrustum);
	int GenFrutumIndex(int nID, int nTris, DWORD* pIB, LandVertex *pVB, CZFrustum *pFrustum, float fLodRatio);
	bool Visible(int nID, LandVertex *pVB, CZFrustum *pFrustum, float fLodRatio);

	// ��������뵱ǰ�ڵ�ľ���Ϊ��׼���LODֵ
	int _GetLODLevel(int ID,LandVertex* pHeightMap,D3DXVECTOR3* pCamera,float fLODRatio)
	{
		float d = __GetDistance(*(D3DXVECTOR3*)(pHeightMap + m_pDataList[ID].nCenter),*pCamera);
		return max((int)(d * fLODRatio),1);
	}

private:
	QuadNode		*m_pTreeHead;//�����ڵ�
	QuadNode		*m_pDataList;
	int		m_nMaxNodeNum;//���ڵ�����
	int	m_nCellSize;//���ӳߴ�
	int		m_nWidth;//��ȸ�������
	int		m_nDepth;//���ȸ�������
	int		m_TotalTreeID;
};
