#pragma once
#include "CommonAPI.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "memory.h"
//lib
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "Winmm.lib")

#define COLOR_WRITE		D3DCOLOR_XRGB(255, 255, 255)
// 地形结构
struct LandVertex
{
	float x,y,z;	
	float u,v;
};
#define LAND_FVF D3DFVF_XYZ|D3DFVF_TEX1

//粒子
struct Particle
{
	D3DXVECTOR3		_position;
	D3DCOLOR			_color;
	//float						_size;
	static const DWORD _PFVF;
};

struct AttributeP
{
	D3DXVECTOR3		_position;//世界空间中的pos
	D3DXVECTOR3		_velocity;//速度
	D3DXVECTOR3		_acceleration;//加速度
	D3DXVECTOR3		_vDir;//移动方向
	float						_lifeTime;//生命周期
	float						_age;//当前年龄
	D3DXCOLOR			_colorAlive;//颜色
	bool						_isAlive;//生死
	AttributeP():_lifeTime(0),_age(0),_isAlive(false){};
};

//包围盒，球
const float INFINITY = FLT_MAX;
const float EPSILON = 0.001f;
struct BoundingBox
{
	BoundingBox():_max(INFINITY,INFINITY,INFINITY),_min(-INFINITY,-INFINITY,-INFINITY){};
	bool isPointInside(D3DXVECTOR3& p);
	
	
	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};
struct BoundingSphere
{
	BoundingSphere():_radius(0.0f){};
	D3DXVECTOR3 _center;
	float _radius;
};

//射线
struct CRay
{
	D3DXVECTOR3		vOrigin;
	D3DXVECTOR3		vDir;
	CRay():vOrigin(0.0f,0.0f,0.0f), vDir(0.0f,0.0f,0.0f){};
};


#define V(x)           { hr = x; }
#define DEBUG_FVF (D3DFVF_XYZ|D3DFVF_DIFFUSE)


struct DEBUGVERTEX
{
	D3DXVECTOR3 position; // The 3D position for the vertex
	DWORD color;
};

// 求得两个点的距离
extern "C" float __GetDistance(const D3DXVECTOR3 &pv1,const D3DXVECTOR3 &pv2);

//求两个向量的斜率
float CalcSlope(const D3DXVECTOR3 &v1,const D3DXVECTOR3 &v2);

// 方向转换偏移量
D3DXVECTOR3 DirToRot(const D3DXVECTOR3 &vDir,const D3DXVECTOR3 &vOrigRot);

//比较
bool _Equals(float lhs, float rhs);

//在包围盒中随机产生一个点
D3DXVECTOR3 RandomVectorFormBoundBox(const BoundingBox &BoundBox);

//置一个向量为0；
void ZeroVector3(D3DXVECTOR3 &v);

//画直线
void DrawLine(LPDIRECT3DDEVICE9 pDevice,
			  D3DXVECTOR3 &v1,
			  D3DXVECTOR3 &v2,
			  D3DCOLOR color);


DWORD FtoW(float f);

HRESULT CopyName(LPCSTR Name,LPSTR *pNewName);

/*********************************************************************************************************
D3DXVec3TransformCoord(v1, v2, mat);//点v2与矩阵mat相乘，得到点的矩阵转换v1

v1-v2=v3: 向量v1v2的方向向量v3

D3DXIntersect,D3DXIntersectTri : 判断射线是否相交于ID3DXMesh

*********************************************************************************************************/

/*********************************************************************************************************
inline: 代码直接放在符号表中，使用时直接替换，函数内容要简单直接，太复杂展开的时候代码膨胀的太厉害
反而效益会小于它带来的效率，最重要的使用是用于类的存取函数

*********************************************************************************************************/
