#include "StdAfx.h"
#include "Terrain.h"
#include "atlimage.h"
#include "ResFileManager.h"
#include "DXDevice.h"

CTerrain::CTerrain(IDirect3DDevice9* pDevice)
{
	m_pDevice=pDevice;

	m_nCol=0;//列格子数量
	m_nRow=0;
	m_nCellSpace=0;//格子size
	m_nHeightScale=0;//高度比
	m_nWidth=0;//宽度
	m_nDepth=0;//长度
	m_pHeightmap=NULL;//高度图信息
	m_nVertexNum=0;
	m_nTriangleNum=0;
	m_fLods=0.0f;
	m_pTexture=NULL;
	m_pQuadTree = NULL;
}

CTerrain::~CTerrain(void)
{
	DeleteAndNull(m_pHeightmap);
	DeleteAndNull(m_pQuadTree);
}

HRESULT CTerrain::Init(CZFrustum *pFrustum)
{
	CResFileManager::Instance()->GetTerrainAttribute(m_nRow, m_nCol, m_nCellSpace, m_nHeightScale);
	CResFileManager::Instance()->GetLods(m_fLods);
	m_nVCol = m_nCol+1;
	m_nVRow = m_nRow+1;
	m_nVertexNum=m_nVCol*m_nVRow;
	m_nWidth = m_nCol*m_nCellSpace;
	m_nDepth = m_nRow*m_nCellSpace;

	if( FAILED(LoadHeightMap()) || FAILED(LoadTexture()) )
		return E_FAIL;

	if(ComputeVertex() || ComputeIndex())
		return E_FAIL;

	m_pFrustum=pFrustum;
	m_pQuadTree = new ZQuadTree(m_nVCol, m_nVRow);

	LandVertex *pVB;
	m_pVB->Lock(0,0,(void**)&pVB, 0);
	m_pVB->Unlock();
	if(FAILED(m_pQuadTree->Build(pVB)))
		return E_FAIL;

	return S_OK;
   }

HRESULT CTerrain::LoadTexture()
{
	return D3DXCreateTextureFromFile(m_pDevice, CResFileManager::Instance()->GetTerrainTextureFilePath().c_str(), &m_pTexture);
}

HRESULT CTerrain::LoadHeightMap()
{
	m_pHeightmap = new char[m_nRow*m_nCol];
	memset(m_pHeightmap, 0x0, sizeof(char)*(m_nRow*m_nCol));

	//way.1
// 	std::ifstream InFile(CResFileManager::Instance()->GetTerrainHeightMapFilePath().c_str(), 
// 		std::ios_base::binary);
// 	if(!InFile)
// 		return E_FAIL;
// 	
// 	InFile.read(m_pHeightmap, sizeof(char)*(m_nWidth*m_nDepth-1));
	
	//way.2
// 	CImage HightmapImg;
// 	if(FAILED(HightmapImg.Load(CResFileManager::Instance()->GetTerrainHeightMapFilePath().c_str())))
// 		return E_FAIL;
// 
// 	int ImgWidth = HightmapImg.GetWidth()/m_nCellSpace;
// 	int ImgHeight = HightmapImg.GetHeight()/m_nCellSpace;
// 
// 	for (int nwidth=0; nwidth<ImgWidth; ++nwidth)
// 	{
// 		for (int nheight=0; nheight<ImgHeight; ++nheight)
// 		{
// 			*(m_pHeightmap+nwidth*nheight+nheight)=(char)HightmapImg.GetPixel(nwidth, nheight);
// 		}
// 	}

	//way.3
	BITMAP bmp;
	HBITMAP hBmp;
	HDC hDC;
	HBITMAP oldBmp;

	hDC = CreateCompatibleDC(NULL);
	hBmp = (HBITMAP)LoadImage(NULL,CResFileManager::Instance()->GetTerrainHeightMapFilePath().c_str(),
		IMAGE_BITMAP,m_nCol,m_nRow,LR_LOADFROMFILE);
	oldBmp = (HBITMAP)SelectObject(hDC,hBmp);

	// 从高度图中读取高度信息
	GetObject(hBmp,sizeof(bmp),&bmp);
	LONG iCol = bmp.bmWidth;
	LONG iRow = bmp.bmHeight;
	DWORD color;
	for (int i=0;i<iRow;i++)
	{
		for (int j=0;j<iCol;j++)
		{
			color = GetPixel(hDC,j,i);
			m_pHeightmap[i*iCol+j] = *(BYTE*)&color;
		}
	}
	DeleteObject(oldBmp);
	DeleteObject(hDC);
	DeleteObject(hBmp);
	
	return S_OK;
}

HRESULT CTerrain::ComputeVertex()
{
	HRESULT hr = 0;
	hr = m_pDevice->CreateVertexBuffer(
		m_nVertexNum * sizeof(LandVertex),
		D3DUSAGE_WRITEONLY,
		LAND_FVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0);
	if (FAILED(hr))
		return E_FAIL;

	//视中心点为原点, 从地形的左上角开始
	float nStartX=-((float)m_nWidth/2.0);
	float nStartZ=(float)m_nDepth/2.0;
	float nUCellsize=1.0/(float)m_nCol;
	float nVCellsize=1.0/(float)m_nRow;

	LandVertex* pV, *pTmp;
	if (FAILED(m_pVB->Lock(0,m_nVertexNum * sizeof(LandVertex),(void**)&pV,0)))
		return E_FAIL;

	int n1=0, n2=0;
	int nTestCount=0;
	for (n1=0; n1<m_nVRow; ++n1)
	{
		for (n2=0; n2<m_nVCol; ++n2)
		{
			++nTestCount;
			pTmp = pV+n1*m_nVCol+n2;	
			pTmp->x=nStartX+n2*m_nCellSpace;
			if(n2 == m_nVCol-1 || n1 == m_nVRow-1 || 0 == n2*n1 )
				pTmp->y=255*m_nHeightScale;
			else
				pTmp->y=*(m_pHeightmap+n1*m_nVCol+n2)*m_nHeightScale;
			pTmp->z=nStartZ-n1*m_nCellSpace;
			pTmp->u=n2*nUCellsize;
			pTmp->v=n1*nVCellsize;
		}
	}
	m_pVB->Unlock();
	

// 	//对应第一个顶点坐标和最后一个顶点坐标
// 	int startX = - m_nWidth/ 2;
// 	int startZ = m_nDepth / 2;
// 	int endX = m_nWidth / 2;
// 	int endZ = - m_nDepth / 2;
// 
// 	// 计算纹理u，v每格距离
// 	float uCellSize = 1.0f / (float)m_nCol;
// 	float vCellSize = 1.0f / (float)m_nRow;
// 	LandVertex* pV;
// 	if (FAILED(m_pVB->Lock(0,m_nVertexNum * sizeof(LandVertex),(void**)&pV,0)))
// 		return false;

// 	int i = 0;
// 	
// 	for (int z = startZ;z >= endZ;z -= m_nCellSpace)
// 	{
// 		int j = 0;
// 		for (int x = startX;x <= endX;x += m_nCellSpace)
// 		{
// 			++nTestCount;
// 			int index = i * m_nVCol + j;
// 			pV[index].x = (float)x;
// 			if (i != (m_nRow - 1) && j != (m_nVCol - 1))
// 			{
// 				int heightIndex = i * (m_nVCol - 1) + j;
// 				pV[index].y = (float)m_pHeightmap[heightIndex];
// 			}
// 			// 补最后一行和一列
// 			else
// 			{
// 				pV[index].y = 0;
// 			}
// 			pV[index].z = (float)z;
// 			pV[index].u = (float)j * uCellSize;
// 			pV[index].v = (float)i * vCellSize;
// 			j++;
// 		}
// 		i++;
// 	}
//	m_pVB->Unlock();
	return S_OK;
}

HRESULT CTerrain::ComputeIndex()
{
	HRESULT hr = 0;
	int nIndexNum=(m_nVCol-1)*(m_nVRow-1)*6;
//	nIndexNum=6;//test code
	hr = m_pDevice->CreateIndexBuffer(
		nIndexNum*sizeof(DWORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL);
	if (FAILED(hr))
		return false;

	DWORD* pI,*pOrgI;
	m_pIB->Lock(0,0,(void**)&pOrgI,0);
	

// 	for (i=0,j=0;i<m_nVertexNum;i++)
// 	{
// 		if((i+1)%m_nVCol && i/m_nVCol!=m_nRow-1)//计算奇数列(再向右一次就可以算出三角形)和
// 		{
// 			pI[j+0] = /*(WORD)*/i;
// 			pI[j+1] = i + 1;
// 			pI[j+2] = i + m_nVCol;
// 			pI[j+3] = i + m_nVCol;
// 			pI[j+4] = i + 1;
// 			pI[j+5] = i + m_nVCol + 1;
// 			j += 6;
// 			nCount++;
// 		}
// 	}

	int n1=0, n2=0;
	int nCount = 0;
	pI=pOrgI;
	for (n1=0; n1<m_nVRow-1; ++n1)
	{
		for (n2=0; n2<m_nVCol-1; ++n2)
		{
			*(pI+0)=( (n1)*m_nVCol + n2 );
			*(pI+1)=(*(pI+0)+1);
			*(pI+2)=( (n1+1)*m_nVCol + n2);
			
			*(pI+3)=*(pI+2);
			*(pI+4)=*(pI+1);
			*(pI+5)=(*(pI+2)+1);

			pI+=6;
			nCount++;
		}
	}

	m_pIB->Unlock();

	m_nTriangleNum = nIndexNum/3;
	return S_OK;
}

void CTerrain::Render()
{
	//固定不动
 	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pDevice->SetRenderState(D3DRS_LIGHTING,false);
	m_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);//打开深度测试
	m_pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESS);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);//打开深度缓冲区


	if (m_pTexture)
	{
		m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);//在第n stage对纹理的哪个颜色通道执行那种混合操作
		m_pDevice->SetTexture(0, m_pTexture);
	}
	m_pDevice->SetFVF(LAND_FVF);
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(LandVertex));
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_nVertexNum,0,m_nTriangleNum);

}

void CTerrain::Update()
{
	LandVertex *pVB;
	DWORD *pIB;
	m_pVB->Lock(0,0,(void**)&pVB,0);
	m_pIB->Lock(0, 0, (void**)&pIB, 0);
	m_nTriangleNum = m_pQuadTree->GenerateIndex( pIB, pVB, m_pFrustum, m_fLods);
	m_pVB->Unlock();
	m_pIB->Unlock();

	
}

float CTerrain::GetTHeight(float x, float z)
{
	float nStartX=-((float)m_nWidth/2.0);
	float nStartZ=(float)m_nDepth/2.0;

	int nCellX = abs(x-nStartX)/m_nCellSpace;
	int nCellZ = abs(nStartZ-z)/m_nCellSpace;

	LandVertex *pVB;
	D3DXVECTOR3 vtrA, vtrB, vtrC, vtrD, vtrCenter;
	m_pVB->Lock(0, 0, (void**)&pVB, 0);
	vtrA = *(D3DXVECTOR3*)(pVB+nCellZ*m_nVCol + nCellX);
	vtrB = *(D3DXVECTOR3*)(pVB+nCellZ*m_nVCol + nCellX + 1);
	vtrC = *(D3DXVECTOR3*)(pVB+(nCellZ+1)*m_nVCol + nCellX);
	vtrD = *(D3DXVECTOR3*)(pVB+(nCellZ+1)*m_nVCol + nCellX + 1);	
	m_pVB->Unlock();

	//这里只算格子的中心点，可优化，pos和Look点的射线与格子的交点
	/*vtrCenter = (vtrA+vtrB+vtrC+vtrD)/4.0f;*/

	
	D3DXVECTOR3 vRayDir(0,-1,0), vOut;//射线往下的方向(单位向量)
	D3DXVECTOR3 vPos(x, 255*m_nHeightScale, z);//Ray射线的起点

	float fu,fv,fdist;
	if(D3DXIntersectTri(&vtrA, &vtrB, &vtrC, &vPos, &vRayDir, &fu, &fv, &fdist))
	{
		vOut = vtrA + fu*(vtrB-vtrA) + fv*(vtrC-vtrA);
		return vOut.y;
	}
	else if(D3DXIntersectTri(&vtrC, &vtrB, &vtrD, &vPos, &vRayDir, &fu, &fv, &fdist))
	{
		vOut = vtrC + fu*(vtrB-vtrC) + fv*(vtrD-vtrC);
		return vOut.y;
	}
	else 
		return 0;
}

bool CTerrain::IntersectTerrain(D3DXVECTOR3 &vRayOrigin, D3DXVECTOR3 &vRayDir, float *fDist, D3DXVECTOR3 *vRet)
{
	
	D3DXVec3Normalize(&vRayDir, &vRayDir);

	LandVertex* pV;
	void *pI;
	D3DINDEXBUFFER_DESC IndexDesc;
	D3DXVECTOR3* vTris[3];
	D3DXVECTOR3 vTriInter;
	int nIndex = 0, nIndexTri=0;
	float fU=0.0f, fV=0.0f, fTDist=0.0f, fDefaultDist = 1e+32f;
	if (FAILED(m_pVB->Lock(0,m_nVertexNum * sizeof(LandVertex),(void**)&pV,0)))
		return false;
	if(FAILED(m_pIB->Lock(0,0,(void**)&pI,0))){
		m_pVB->Unlock();
		return false;
	}
	m_pIB->GetDesc(&IndexDesc);
	
	for (int nTri = 0; nTri < m_nTriangleNum; ++nTri)
	{
		for (int i=0; i<3; ++i)
		{
			switch( IndexDesc.Format )
			{
			case D3DFMT_INDEX16:{
				nIndex = *((WORD*)pI+nTri*3+i);
								}break;

			case D3DFMT_INDEX32:{
				nIndex = *((DWORD*)pI+nTri*3+i);
								}break;
			}
			vTris[i] = (D3DXVECTOR3*)(pV+nIndex);
		}

		if(D3DXIntersectTri(vTris[0], vTris[1], vTris[2], &vRayOrigin, &vRayDir, &fU, &fV, &fTDist))
		{
			if(fTDist < fDefaultDist)
			{
				fDefaultDist = fTDist;
				vTriInter = *(vTris[0])+fU*(*(vTris[1])-*(vTris[0]))+fV*(*(vTris[2])-*(vTris[0]));
			}
		}
	}
	m_pVB->Unlock();
	m_pIB->Unlock();

	if(1e+32f == fDefaultDist)
		return false;

	if(fDist)
		*fDist = fDefaultDist;
	if(vRet)
		*vRet = vTriInter;

	return true;
}

// D3DXVECTOR3 CTerrain::ComputeMouseClickVtr()
// {
// 	D3DXMATRIX RevProjMat;
// 	POINT CursorPos;
// 	GetCursorPos(&CursorPos);
// 	ScreenToClient(CDXDevice::Instance()->GetMainHwnd(), &CursorPos);
// 	CDXDevice::Instance()->GetD3Device()->GetTransform(D3DTS_PROJECTION, &RevProjMat);
// 	D3DXMatrixInverse(&RevProjMat, NULL, &RevProjMat);
// 
// }


D3DXVECTOR3 CTerrain::CalcMouseClickVtr()
{
	D3DXMATRIX RevProjMat, ProjMat, RevViewMat, ViewMat;
	D3DXVECTOR3 vRet;
	D3DVIEWPORT9 ViewPort;
	POINT CursorPos;
	CRay Ray;
	GetCursorPos(&CursorPos);
	ScreenToClient(CDXDevice::Instance()->GetMainHwnd(), &CursorPos);

	m_pDevice->GetTransform(D3DTS_VIEW, &ViewMat);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &ProjMat);
	//D3DXMatrixInverse(&RevProjMat, NULL, &ProjMat);
	D3DXMatrixInverse(&RevViewMat, NULL, &ViewMat);
	m_pDevice->GetViewport(&ViewPort);

	//通过视口矩阵，得到投影窗口的点
	Ray.vDir.x = ((float)(2.0f*CursorPos.x)/(float)ViewPort.Width-1.0f);
	Ray.vDir.y = ((float)(-2.0f*CursorPos.y)/(float)ViewPort.Height+1.0f);
	Ray.vDir.z = 1.0f;

	//再转换到视图空间
	Ray.vDir.x /= ProjMat(0,0);
	Ray.vDir.y /= ProjMat(1,1);

	//转换到世界空间
	D3DXVec3TransformNormal(&Ray.vDir, &Ray.vDir, &RevViewMat);
	D3DXVec3TransformCoord(&Ray.vOrigin, &Ray.vOrigin, &RevViewMat);

	IntersectTerrain(Ray.vOrigin, Ray.vDir, NULL, &vRet);

	return vRet;
}