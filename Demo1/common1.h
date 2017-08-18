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
// ���νṹ
struct LandVertex
{
	float x,y,z;	
	float u,v;
};
#define LAND_FVF D3DFVF_XYZ|D3DFVF_TEX1

//����
struct Particle
{
	D3DXVECTOR3		_position;
	D3DCOLOR			_color;
	//float						_size;
	static const DWORD _PFVF;
};

struct AttributeP
{
	D3DXVECTOR3		_position;//����ռ��е�pos
	D3DXVECTOR3		_velocity;//�ٶ�
	D3DXVECTOR3		_acceleration;//���ٶ�
	D3DXVECTOR3		_vDir;//�ƶ�����
	float						_lifeTime;//��������
	float						_age;//��ǰ����
	D3DXCOLOR			_colorAlive;//��ɫ
	bool						_isAlive;//����
	AttributeP():_lifeTime(0),_age(0),_isAlive(false){};
};

//��Χ�У���
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

//����
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

// ���������ľ���
extern "C" float __GetDistance(const D3DXVECTOR3 &pv1,const D3DXVECTOR3 &pv2);

//������������б��
float CalcSlope(const D3DXVECTOR3 &v1,const D3DXVECTOR3 &v2);

// ����ת��ƫ����
D3DXVECTOR3 DirToRot(const D3DXVECTOR3 &vDir,const D3DXVECTOR3 &vOrigRot);

//�Ƚ�
bool _Equals(float lhs, float rhs);

//�ڰ�Χ�����������һ����
D3DXVECTOR3 RandomVectorFormBoundBox(const BoundingBox &BoundBox);

//��һ������Ϊ0��
void ZeroVector3(D3DXVECTOR3 &v);

//��ֱ��
void DrawLine(LPDIRECT3DDEVICE9 pDevice,
			  D3DXVECTOR3 &v1,
			  D3DXVECTOR3 &v2,
			  D3DCOLOR color);


DWORD FtoW(float f);

HRESULT CopyName(LPCSTR Name,LPSTR *pNewName);

/*********************************************************************************************************
D3DXVec3TransformCoord(v1, v2, mat);//��v2�����mat��ˣ��õ���ľ���ת��v1

v1-v2=v3: ����v1v2�ķ�������v3

D3DXIntersect,D3DXIntersectTri : �ж������Ƿ��ཻ��ID3DXMesh

*********************************************************************************************************/

/*********************************************************************************************************
inline: ����ֱ�ӷ��ڷ��ű��У�ʹ��ʱֱ���滻����������Ҫ��ֱ�ӣ�̫����չ����ʱ��������͵�̫����
����Ч���С����������Ч�ʣ�����Ҫ��ʹ����������Ĵ�ȡ����

*********************************************************************************************************/
