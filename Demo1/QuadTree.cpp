#include "stdafx.h"
#include "QuadTree.h"

int ZQuadTree::TotalTreeID = 0;

ZQuadTree::ZQuadTree(int cx,int cy)
{
	int Width = cx - 1,Depth = 0;
	while (Width >= 1)
	{
		Width = Width >> 1;
		Depth++;
	}
	MaxNodeNum = (pow(4.0,Depth) - 1) / (4 - 1);
	TreeList = new TreeNode[MaxNodeNum];
	TreeList[0].ID = 0;
	TreeList[0].ParentID = 0;
	for (int i = 0;i < 4;i++)
	{
		TreeList[0].ChildID[i] = -1;
		TreeList[0].NeighborID[i] = -1;
	}
	TreeList[0].iCorner[0] = 0;
	TreeList[0].iCorner[1] = cx - 1;
	TreeList[0].iCorner[2] = cx * (cy - 1);
	TreeList[0].iCorner[3] = cx * cy - 1;
	TreeList[0].iCenter = (TreeList[0].iCorner[0] + TreeList[0].iCorner[1] + 
							TreeList[0].iCorner[2] + TreeList[0].iCorner[3]) >> 2;
	TreeList[0].bCulled = false;
	TreeList[0].fRadius = 0.0f;
}

//ZQuadTree::ZQuadTree(int cx,int cy)
//{
//	m_pParent = NULL;
//	for (int i = 0;i < 4;i++)
//	{
//		m_pChild[i] = NULL;
//		m_pNeighbor[i] = NULL;
//	}
//	m_iCorner[CORNER_TL] = 0;
//	m_iCorner[CORNER_TR] = cx - 1;
//	m_iCorner[CORNER_BL] = cx * (cy - 1);
//	m_iCorner[CORNER_BR] = cx * cy - 1;
//	m_iCenter = (m_iCorner[CORNER_TL] + m_iCorner[CORNER_TR] + m_iCorner[CORNER_BL] + m_iCorner[CORNER_BR]) >> 2;
//	m_bCulled = false;
//	m_fRadius = 0.0f;
//}

//ZQuadTree::ZQuadTree(ZQuadTree* pParent)
//{
//	m_pParent = pParent;
//	for (int i = 0;i < 4;i++)
//	{
//		m_pChild[i] = NULL;
//		m_pNeighbor[i] = NULL;
//		m_iCorner[i] = 0;
//	}
//	m_iCenter = 0;
//	m_bCulled = false;
//	m_fRadius = 0.0f;
//}

ZQuadTree::~ZQuadTree(void)
{
	//SafeRelease(TreeList);
}

int ZQuadTree::CreateNode(int ParentID,int nCornerTL,int nCornerTR,int nCornerBL,int nCornerBR)
{
	TotalTreeID++;
	TreeList[TotalTreeID].ID = TotalTreeID;
	TreeList[TotalTreeID].ParentID = ParentID;
	for (int i = 0;i < 4;i++)
	{
		TreeList[TotalTreeID].ChildID[i] = -1;
		TreeList[TotalTreeID].NeighborID[i] = -1;
	}
	TreeList[TotalTreeID].iCorner[0] = nCornerTL;
	TreeList[TotalTreeID].iCorner[1] = nCornerTR;
	TreeList[TotalTreeID].iCorner[2] = nCornerBL;
	TreeList[TotalTreeID].iCorner[3] = nCornerBR;
	TreeList[TotalTreeID].iCenter = (nCornerTL + nCornerTR + nCornerBL + nCornerBR) >> 2;
	TreeList[TotalTreeID].bCulled = false;
	TreeList[TotalTreeID].fRadius = 0.0f;
	return TotalTreeID;
}

// �����Ĳ���
HRESULT ZQuadTree::Build(LandVertex* pHeightMap)
{
	if (_BuildQuadTree(pHeightMap,0))
	{
		_BuildNeighborNode();
		return S_OK;
	}
	return E_FAIL;
}

// ��������������,������������εĸ���
int ZQuadTree::GenerateIndex(void* pIndex,LandVertex* pHeightMap,CZFrustum* pFrustum,float fLODRatio)
{
	_Reset();
	_FrustumCull(0,pHeightMap,pFrustum);
	return _GenTriIndex(0,0,pIndex,pHeightMap,pFrustum,fLODRatio);
}

// ʹ��4���²���Ĳ��������ٷָ�(subdivide)
bool ZQuadTree::_SubDivide(int ID)
{
	if (TreeList[ID].iCorner[1] - TreeList[ID].iCorner[0] <= 1)
		return false;
	// �����ıߵ��е�
	int nCornerTM = (TreeList[ID].iCorner[0] + TreeList[ID].iCorner[1]) >> 1;
	int nCornerLM = (TreeList[ID].iCorner[0] + TreeList[ID].iCorner[2]) >> 1;
	int nCornerRM = (TreeList[ID].iCorner[1] + TreeList[ID].iCorner[3]) >> 1;
	int nCornerBM = (TreeList[ID].iCorner[2] + TreeList[ID].iCorner[3]) >> 1;
	TreeList[ID].ChildID[0] = CreateNode(ID,TreeList[ID].iCorner[0],nCornerTM,nCornerLM,TreeList[ID].iCenter);
	TreeList[ID].ChildID[1] = CreateNode(ID,nCornerTM,TreeList[ID].iCorner[1],TreeList[ID].iCenter,nCornerRM);
	TreeList[ID].ChildID[2] = CreateNode(ID,nCornerLM,TreeList[ID].iCenter,TreeList[ID].iCorner[2],nCornerBM);
	TreeList[ID].ChildID[3] = CreateNode(ID,TreeList[ID].iCenter,nCornerRM,nCornerBM,TreeList[ID].iCorner[3]);
	return true;
}

// �����������ε�����
int ZQuadTree::_GenTriIndex(int ID,int nTris,void* pIndex,LandVertex* pHeightMap,CZFrustum* pFrustum,float fLodRatio)
{
	/*if (TreeList[ID].bCulled == false)
	{*/
		if (IsVisible(ID,pHeightMap,&pFrustum->GetPos(),fLodRatio))
		{
			DWORD* p = ((DWORD*)pIndex) + nTris * 3;
			
			// �ж��ĸ�������ڽ��ڵ�LOD�ȼ��뵱ǰ�ڵ��ϵ��ע���ǵ�ǰ��Ԫ�������ڽ�LOD�Ƚϣ���LOD��LOD�Ƚϣ�
			int iRepair[4] = {0,0,0,0};
			int NeighborLod;
			for (int i=0;i<4;i++)
			{
				if (TreeList[ID].ChildID[0] != -1 && TreeList[ID].NeighborID[i] != -1)
				{
					NeighborLod = _GetLODLevel(TreeList[ID].NeighborID[i],pHeightMap,&pFrustum->GetPos(),fLodRatio);
					if (TreeList[ID].iCorner[1] - TreeList[ID].iCorner[0] > NeighborLod)
						iRepair[i] = 1;
				}
			}
			// �ĸ�������ͬ
			if ((iRepair[0] + iRepair[1] + iRepair[2] + iRepair[3]) == 0)
			{
				// ���Ϸ�������    
				*p++ = TreeList[ID].iCorner[0];   
				*p++ = TreeList[ID].iCorner[1];   
				*p++ = TreeList[ID].iCorner[2];   
				nTris++;   
				// ���·�������    
				*p++ = TreeList[ID].iCorner[2];   
				*p++ = TreeList[ID].iCorner[1];   
				*p++ = TreeList[ID].iCorner[3];
				nTris++;   
			}
			else
			{
				// �Ϸ�������
				if (iRepair[0] == 1)
				{
					int nCornerTM = (TreeList[ID].iCorner[0] + TreeList[ID].iCorner[1]) >> 1;
					*p++ = TreeList[ID].iCorner[0];
					*p++ = nCornerTM;
					*p++ = TreeList[ID].iCenter;
					nTris++;
					*p++ = nCornerTM;
					*p++ = TreeList[ID].iCorner[1];
					*p++ = TreeList[ID].iCenter;
					nTris++;	
				}
				else
				{
					*p++ = TreeList[ID].iCorner[0];
					*p++ = TreeList[ID].iCorner[1];
					*p++ = TreeList[ID].iCenter;
					nTris++;
				}
				// �·�������
				if (iRepair[1] == 1)
				{
					int nCornerBM = (TreeList[ID].iCorner[2] + TreeList[ID].iCorner[3]) >> 1;
					*p++ = TreeList[ID].iCorner[2];
					*p++ = TreeList[ID].iCenter;
					*p++ = nCornerBM;
					nTris++;
					*p++ = nCornerBM;
					*p++ = TreeList[ID].iCenter;
					*p++ = TreeList[ID].iCorner[3];
					nTris++;
				}
				else
				{
					*p++ = TreeList[ID].iCorner[2];
					*p++ = TreeList[ID].iCenter;
					*p++ = TreeList[ID].iCorner[3];
					nTris++;
				}
				// �󷽲�����
				if (iRepair[2] == 1)
				{
					int nCornerLM = (TreeList[ID].iCorner[0] + TreeList[ID].iCorner[2]) >> 1;
					*p++ = nCornerLM;
					*p++ = TreeList[ID].iCorner[0];
					*p++ = TreeList[ID].iCenter;
					nTris++;
					*p++ = nCornerLM;
					*p++ = TreeList[ID].iCenter;
					*p++ = TreeList[ID].iCorner[2];
					nTris++;
				}
				else
				{
					*p++ = TreeList[ID].iCorner[0];
					*p++ = TreeList[ID].iCenter;
					*p++ = TreeList[ID].iCorner[2];
					nTris++;
				}
				// �ҷ�������
				if (iRepair[3] == 1)
				{
					int nCornerRM = (TreeList[ID].iCorner[1] + TreeList[ID].iCorner[3]) >> 1;
					*p++ = TreeList[ID].iCenter;
					*p++ = TreeList[ID].iCorner[1];
					*p++ = nCornerRM;
					nTris++;
					*p++ = TreeList[ID].iCenter;
					*p++ = nCornerRM;
					*p++ = TreeList[ID].iCorner[3];
					nTris++;
				}
				else
				{
					*p++ = TreeList[ID].iCorner[3];
					*p++ = TreeList[ID].iCenter;
					*p++ = TreeList[ID].iCorner[1];
					nTris++;
				}
			}
			return nTris;
		}
		if (TreeList[ID].ChildID[0] != -1 && TreeList[TreeList[ID].ChildID[0]].bCulled != true)
			nTris = _GenTriIndex(TreeList[ID].ChildID[0],nTris,pIndex,pHeightMap,pFrustum,fLodRatio);   
		if (TreeList[ID].ChildID[1] != -1 && TreeList[TreeList[ID].ChildID[1]].bCulled != true)
			nTris = _GenTriIndex(TreeList[ID].ChildID[1],nTris,pIndex,pHeightMap,pFrustum,fLodRatio);   
		if (TreeList[ID].ChildID[2] != -1 && TreeList[TreeList[ID].ChildID[2]].bCulled != true)
			nTris = _GenTriIndex(TreeList[ID].ChildID[2],nTris,pIndex,pHeightMap,pFrustum,fLodRatio);   
		if (TreeList[ID].ChildID[3] != -1 && TreeList[TreeList[ID].ChildID[3]].bCulled != true)
			nTris = _GenTriIndex(TreeList[ID].ChildID[3],nTris,pIndex,pHeightMap,pFrustum,fLodRatio);   
		return nTris;
	//}
	//else
	//	return nTris;
}

// ��ǰ�ڵ������ƽ��̨��?
int ZQuadTree::_IsInFrustum(int ID,LandVertex* pHeightMap,CZFrustum* pFrustum)
{
	bool b[4];
	bool bInSphere;
	// �ڱ߽�������
	bInSphere = pFrustum->IsInSphere((D3DXVECTOR3*)(pHeightMap + TreeList[ID].iCenter),TreeList[ID].fRadius);
	// ���ڱ߽������ڵĻ�,����ʡ�Ե㵥λ��ƽ��̨��Ĳ���.
	if (!bInSphere)
		return FRUSTUM_OUT;
	// �Ĳ�����4��߽�ƽ��̨�����
	b[0] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + TreeList[ID].iCorner[0]));
	b[1] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + TreeList[ID].iCorner[1]));
	b[2] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + TreeList[ID].iCorner[2]));
	b[3] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + TreeList[ID].iCorner[3]));
	// 4������ƽ��̨���ڲ�
	if ((b[0] + b[1] + b[2] + b[3]) == 4)
		return FRUSTUM_COMPLETELY_IN;
	// һ������ƽ��̨���ڲ�
	return FRUSTUM_PARTIALLY_IN;
}

// ����_IsInFrustum()�����Ľ����ƽ��̨������޳�
void ZQuadTree::_FrustumCull(int ID,LandVertex* pHeightMap,CZFrustum* pFrustum)
{
	int ret;
	ret = _IsInFrustum(ID,pHeightMap,pFrustum);
	switch (ret)
	{
	case FRUSTUM_COMPLETELY_IN:
		TreeList[ID].bCulled = false;
		return;
	case FRUSTUM_PARTIALLY_IN:
		TreeList[ID].bCulled = false;
		break;
	case FRUSTUM_OUT:
		TreeList[ID].bCulled = true;
		return;
	}
	if (TreeList[ID].ChildID[0] != -1)
		_FrustumCull(TreeList[ID].ChildID[0],pHeightMap,pFrustum);
	if (TreeList[ID].ChildID[1] != -1)
		_FrustumCull(TreeList[ID].ChildID[1],pHeightMap,pFrustum);
	if (TreeList[ID].ChildID[2] != -1)
		_FrustumCull(TreeList[ID].ChildID[2],pHeightMap,pFrustum);
	if (TreeList[ID].ChildID[3] != -1)
		_FrustumCull(TreeList[ID].ChildID[3],pHeightMap,pFrustum);
}

//�����Ĳ���(��Build()������)
bool ZQuadTree::_BuildQuadTree(LandVertex* pHeightMap,int ID)
{
	D3DXVECTOR3 pv1 = *(D3DXVECTOR3*)(pHeightMap + TreeList[ID].iCenter);
	D3DXVECTOR3 pv2 = *(D3DXVECTOR3*)(pHeightMap + TreeList[ID].iCorner[0]);
	TreeList[ID].fRadius = _GetDistance(pv1,pv2); 
	if (_SubDivide(ID))
	{
		for (int i = 0;i < 4;i++)
		{
			_BuildQuadTree(pHeightMap,TreeList[ID].ChildID[i]);
		}
	}
	return true;
}

// �����Ĳ���,��ѯ���ĸ���ֵһ�µĽڵ�
int ZQuadTree::_FindNode(int _0,int _1,int _2,int _3)
{
	for (int i = 0;i < MaxNodeNum;i++)
	{
		if (TreeList[i].iCorner[0] == _0 &&
			TreeList[i].iCorner[1] == _1 &&
			TreeList[i].iCorner[2] == _2 &&
			TreeList[i].iCorner[3] == _3)
			return i;
	}
	return -1;
}

// ���ø��ڵ���ڽ��ڵ�
void ZQuadTree::_BuildNeighborNode()
{
	int TL,TR,BL,BR;
	for (int i = 0;i < MaxNodeNum;i++)
	{
		// ���ڽӵ�
		TL = 2 * TreeList[i].iCorner[0] - TreeList[i].iCorner[2]; 
		TR = 2 * TreeList[i].iCorner[1] - TreeList[i].iCorner[3]; 
		BL = TreeList[i].iCorner[0];
		BR = TreeList[i].iCorner[1];
		TreeList[i].NeighborID[0] = _FindNode(TL,TR,BL,BR);
		// ���ڽӵ�
		TL = TreeList[i].iCorner[2];
		TR = TreeList[i].iCorner[3];
		BL = 2 * TreeList[i].iCorner[2] - TreeList[i].iCorner[0];
		BR = 2 * TreeList[i].iCorner[3] - TreeList[i].iCorner[1];
		TreeList[i].NeighborID[1] = _FindNode(TL,TR,BL,BR);

		// ���ڽӵ�
		TL = 2 * TreeList[i].iCorner[0] - TreeList[i].iCorner[1];
		TR = TreeList[i].iCorner[0];
		BL = 2 * TreeList[i].iCorner[2] - TreeList[i].iCorner[3];
		BR = TreeList[i].iCorner[2];
		TreeList[i].NeighborID[2] = _FindNode(TL,TR,BL,BR);
		// ���ڽӵ�
		TL = TreeList[i].iCorner[1];
		TR = 2 * TreeList[i].iCorner[1] - TreeList[i].iCorner[0];
		BL = TreeList[i].iCorner[3];
		BR = 2 * TreeList[i].iCorner[3] - TreeList[i].iCorner[2];
		TreeList[i].NeighborID[3] = _FindNode(TL,TR,BL,BR);
	}
}

// ���ýڵ�
void ZQuadTree::_Reset()
{
	for (int i = 0;i < MaxNodeNum;i++)
	{
		TreeList[i].bCulled = false;
	}
}

// �ж��Ƿ�Ϊ����N�η�
//bool ZQuadTree::IsTowPow(int value)
//{
//	int i,j;
//	i = (value - 1) | value;
//	j = (value << 1) - 1;
//	if (i == j)
//		return true;
//	return false;
//}


//int ZQuadTree::CalcNodeNum(int max,int min)
//{
//	int ctr = 0;
//	int var = 0;
//	while (var == 0)
//	{
//		ctr += max;
//		max = max / min;
//		if (max == 1)
//			var = 1;
//	}
//	ctr++;
//	return ctr;
//}

// ����ӽڵ�
//ZQuadTree* ZQuadTree::_AddChild(int nCornerTL,int nCornerTR,int nCornerBL,int nCornerBR)
//{
//	ZQuadTree* newNode = new ZQuadTree(this);
//	newNode->_SetCorners(nCornerTL,nCornerTR,nCornerBL,nCornerBR);
//	return newNode;
//}

// ����4����ֵ
//bool ZQuadTree::_SetCorners(int nCornerTL,int nCornerTR,int nCornerBL,int nCornerBR)
//{
//	m_iCorner[CORNER_TL] = nCornerTL;
//	m_iCorner[CORNER_TR] = nCornerTR;
//	m_iCorner[CORNER_BL] = nCornerBL;
//	m_iCorner[CORNER_BR] = nCornerBR;
//	m_iCenter = (m_iCorner[CORNER_TL] + m_iCorner[CORNER_TR] + m_iCorner[CORNER_BL] + m_iCorner[CORNER_BR]) >> 2;
//	return true;
//}

// �ڴ洢����ɾ���Ĳ���
//void ZQuadTree::_Destroy()
//{
//	for (int i=0;i < 4;i++)
//	{
//		if (m_pChild[i] != NULL)
//			delete m_pChild[i];
//	}
//}