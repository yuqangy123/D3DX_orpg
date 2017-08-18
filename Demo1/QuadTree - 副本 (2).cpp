#include "StdAfx.h"
#include "QuadTree.h"
#include "math.h"
#include "memory.h"

CQuadTree::CQuadTree(int nx, int ny)
{
	
	int nWidth=0, nDepth=0;
	nWidth=nx>ny?ny:nx;//选择最短的边，计算总节点数
	--nWidth;//边长度比边节点数少一
	
	while (nWidth>=1)//每次深度分一半
	{
		++nDepth;
		nWidth=nWidth>>1;
	}
	
	m_nMaxNodeNum = (int)(pow(4.0, nDepth)-1)/(4-1);//满四叉数的节点数
	m_pTreeHead = new QuadNode[m_nMaxNodeNum];
	m_pDataList=m_pTreeHead;
	memset(m_pTreeHead, -1, m_nMaxNodeNum*sizeof(QuadNode));
	
	//初始化根节点
	m_pTreeHead[0].nodeID=0;
	m_pTreeHead[0].ParentID=0;
	memset(m_pTreeHead[0].ChildsID, -1, sizeof(int)*4);
	memset(m_pTreeHead[0].NeighboursID, -1, sizeof(int)*4);
	m_pTreeHead[0].nCorner[LT]=0;
	m_pTreeHead[0].nCorner[RT]=nx-1;
	m_pTreeHead[0].nCorner[LB]=nx*(ny-1);
	m_pTreeHead[0].nCorner[RB]=(nx*ny)-1;
	m_pTreeHead[0].nCenter=CalcCenter(m_pTreeHead[0].nCorner[LT], m_pTreeHead[0].nCorner[RT], m_pTreeHead[0].nCorner[LB], m_pTreeHead[0].nCorner[RB]);
	m_pTreeHead[0].bCulled=false;
	m_pTreeHead[0].nRadius=0.0f;

	m_nCellSize = 1;
	m_TotalTreeID=0;
}

CQuadTree::~CQuadTree(void)
{
}

int CQuadTree::CalcCenter(int &LT, int &RT, int &LB, int &RB)
{
	return (LT+RT+LB+RB)>>2;
}

bool CQuadTree::Create(LandVertex *pMap)
{
	if(!pMap)
		return false;

	 if(!BuildQuadTree(pMap, 0))
		 return false;

	BuildNeighboursID();
	return true;
}

//提示：nodeID与m_pDataList的数组索引是两码事
bool CQuadTree::BuildQuadTree(LandVertex *pMap, int nNodeID)
{
	if(-1 == nNodeID)
		return false;

	D3DXVECTOR3 vtrCenter=*(D3DXVECTOR3*)(pMap+m_pDataList[nNodeID].nCenter);
	D3DXVECTOR3 vtrCorner=*(D3DXVECTOR3*)(pMap+m_pDataList[nNodeID].nCorner[LT]);
	m_pDataList[nNodeID].nRadius=__GetDistance(vtrCenter, vtrCorner);

	if(BuildSubTree(nNodeID))
	{
		int n;
		for (n=0; n<4; ++n)
		{
			BuildQuadTree(pMap, m_pDataList[nNodeID].ChildsID[n]);
		}
	}
	return true;
}

bool CQuadTree::BuildSubTree(int nNodeID)
{
	if( (m_pDataList[nNodeID].nCorner[RT] -  m_pDataList[nNodeID].nCorner[LT]) <= 1)//边界值为空, 尺寸只考虑方形地形, 正方形格子
		return false;

	int nCornerTopMid = (m_pDataList[nNodeID].nCorner[LT] + m_pDataList[nNodeID].nCorner[RT])>>1;
	int nConerBottomMid = (m_pDataList[nNodeID].nCorner[LB] + m_pDataList[nNodeID].nCorner[RB])>>1;
	int nConerLeftMid = (m_pDataList[nNodeID].nCorner[LT] + m_pDataList[nNodeID].nCorner[LB])>>1;
	int nConerRightMid = (m_pDataList[nNodeID].nCorner[RT] + m_pDataList[nNodeID].nCorner[RB])>>1;

	m_pDataList[nNodeID].ChildsID[0] = CreateNode(nNodeID, m_pDataList[nNodeID].nCorner[LT], nCornerTopMid, nConerLeftMid, m_pDataList[nNodeID].nCenter);
	m_pDataList[nNodeID].ChildsID[1] = CreateNode(nNodeID, nCornerTopMid, m_pDataList[nNodeID].nCorner[RT],  m_pDataList[nNodeID].nCenter, nConerRightMid);
	m_pDataList[nNodeID].ChildsID[2] = CreateNode(nNodeID, nConerLeftMid, m_pDataList[nNodeID].nCenter, m_pDataList[nNodeID].nCorner[LB], nConerBottomMid);
	m_pDataList[nNodeID].ChildsID[3] = CreateNode(nNodeID, m_pDataList[nNodeID].nCenter, nConerRightMid, nConerBottomMid, m_pDataList[nNodeID].nCorner[RB]);
	return true;
}

int  CQuadTree::CreateNode(int nParentID, int &nLT, int &nRT, int &nLB, int &nRB)
{
	if( (nRT-nLT) <= 1 )
		return -1;
	m_pDataList[++m_TotalTreeID].ParentID=nParentID;
	m_pDataList[m_TotalTreeID].nodeID=m_TotalTreeID;
	for (int n=0;n<4;++n)
	{
		m_pDataList[m_TotalTreeID].ChildsID[n]=-1;
		m_pDataList[m_TotalTreeID].NeighboursID[n]=-1;
	}
// 	memset(m_pDataList[m_TotalTreeID].ChildsID, -1, 4*sizeof(int));
// 	memset(m_pDataList[m_TotalTreeID].NeighboursID, -1, 4*sizeof(int));
	
	m_pDataList[m_TotalTreeID].nCorner[LT]=nLT;
	m_pDataList[m_TotalTreeID].nCorner[RT]=nRT;
	m_pDataList[m_TotalTreeID].nCorner[LB]=nLB;
	m_pDataList[m_TotalTreeID].nCorner[RB]=nRB;

	m_pDataList[m_TotalTreeID].nCenter=CalcCenter(nLT, nRT, nLB, nRB);
	m_pDataList[m_TotalTreeID].bCulled=false;
	m_pDataList[m_TotalTreeID].nRadius=0.0f;
	return m_pDataList[m_TotalTreeID].nodeID;
}

bool CQuadTree::BuildNeighboursID()
{
	for (int n=0; n<m_nMaxNodeNum; ++n)
	{
		/*

		E		    	F
		.				.
		
					
	 A .				. B


		.				.
		C			   D

		E=A*2-C, F=B*2-D	

		*/
		//上邻接格
		int nLT = (m_pDataList[n].nCorner[LT]*2-m_pDataList[n].nCorner[LB]);
		int nRT = (m_pDataList[n].nCorner[RT]*2-m_pDataList[n].nCorner[RB]);
		int nLB = m_pDataList[n].nCorner[LT];
		int nRB = m_pDataList[n].nCorner[RT];
		m_pDataList[n].NeighboursID[Top]=FindNode(nLT, nRT, nLB, nRB);		

		//下邻接格
		nLT = m_pDataList[n].nCorner[LB];
		nRT = m_pDataList[n].nCorner[RB];
		nLB = (m_pDataList[n].nCorner[LB]*2-m_pDataList[n].nCorner[LT]);
		nRB = (m_pDataList[n].nCorner[RB]*2-m_pDataList[n].nCorner[RT]);
		m_pDataList[n].NeighboursID[Bottom]=FindNode(nLT, nRT, nLB, nRB);

		//左邻接格
		nLT = m_pDataList[n].nCorner[LT]*2-m_pDataList[n].nCorner[RT];
		nRT =m_pDataList[n].nCorner[LT];
		nLB = m_pDataList[n].nCorner[LB]*2-m_pDataList[n].nCorner[RB];
		nRB = m_pDataList[n].nCorner[LB];
		m_pDataList[n].NeighboursID[Left]=FindNode(nLT, nRT, nLB, nRB);

		//右邻接格
		nLT = m_pDataList[n].nCorner[RT];
		nRT =m_pDataList[n].nCorner[RT]*2+m_pDataList[n].nCorner[LT];
		nLB = m_pDataList[n].nCorner[RB];
		nRB = m_pDataList[n].nCorner[RB]*3+m_pDataList[n].nCorner[LB];
		m_pDataList[n].NeighboursID[Right]=FindNode(nLT, nRT, nLB, nRB);
	}
	return true;
}

//m_pDataList下标跟NodeID一致才行，否则会发送错误
//查找节点函数可以再优化
int CQuadTree::FindNode(const int &nLT, const int &nRT, const int &nLB, const int &nRB)
{
	for (int n=0;n<m_nMaxNodeNum;++n)
	{
		if(nLT == m_pDataList[n].nCorner[LT] &&
			nRT == m_pDataList[n].nCorner[RT] &&
			nLB == m_pDataList[n].nCorner[LB] &&
			nRB == m_pDataList[n].nCorner[RB])
			return m_pDataList[n].nodeID;
	}
	return -1;
}

int CQuadTree::GenerateIndex(int nID, DWORD* pIB, LandVertex *pVB, CZFrustum *pFrustum, float fLodRatio)
{
	Reset();
		
	FrustumCull(nID, pVB, pFrustum);

	int nCount=0;
	for (int n=0; n< m_nMaxNodeNum; ++n)
	{
		if(m_pDataList[n].bCulled == false)
			++nCount;
	}
// 	D3DXVECTOR3 FrustumCameraPos = pFrustum->GetPos();
// 	printf("bull is false count:%d, Frustum is pos:%f,%f,%f\r\n",nCount, FrustumCameraPos.x, FrustumCameraPos.y, FrustumCameraPos.z);
// 	
	return GenFrutumIndex(nID, 0, pIB, pVB, pFrustum, fLodRatio);
}

void CQuadTree::Reset()
{
	for (int n=0; n<m_nMaxNodeNum; ++n)
	{
		m_pDataList[n].bCulled=true;
	}
}

void CQuadTree::FrustumCull(int nID, LandVertex *pVB, CZFrustum *pFrustum)
{
	if(IsInFrustum(nID, pVB, pFrustum))
		m_pDataList[nID].bCulled=false;
	else 
		return;
	
	if(-1 != m_pDataList[nID].ChildsID[LT])
		FrustumCull(m_pDataList[nID].ChildsID[LT], pVB, pFrustum);

	if(-1 != m_pDataList[nID].ChildsID[RT])
		FrustumCull(m_pDataList[nID].ChildsID[RT], pVB, pFrustum);

	if(-1 != m_pDataList[nID].ChildsID[LB])
		FrustumCull(m_pDataList[nID].ChildsID[LB], pVB, pFrustum);

	if(-1 != m_pDataList[nID].ChildsID[RB])
		FrustumCull(m_pDataList[nID].ChildsID[RB], pVB, pFrustum);
}

bool CQuadTree::IsInFrustum(int nID, LandVertex *pVB, CZFrustum *pFrustum)
{
	D3DXVECTOR3 *vtrCenter = (D3DXVECTOR3*)(pVB+m_pDataList[nID].nCenter);

	if(pFrustum->IsIn(vtrCenter)){
		return true;//这里可以判断四个角是否在内，优化捡取算法
	}
	else{
		return false;
	}
}

int CQuadTree::GenFrutumIndex(int ID, int nTris, 
							  DWORD* pIB, LandVertex *pVB, CZFrustum *pFrustum, float fLodRatio)
{
	if(Visible(ID, pVB, pFrustum, fLodRatio )){
		/*
		DWORD *pTempIB=pIB+3*nTriNum;
		*pTempIB++=m_pDataList[nID].nCorner[LT];
		*pTempIB++=m_pDataList[nID].nCorner[LB];
		*pTempIB++=m_pDataList[nID].nCorner[RB];
		++nTriNum;

		*pTempIB++=m_pDataList[nID].nCorner[LT];
		*pTempIB++=m_pDataList[nID].nCorner[RB];
		*pTempIB++=m_pDataList[nID].nCorner[RT];
		++nTriNum;//从最小lod等级的tri开始
		return nTriNum;
		*/
		DWORD* p = ((DWORD*)pIB) + nTris * 3;

#define	TreeList		m_pDataList
#define iCorner		nCorner
#define	iCenter		nCenter
		// 判断四个方向的邻近节点LOD等级与当前节点关系（注意是当前单元格数与邻近LOD比较，非LOD与LOD比较）
		int iRepair[4] = {0,0,0,0};
		int NeighborLod;
		for (int i=0;i<4;i++)
		{
			if (m_pDataList[ID].ChildsID[0] != -1 && m_pDataList[ID].NeighboursID[i] != -1)
			{
				NeighborLod = _GetLODLevel(m_pDataList[ID].NeighboursID[i],pVB,&pFrustum->GetPos(),fLodRatio);
				if (m_pDataList[ID].nCorner[RT] - m_pDataList[ID].nCorner[LT] > NeighborLod)
					iRepair[i] = 1;
			}
		}
		// 四个方向相同
		if ((iRepair[0] + iRepair[1] + iRepair[2] + iRepair[3]) == 0)
		{
			// 左上方三角形    
			*p++ = m_pDataList[ID].nCorner[0];   
			*p++ = m_pDataList[ID].nCorner[1];   
			*p++ = m_pDataList[ID].nCorner[2];   
			nTris++;   
			// 右下方三角形    
			*p++ = m_pDataList[ID].nCorner[2];   
			*p++ = m_pDataList[ID].nCorner[1];   
			*p++ = m_pDataList[ID].nCorner[3];
			nTris++;   
		}
		else
		{
			// 上方补三角
			if (iRepair[0] == 1)
			{
				int nCornerTM = (m_pDataList[ID].nCorner[0] + m_pDataList[ID].nCorner[1]) >> 1;
				*p++ = m_pDataList[ID].nCorner[0];
				*p++ = nCornerTM;
				*p++ = m_pDataList[ID].nCenter;
				nTris++;
				*p++ = nCornerTM;
				*p++ = m_pDataList[ID].nCorner[1];
				*p++ = m_pDataList[ID].nCenter;
				nTris++;	
			}
			else
			{
				*p++ = m_pDataList[ID].nCorner[0];
				*p++ = m_pDataList[ID].nCorner[1];
				*p++ = m_pDataList[ID].nCenter;
				nTris++;
			}
			// 下方补三角
			if (iRepair[1] == 1)
			{
				int nCornerBM = (m_pDataList[ID].nCorner[2] + m_pDataList[ID].nCorner[3]) >> 1;
				*p++ = m_pDataList[ID].nCorner[2];
				*p++ = m_pDataList[ID].nCenter;
				*p++ = nCornerBM;
				nTris++;
				*p++ = nCornerBM;
				*p++ = m_pDataList[ID].nCenter;
				*p++ = m_pDataList[ID].nCorner[3];
				nTris++;
			}
			else
			{
				*p++ = m_pDataList[ID].nCorner[2];
				*p++ = m_pDataList[ID].nCenter;
				*p++ = m_pDataList[ID].nCorner[3];
				nTris++;
			}
			// 左方补三角
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
			// 右方补三角
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

	if(-1 != m_pDataList[ID].ChildsID[LT])
		nTris = GenFrutumIndex(m_pDataList[ID].ChildsID[LT], nTris, pIB, pVB, pFrustum, fLodRatio);
	if(-1 != m_pDataList[ID].ChildsID[RT])
		nTris = GenFrutumIndex(m_pDataList[ID].ChildsID[RT], nTris, pIB, pVB, pFrustum, fLodRatio);
	if(-1 != m_pDataList[ID].ChildsID[LB])
		nTris= GenFrutumIndex(m_pDataList[ID].ChildsID[LB], nTris, pIB, pVB, pFrustum, fLodRatio);
	if(-1 != m_pDataList[ID].ChildsID[RB])
		nTris = GenFrutumIndex(m_pDataList[ID].ChildsID[RB], nTris, pIB, pVB, pFrustum, fLodRatio);

	return nTris;
}

bool CQuadTree::Visible(int nID, LandVertex *pVB, CZFrustum *pFrustum, float fLodRatio)
{
	float d1 = __GetDistance( *(D3DXVECTOR3*)(pVB+m_pDataList[nID].nCorner[LT]), *(D3DXVECTOR3*)(pVB+m_pDataList[nID].nCorner[RT]) );
	float d2 = __GetDistance(pFrustum->GetPos(), *(D3DXVECTOR3*)(pVB+m_pDataList[nID].nCenter));
	d2 *= fLodRatio;
	if(d2 < 1.0f)
		d2=1.0f;

//	printf("d1=%f, d2=%f\r\n", d1, d2);
	return d1 <= d2;
}