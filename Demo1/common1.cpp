#include "StdAfx.h"
#include "common1.h"
#include <time.h>

const DWORD Particle::_PFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE /*| D3DFVF_PSIZE*/;//D3DFVF_PSIZE指定灵活的顶点格式

extern "C" float __GetDistance(const D3DXVECTOR3 &pv1,const D3DXVECTOR3 &pv2)
{
	return D3DXVec3Length(&(pv2 - pv1));
}

float CalcSlope(const D3DXVECTOR3 &v1,const D3DXVECTOR3 &v2)
{
	float fY = (v2.y-v1.y);
	float fZ = __GetDistance(v1, v2);//注意v2 v1顺序
	return asin(fY/fZ);
//	return fY>0 ? -asin(fY/fZ) : asin(fY/fZ);
}

HRESULT CopyName(LPCSTR Name,LPSTR *pNewName)
{
	UINT cbLength;

	if( Name != NULL )
	{
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy( *pNewName,Name,cbLength*sizeof(CHAR) );
		(*pNewName)[cbLength-1]=0;
	}
	else
	{
		*pNewName = NULL;
	}
	return S_OK;
}


D3DXVECTOR3 DirToRot(const D3DXVECTOR3 &vDir,const D3DXVECTOR3 &vOrigRot)
{
	D3DXVECTOR3 vZ1(0,0,-1);//指向正前方
	D3DXVECTOR3 vDirTemp=vDir;
	D3DXVECTOR3	vOut;
	vDirTemp.y=0;//与vZ1在xoz平面上
	D3DXVec3Normalize(&vDirTemp,&vDirTemp);

	float fDot = D3DXVec3Dot( &vZ1, &vDirTemp );//求出两个向量的点积(即两向量的夹角cos值)
	if(0 >= vDirTemp.x)//表示绕Y轴顺时针旋转acos(fDot)角度，已0,0,0为原点，判断vDir的方向，然后求得夹角
		vOut = D3DXVECTOR3(0, acos(fDot), 0);
	else
		vOut = D3DXVECTOR3(0, 2*D3DX_PI - acos(fDot), 0);
	vOut+=vOrigRot;//加上原始旋转矩阵

	return vOut;
}

void DrawLine(LPDIRECT3DDEVICE9 pDevice,
						  D3DXVECTOR3 &v1,
						  D3DXVECTOR3 &v2,
						  D3DCOLOR color)
{
	struct D3D_VERTEX
	{
		D3DXVECTOR3 pos;
		D3DCOLOR color;
	};

	D3D_VERTEX point[2];

	point[0].pos = v1;
	point[0].color = color;
	point[1].pos = v2;
	point[1].color = color;

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pDevice->SetTransform(D3DTS_WORLD,&matWorld);
	DWORD data;
	pDevice->GetRenderState(D3DRS_LIGHTING,&data);
	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

	pDevice->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE );
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST,1,point,sizeof(D3D_VERTEX));

	pDevice->SetRenderState(D3DRS_LIGHTING,data);
}

bool _Equals(float lhs, float rhs)
{
	// if lhs == rhs their difference should be zero
	return fabs(lhs - rhs) < EPSILON ? true : false;
}

D3DXVECTOR3 RandomVectorFormBoundBox(const BoundingBox &BoundBox)
{
	static bool bSrand=false;
	if(!bSrand){
		time_t t;
		srand((unsigned)time(&t));
		bSrand=true;
	}

	D3DXVECTOR3 vRet;
	
	float nRenderX = float(rand()%10000)*0.0001f*(BoundBox._max.x-BoundBox._min.x);
	float nRenderY = float(rand()%10000)*0.0001f*(BoundBox._max.y-BoundBox._min.y);
	float nRenderZ = float(rand()%10000)*0.0001f*(BoundBox._max.z-BoundBox._min.z);
	
	vRet.x= BoundBox._min.x +nRenderX;
	vRet.y= BoundBox._min.y +nRenderY;
	vRet.z= BoundBox._min.z +nRenderZ;

	return vRet;
}

void ZeroVector3(D3DXVECTOR3 &v)
{
	v.x=0;
	v.y=0;
	v.z=0;
}

bool BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if(p.y>=_min.y&&p.y<=_max.y)
	{
		if((p.z>=_min.z&&p.z<=_max.z) || (p.z>=_max.z&&p.z<=_min.z))
		{
			if((p.x>=_min.x&&p.x<=_max.x) || (p.x>=_max.x&&p.x<=_min.x))
			{
				return true;
			}
		}
	}
	return false;
}
DWORD FtoW(float f)
{
	return *((DWORD*)&f);
}
