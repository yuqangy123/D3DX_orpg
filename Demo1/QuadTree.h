/*
*	�Ĳ����࣬�ָ���Σ��ü���LOD�����ڵ㣬���ɵ���������
*/
#pragma once
#include "common1.h"
#include "ZFrustum.h"

class ZQuadTree
{
public:
	//enum CornerType{CORNER_TL,CORNER_TR,CORNER_BL,CORNER_BR};    // �Ĳ��������4���ǵĳ���ֵ
	//enum {EDGE_UP,EDGE_DN,EDGE_LT,EDGE_RT};                      // �ڽ��ڵ㴦���õĳ���ֵ
	enum QuadLocation                            // �Ĳ�����ƽ��̨��Ĺ�ϵ
	{
		FRUSTUM_OUT = 0,                        // ��ȫ����ƽ��̨��
		FRUSTUM_PARTIALLY_IN = 1,               // ���ְ�����ƽ��̨��
		FRUSTUM_COMPLETELY_IN = 2,              // ��ȫ������ƽ��̨��
		FRUSTUM_UNKNOWN = -1
	};

private:
	typedef struct node
	{
		int		ID;                           // �ڵ�ID
		int		ParentID;                     // ���ڵ�ID
		int		ChildID[4];                   // �ӽڵ�ID
		int		NeighborID[4];                // �ڽ��ڵ�ID
		int		iCenter;                      // �ڵ�����ֵ
		int		iCorner[4];                   // �ڵ��Ľ�ֵ
		bool	bCulled;                      // �ڵ��޳���־
		float	fRadius;                      // �߽�����뾶
	}TreeNode;
	TreeNode*	TreeList;                     // ���ڵ��б�˳�򴢴淽ʽ��
	int			MaxNodeNum;                   // �ڵ�����
	static int	TotalTreeID;                  // ��¼������ڵ�ID

	//ZQuadTree* m_pChild[4];                      // �Ĳ�����4���ӽڵ�
	//ZQuadTree* m_pParent;                        // ��ֹ����(Popping)�Ĳ���
	//ZQuadTree* m_pNeighbor[4];                   // ��ֹ����(Popping)�Ĳ���
	//int m_iCenter;                               // �Ĳ�������ĵ�һ��ֵ
	//int m_iCorner[4];                            // �Ĳ�������ĵڶ���ֵ
                                                 //   TopLeft(TL)       TopRight(TR)
	                                             //             0-------1
                                                 //             |       |
	                                             //             |       |
	                                             //             2-------3
	                                             // BottomLeft(BL)      BottomRight(BR)

	//bool m_bCulled;                              // ƽ��̨���޳��Ľڵ�
	//float m_fRadius;                             // �����ڵ�ı߽�����(bounding sphere)�İ뾶

private:
	// ��ʼ���ڵ�
	int CreateNode(int ParentID,int nCornerTL,int nCornerTR,int nCornerBL,int nCornerBR);
	// ʹ��4���²���Ĳ��������ٷָ�(subdivide)
	bool _SubDivide(int ID);
	// �����������ε�����
	int _GenTriIndex(int ID,int nTris,void* pIndex,LandVertex* pHeightMap,CZFrustum* pFrustum,float fLodRatio);
	// ���������ľ���
	float _GetDistance(D3DXVECTOR3 pv1,D3DXVECTOR3 pv2)
	{
		return D3DXVec3Length(&(pv2 - pv1));
	}
	// ��������뵱ǰ�ڵ�ľ���Ϊ��׼���LODֵ
	int _GetLODLevel(int ID,LandVertex* pHeightMap,D3DXVECTOR3* pCamera,float fLODRatio)
	{
		float d = _GetDistance(*(D3DXVECTOR3*)(pHeightMap + TreeList[ID].iCenter),*pCamera);
		return max((int)(d * fLODRatio),1);
	}
	// ��ǰ�ڵ������ƽ��̨��?
	int _IsInFrustum(int ID,LandVertex* pHeightMap,CZFrustum* pFrustum);
	// ����_IsInFrustum()�����Ľ����ƽ��̨������޳�
	void _FrustumCull(int ID,LandVertex* pHeightMap,CZFrustum* pFrustum);
	// �����Ĳ���(��Build()������)
	bool _BuildQuadTree(LandVertex* pHeightMap,int ID);
	// �����Ĳ���,��ѯ���ĸ���ֵһ�µĽڵ�
	int _FindNode(int _0,int _1,int _2,int _3);
	// ���ø��ڵ���ڽ��ڵ�
	void _BuildNeighborNode();
	// ����ÿ���ڵ�
	void _Reset();

	// �ж��Ƿ�Ϊ2��N�η�
	//bool IsTowPow(int value);
	// �������Ľڵ�����
	//int CalcNodeNum(int max,int min);
	// ����ӽڵ�
	//ZQuadTree* _AddChild(int nCornerTL,int nCornerTR,int nCornerBL,int nCornerBR);
	// ����4����ֵ
	//bool _SetCorners(int nCornerTL,int nCornerTR,int nCornerBL,int nCornerBR);
	// �ڴ洢����ɾ���Ĳ���
	//void _Destroy();

public:
	int GetMaxNodeNum(){return MaxNodeNum;};
	// ���ѭ���ڵ㹹�캯��
	ZQuadTree(int cx,int cy);
	// �²��ӽڵ㹹�캯��
	//ZQuadTree(ZQuadTree* pParent);
	// ��������
	~ZQuadTree(void);

	// ��LOD�ȼ�����,��ǰ�ڵ��ǿ�������Ľڵ���?
	bool IsVisible(int ID,LandVertex* pHeightMap,D3DXVECTOR3* pCamera,float fLODRatio)
	{
		return ((TreeList[ID].iCorner[1] - TreeList[ID].iCorner[0]) <= _GetLODLevel(ID,pHeightMap,pCamera,fLODRatio));
	}
	// �����Ĳ���
	HRESULT Build(LandVertex* pHeightMap);
	// ��������������,������������εĸ���
	int GenerateIndex(void* pIndex,LandVertex* pHeightMap,CZFrustum* pFrustum,float fLODRatio);

	// �õ��ǵ�����ֵ
	//void GetCorner(int& _0,int& _1,int& _2,int& _3)
	//{
	//	_0 = m_iCorner[0];
	//	_1 = m_iCorner[1];
	//	_2 = m_iCorner[2];
	//	_3 = m_iCorner[3];
	//}
};

